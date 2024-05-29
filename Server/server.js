const express = require('express');
const mongoose = require('mongoose');
const ping = require('ping');
const bodyParser = require('body-parser');

require('dotenv').config();

const URI = process.env.MONGO_URI;
const PORT = 4000;

const app = express();

app.set('view engine', 'ejs');
app.set('trust proxy', true);

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.json());
app.use(express.static('public'));

app.use((req,res,next) =>{
  req.time = new Date(Date.now()).toString();
  console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[36m%s\x1b[0m', req.method, req.path, res.statusCode, req.socket.address());
  next();
});


var chartData = [];
var recentActivity = [];

mongoose.connect(URI)
.then()
  console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mMongo DB\x1b[32m sucessfully\x1b[0m connected!');

async function isIpOnline(ip) {
  return new Promise((resolve, reject) => {
      ping.sys.probe(ip, function(isAlive) {
          resolve(isAlive);
      });
  });
}
const Farm = mongoose.model('Farm', {
  name: String, 
  ssid: String, 
  password: String, 
  ip: String,
  cameraIP: String,
  plantType: String, 
  soilHum: Number, 
  airTemp: Number, 
  airHum: Number, 
  light: Number
});


app.get('/', async (req, res) => {
  try {
      const farms = [];
      let activeFarms = 0;
      let nonActiveFarms = 0;
      let strawberryCount = 0;
      let cherryCount = 0;
      let potatoCount = 0;
      let tomatoesCount = 0;
      let otherCount = 0;

      const allFarms = await Farm.find({});
      const farmPromises = allFarms.map(async (farm) => {
          farms.push(farm);
          if(farm.plantType == "Potatoes"){potatoCount++}
          else if(farm.plantType == "Strawberries"){strawberryCount++}
          else if(farm.plantType == "Cherries"){cherryCount++}
          else if(farm.plantType == "Tomatoes"){tomatoesCount++}
          else{otherCount++};

          const isOnline = await isIpOnline(farm.ip);
          if (isOnline) {
            activeFarms++;
          } else {
            recentActivity.push("Farm with IP <strong>" + farm.ip + "</strong> disconnected")
            console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mFarm with IP \x1b[90m' + farm.ip + ' \x1b[0mis\x1b[31m down\x1b[0m!');
            nonActiveFarms++;
          }
      });

      await Promise.all(farmPromises);

      var soilHum1Values = [];
      var soilHum2Values = [];
      var airTempValues = [];
      var airHumValues = [];
      var lightValues = [];

      for(var i = 0; i < chartData.length; i++){
        if((6 - chartData[i].data.map(entry => entry.soilHum1).length) > 0){
          soilHum1Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum1).concat(Array(6 - chartData[i].data.map(entry => entry.soilHum1).length).fill(0)).reverse()});
          soilHum2Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum2).concat(Array(6 - chartData[i].data.map(entry => entry.soilHum2).length).fill(0)).reverse()});
          airTempValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airTemp).concat(Array(6 - chartData[i].data.map(entry => entry.airTempValues).length).fill(0)).reverse()});
          airHumValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airHum).concat(Array(6 - chartData[i].data.map(entry => entry.airHumValues).length).fill(0)).reverse()});
          lightValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.light).concat(Array(6 - chartData[i].data.map(entry => entry.lightValues).length).fill(0)).reverse()});
        }
        else{
          soilHum1Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum1).reverse().slice(0,6)});
          soilHum2Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum2).reverse().slice(0,6)});
          airTempValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airTemp).reverse().slice(0,6)});
          airHumValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airHum).reverse().slice(0,6)});
          lightValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.light).reverse().slice(0,6)});
        }
      }

      res.render('index', {
          farms: farms,
          activeFarms: activeFarms,
          nonActiveFarms: nonActiveFarms,
          powerUsage: (0.8 * 9) * 3 * activeFarms,
          tomatoesCount: tomatoesCount,
          potatoCount: potatoCount,
          strawberryCount: strawberryCount,
          cherryCount: cherryCount,
          otherCount: otherCount,
          soilHum1Values,
          soilHum2Values,
          airTempValues,
          airHumValues,
          lightValues,
          recentActivity: recentActivity.reverse().slice(0,15)
      });
  } catch (error) {
      res.status(500).send(error.message);
  }
});

app.post('/add-farm', async (req, res) => {

  Farm.exists({"ip": req.body.ip}).then(async result => { 
    if(!result){
      const farm = new Farm({
        name: "Farm_" + req.body.ip, 
        ssid: req.body.ssid, 
        ip: req.body.ip,
        cameraIP: req.body.camera_ip,
        password: req.body.password, 
        plantType: "Potatoes", 
        soilHum: 50, 
        airTemp: 24, 
        airHum: 399, 
        light:139
      });
    
      await farm.save();

      recentActivity.push("A new farm with IP <strong>" + req.body.ip + "</strong> was added")
      
      await loadFarms();
      console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mFarm with IP \x1b[90m' + req.body.ip + ' \x1b[was added\x1b[32m successfully\x1b[0m!', farm);
      
      await fetch('http://' + req.body.ip + ':80/update')
      .then(
        console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + farm.ip + ' \x1b[0mwas\x1b[32m successfull\x1b[0m!')
      )
      .catch(error => 
        console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + farm.ip + ' \x1b[32m failed\x1b[0m!', error),
      )

      res.send(farm._id.toString());
    }
    else{
      recentActivity.push("Farm with IP <strong>" + req.body.ip + "</strong> connected")
      console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mFarm with IP \x1b[90m' + req.body.ip + ' \x1b[connected\x1b[32m successfully\x1b[0m!');
      await fetch('http://' + req.body.ip + ':80/update')
      .then(
        console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + req.body.ip + ' \x1b[0mwas\x1b[32m successfull\x1b[0m!')
      )
      .catch(error => 
        console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + req.body.ip + ' \x1b[32m failed\x1b[0m!', error)
      )

      await Farm.find({"ip" : req.body.ip}).then(x => x.forEach(function(y){
        res.send(y._id.toString());
      }))
    }
  }); 
});

async function loadFarms(){
  await Farm.find({}).then(x => x.forEach(function(y){
    var pushData = true;
    for(var i = 0; i<chartData.length; i++){
      if(chartData[i].id == y._id.toString()){
        pushData = false;
      }
    }
    if(pushData){chartData.push({ id: y._id.toString(), data: [] });}

    app.get(`/farm/${y._id}`, async (req, res) => {
      var farms = [];

      const isOnline = await isIpOnline(y.ip);
      if(isOnline){
        data = await (await fetch("http://" + y.ip + ":80/")).json();
      }
      else{
        data = {"soilHum1": "N/A", "soilHum2": "N/A", "airHum": "N/A", "airTemp": "N/A", "light":"N/A"}
      }

      await Farm.find({}).then(x => x.forEach(function(y){
        farms.push(y);
      }))
      .then()
        await Farm.find({}).then(x => x.forEach(function(z){
          y = z;
        })).then()
          res.render('farm', {id:y._id.toString(), isOnline:isOnline, farms:farms, ip:y.ip, camera_ip:y.camera_ip, farm_name:y.name, data: data, name: y.name, ssid: y.ssid, password: y.password, plantType: y.plantType, soilHum: y.soilHum, airTemp: y.airTemp, airHum: y.airHum, light: y.light});
    });

    app.post(`/farm/${y._id}/update`, async (req, res) => {
      await Farm.findOne({_id : y._id}).then(async function(z){
        var farmName = z.farmName;
        var plantType = z.plantType;
        var soilHum = z.soilHum;
        var airTemp = z.airTemp;
        var airHum = z.airHum;
        var light = z.light;

        if(req.body.farmName){farmName=req.body.farmName};
        if(req.body.plantType){plantType=req.body.plantType};
        if(req.body.soilHum){soilHum=req.body.soilHum};
        if(req.body.airTemp){airTemp=req.body.airTemp};
        if(req.body.airHum){airHum=req.body.airHum};
        if(req.body.light){light=req.body.light};

        recentActivity.push('Farm with IP <strong>' + z.ip + "</strong> was updated" );

        await Farm.findOneAndUpdate({_id: z._id}, {name:farmName, plantType: plantType, soilHum: soilHum, airTemp: airTemp, airHum:airHum, light:light}).then(updated_farm=>{
          console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mFarm with IP \x1b[90m' + z.ip + ' \x1b[0mwas updated\x1b[32m successfully\x1b[0m!', updated_farm);
        });

        await fetch('http://' + req.body.ip + ':80/update')
        .then(
          console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + req.body.ip + ' \x1b[0mwas\x1b[32m successfull\x1b[0m!')
        )
        .catch(error => 
          console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[Update request to \x1b[90m' + req.body.ip + ' \x1b[32m failed\x1b[0m!', error)
        )

        res.redirect(`/farm/${y._id}`)
      })
    });

    app.post(`/${y._id}/data`, async (req, res) => {
      var soilHum = y.soilHum;
      var airTemp = y.airTemp;
      var airHum = y.airHum;
      var light = y.light;

      res.send("soilHum:"+soilHum.toString() + ",airTemp:" + airTemp.toString() + ",airHum:" + airHum.toString() + ",light:" + light.toString());
    });
  }));
}

async function collectData(){
  Farm.find({}).then(x => x.forEach(async function(y){
    chartData.forEach(async function(dataPoint){
      if(dataPoint.name = y.id.toString()){
        const isOnline = await isIpOnline(y.ip);
        console.log(y.ip)
        if(isOnline){
          try{
            dataPoint.data.push((await (await fetch("http://" + y.ip + ":80/")).json()));
            console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mNew data collected!', dataPoint);
          }
          catch{}
        }
        else{
          dataPoint.data.push({"soilHum1": 0, 'soilHum2': 0, 'airTemp': 0, 'airHum':0, 'light': 0});
          console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mFarm with IP \x1b[90m' + y.ip + ' \x1b[0mis\x1b[31m down\x1b[0m!');
        }
      }
    });
  }));
}

loadFarms();
collectData();

setInterval(collectData, 60 * 60 * 1000);

app.listen(PORT, () => console.log('\x1b[37m[' + new Date().toISOString() + '] ' + '\x1b[0mServer listening on port \x1b[33m'+ PORT + '\x1b[0m!'));