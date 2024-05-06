const express = require('express');
const mongoose = require('mongoose');

const URI = "mongodb+srv://alexsteiner:T8ILgFlQk6rVDT72@cluster.sr4kalk.mongodb.net/";

const app = express();
app.set('view engine', 'ejs');
app.use(express.static('public'))

mongoose.connect(URI);

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

const farm = new Farm({
  name: "Farm 1", 
  ssid: "H-Farm", 
  ip: "192.168.88.9",
  password: "Password1234", 
  plantType: "Potatoes", 
  soilHum: 50, 
  airTemp: 24, 
  airHum: 399, 
  light:139
});

app.get('/', async (req, res) => {
  var farms = [];

  await Farm.find({}).then(x => x.forEach(function(y){
    farms.push(y);
  }))
  .then()
    res.render('index', {farms: farms});
});

function loadFarms(){
  Farm.find({}).then(x => x.forEach(function(y){
    app.get(`/farm/${y._id}`, async (req, res) => {
      const response = await fetch("http://172.28.16.8:5001/");
      const data = await response.json();

      var farms = [];

      await Farm.find({}).then(x => x.forEach(function(y){
        farms.push(y);
      }))
      .then()
        res.render('farm', {farms:farms, data: data, name: y.name, ssid: y.ssid, password: y.password, plantType: y.plantType, soilHum: y.soilHum, airTemp: y.airTemp, airHum: y.airHum, light: y.light});
    });
    
  }));
}

loadFarms();

app.listen(4000, () => console.log('Server listening on port 4000!'));