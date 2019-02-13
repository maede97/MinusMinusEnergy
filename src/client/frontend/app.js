var express = require('express');
//const Web3 = require('web3');
//const web3 = new Web3(Web3.givenProvider || "ws://localhost:7545");
const sqlite3 = require('sqlite3').verbose();

let db = new sqlite3.Database('../database.db', sqlite3.OPEN_READWRITE, (err) => {
  if (err) {
    console.error(err.message);
  }
  console.log('Connected to the database.');
});

var app = express();

app.use("/css", express.static(__dirname + '/css'));
app.use("/js", express.static(__dirname + '/js'));

app.set('view engine', 'pug');

app.route('/').get(function(req, res) {
  res.render('index', { title: 'Hey', message: 'Hello there!'});
});

app.route('/api').get(function(req, res) {
  var params = req.query;
  var returnData = [];
  db.all('SELECT time,data FROM sensor_data WHERE time<=\''+params.end+'\' AND time>=\''+params.begin+'\'', (err, rows) => {
    if (err) {
      console.error(err.message);
    }
    res.send(rows);
  });
});

app.listen(3000);
