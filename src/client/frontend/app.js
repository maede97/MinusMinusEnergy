const config = require('./config.json');

var express = require('express');
const Web3 = require('web3');
var util = require('ethereumjs-util');
var tx = require('ethereumjs-tx');
var contract = require('truffle-contract');
const sqlite3 = require('sqlite3').verbose();

var HAS_BILL;

var provider = new Web3.providers.HttpProvider("http://localhost:7545");


if(config.test_env){
  var web3 = new Web3(provider);
  console.log("USING LOCAL NETWORK");
} else {
  var web3 = new Web3(new Web3.providers.HttpProvider("https://ropsten.infura.io/v3/1de4b23aea044238ab6c8500d2420f87"));
  console.log("USING ROPSTEN NETWORK");
}

var MMETokenArtifact = require('./tokens/MMEToken');

var contracts = {};

// load MMEToken contract
contracts.MMEToken = contract(MMETokenArtifact);
// Set the provider for our contract.
contracts.MMEToken.setProvider(provider);

var privateKey = new Buffer(config.privatekey, 'hex');

/*
// Example for ETH-Transaction
web3.eth.getTransactionCount(config.address).then(_nonce => {
    nonce = _nonce.toString(16);
    var txParams = {
      nonce: '0x'+nonce,
      gasPrice: '0x1312D00',
      gasLimit: '0x52080',
      to: '0x6f687DF4B652d7f2A69B4500A478E830d096EebA',
      value: '0xDE0B6B3A7640000',
      data: '0x0'
    }

  var transaction = new tx(txParams);
  transaction.sign(privateKey);

  var serializedTx = transaction.serialize().toString('hex');

  web3.eth.sendSignedTransaction('0x'+serializedTx).on('receipt',console.log);
});
*/
async function checkIfBill() {
  var MMETokenInstance = await contracts.MMEToken.deployed();
  return await MMETokenInstance.hasBill(config.address);
}

async function getBillAmount() {
  var MMETokenInstance = await contracts.MMEToken.deployed();
  return await MMETokenInstance.getAmount(config.address);
}
async function getTokenAmount() {
  var MMETokenInstance = await contracts.MMEToken.deployed();
  return await MMETokenInstance.getTokenAmount(config.address);
}

async function payBill(tokensToFond) {
  var MMETokenInstance = await contracts.MMEToken.deployed();
  return await MMETokenInstance.payBill(config.address, tokensToFond);
}

async function getBillData() {
  // TODO
  // read contract function (getData or so) with argument config.address
  return {
    energy_needed: await getBillAmount(), // or pay_amount or so
    tokens_earned: await getTokenAmount(),
    price_per_unit: 10
  };
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
      getBillData().then(function(data) {
        res.render('bill',data);
      });

    } else {
      res.render('no_bill');
    }
  });
});

// wait for call when users pays
app.route('/pay').get(function(req, res){
  var params = req.query;
  var toFond = params['fond'];
  payBill(toFond).then(function(result){
    console.log(result);
    res.send("Bill payed");
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
