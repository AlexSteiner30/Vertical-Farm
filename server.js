const express = require('express');
const mongoose = require('mongoose');
const ping = require('ping');
const bodyParser = require('body-parser');

const URI = "mongodb+srv://alexsteiner:T8ILgFlQk6rVDT72@cluster.sr4kalk.mongodb.net/";
var chartData = [];
var recentActivity = [];
const app = express();
app.use(bodyParser.urlencoded({ extended: true }));

app.use(express.json());
app.set('view engine', 'ejs');
app.use(express.static('public'))

mongoose.connect(URI);

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
            recentActivity.push("Farm with IP " + farm.ip + " disconnected")
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
        soilHum1Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum1).concat(Array(6 - chartData[i].data.map(entry => entry.soilHum1).length).fill(0)).reverse()});
        soilHum2Values.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.soilHum2).concat(Array(6 - chartData[i].data.map(entry => entry.soilHum2).length).fill(0)).reverse()});
        airTempValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airTemp).concat(Array(6 - chartData[i].data.map(entry => entry.airTempValues).length).fill(0)).reverse()});
        airHumValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.airHum).concat(Array(6 - chartData[i].data.map(entry => entry.airHumValues).length).fill(0)).reverse()});
        lightValues.push({name: chartData[i].id, data: chartData[i].data.map(entry => entry.light).concat(Array(6 - chartData[i].data.map(entry => entry.lightValues).length).fill(0)).reverse()});
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
        password: req.body.password, 
        plantType: "Potatoes", 
        soilHum: 50, 
        airTemp: 24, 
        airHum: 399, 
        light:139
      });
    
      await farm.save();

      recentActivity.push("A new farm with IP " + req.body.ip + " was added")
      await loadFarms();
      console.log("Farm Was Added");
      await fetch('http://' + req.body.ip + '/update?' + new URLSearchParams({"idealSoilHum": farm.soilHum, "idealAirTemp": farm.airTemp, "idealAirHum": farm.airHum, "idealLight": farm.light}));
      res.send('Okay');
    }
    else{
      recentActivity.push("Farm with IP " + req.body.ip + " connected")
      console.log("Farm Already Exists");
      await fetch('http://' + req.body.ip + '/update?' + new URLSearchParams({"idealSoilHum": result.soilHum, "idealAirTemp": result.airTemp, "idealAirHum": result.airHum, "ideaLight": res.light}));
      res.json('Okay');
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

      data = await (await fetch("http://" + y.ip + ":8080/")).json();

      await Farm.find({}).then(x => x.forEach(function(y){
        farms.push(y);
      }))
      .then()
        await Farm.find({}).then(x => x.forEach(function(z){
          y = z;
        })).then()
          res.render('farm', {id:y._id.toString(), farms:farms, ip:y.ip, farm_name:y.name, data: data, name: y.name, ssid: y.ssid, password: y.password, plantType: y.plantType, soilHum: y.soilHum, airTemp: y.airTemp, airHum: y.airHum, light: y.light});
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

        await fetch('http://' + z.ip + '/update?' + new URLSearchParams({"idealSoilHum": soilHum, "idealAirTemp": airTemp, "idealAirHum": airHum, "idealLight": light}));  
        await Farm.findOneAndUpdate({_id: z._id}, {name:farmName, plantType: plantType, soilHum: soilHum, airTemp: airTemp, airHum:airHum, light:light})

        res.redirect(`/farm/${y._id}`)
      })
    });
  }));
}

async function collectData(){
  Farm.find({}).then(x => x.forEach(async function(y){
    chartData.forEach(async function(dataPoint){
      if(dataPoint.name = y.id.toString()){
        dataPoint.data.push((await (await fetch("http://" + y.ip + ":8080/")).json()));
      }
    });
  }));
}

loadFarms();
collectData();

setInterval(collectData, 60 * 60 * 1000);

app.listen(4000, () => console.log('Server listening on port 4000!'));