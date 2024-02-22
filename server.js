const express = require('express');

const app = express();
app.set('view engine', 'ejs');

const n_farms = 0;

app.get('/', (req, res) => {
  res.render('index', {
    n_farms : n_farms
  });
});

app.post('/add-farm', (req, res) => {
  res.send('Farm added!');
});

app.listen(4000, () => console.log('Server listening on port 4000!'));