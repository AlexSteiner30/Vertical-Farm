const express = require('express');

const app = express();
app.set('view engine', 'ejs');
app.use(express.static('public'))

const n_farms = 0;

app.get('/', (req, res) => {
  res.render('index');
});

app.get('/farm', (req, res) => {
  res.render('farm');
});

app.listen(4000, () => console.log('Server listening on port 4000!'));