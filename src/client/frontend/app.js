const config = require('./config.json');

var express = require('express');
const Web3 = require('web3');
var util = require('ethereumjs-util');
var tx = require('ethereumjs-tx');
var contract = require('truffle-contract');
const sqlite3 = require('sqlite3').verbose();

var provider = null;

if(config.test_env){
  provider = new Web3.providers.HttpProvider("http://localhost:7545");
  var web3 = new Web3(provider);
  console.log("USING LOCAL NETWORK");
} else {
  provider = new Web3.providers.HttpProvider("https://ropsten.infura.io/v3/1de4b23aea044238ab6c8500d2420f87");
  var web3 = new Web3(provider);
  console.log("USING ROPSTEN NETWORK");
}

var MMETokenArtifact = require('./tokens/MMEToken');
var BillArtifact = require('./tokens/Bill');

var contracts = {};

// load MMEToken contract
contracts.MMEToken = contract(MMETokenArtifact);
contracts.Bill = contract(BillArtifact);
// Set the provider for our contract.
contracts.MMEToken.setProvider(provider);
contracts.Bill = contract(provider);

var privateKey = new Buffer(config.privatekey, 'hex');

async function checkIfBill() {
  var BillInstance = await contracts.Bill.deployed();
  return await BillInstance.hasBill(config.address);
}

let db = new sqlite3.Database('../database.db', sqlite3.OPEN_READWRITE, (err) => {
  if (err) {
    console.error(err.message);
  }
  console.log('Connected to the database.');
});

var app = express();

// Serve static files
app.use("/css", express.static(__dirname + '/css'));
app.use("/js", express.static(__dirname + '/js'));
app.use("/tokens", express.static(__dirname + '/tokens'));

// Set template manager to pug
app.set('view engine', 'pug');

// Serve index page (with overview)
app.route('/').get(function(req, res) {
  res.render('index');
});

// Serve page for the new invoice
app.route('/bill').get(function(req, res) {
  checkIfBill().then(function(result){
    if(result) {
        res.render('bill');
    } else {
      res.render('no_bill');
    }
  });
});

// serve data for display
app.route('/api').get(function(req, res) {
  var params = req.query;
  var returnData = [];
  db.all('SELECT time, data FROM sensor_data WHERE time<=\''+params.end+'\' AND time>=\''+params.begin+'\'', (err, rows) => {
    if (err) {
      console.error(err.message);
    }
    console.log("Serving rows: " + rows.length);
    res.send(rows);
  });
});

// listen on port
app.listen(config.port);
