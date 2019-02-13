var express = require('express');
//const Web3 = require('web3');
//const web3 = new Web3(Web3.givenProvider || "ws://localhost:7545");
const sqlite3 = require('sqlite3').verbose();

const Highcharts = require('highcharts');


// let db = new sqlite3.Database('../DB_Template.db', sqlite3.OPEN_READWRITE, (err) => {
//   if (err) {
//     console.error(err.message);
//   }
//   console.log('Connected to the database.');
// });

var app = express();

app.use("/css", express.static(__dirname + '/css'));
app.use("/js", express.static(__dirname + '/js'));

app.set('view engine', 'pug');

app.route('/').get(function(req, res) {
  res.render('index', { title: 'Hey', message: 'Hello there!',s_data:'var CHART_CATEGORIES = [\'Jan\'];var CHART_DATA = [{name:\'\',data:[2]}];'});
});

app.listen(3000);
//
// app.route('/Node').get(function(req,res){
//   res.send("Hi there");
// });

// db.serialize(() => {
//   db.each(`SELECT PlaylistId as id,
//                   Name as name
//            FROM playlists`, (err, row) => {
//     if (err) {
//       console.error(err.message);
//     }
//     console.log(row.id + "\t" + row.name);
//   });
// });
//
// db.close((err) => {
//   if (err) {
//     console.error(err.message);
//   }
//   console.log('Close the database connection.');
// });
