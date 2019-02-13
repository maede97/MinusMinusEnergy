var express = require('express');
const Web3 = require('web3');
var util = require('ethereumjs-util');
var tx = require('ethereumjs-tx');

const config = require('./config.json');

var web3 = new Web3(new Web3.providers.HttpProvider("https://ropsten.infura.io/v3/1de4b23aea044238ab6c8500d2420f87"));

var address = config.address;
var key = config.privatekey;
var privateKey = new Buffer(key, 'hex');

web3.eth.getTransactionCount(address).then(_nonce => {
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
  //web3.eth.sendSignedTransaction('0x'+serializedTx).on('receipt',console.log);
});
const sqlite3 = require('sqlite3').verbose();

function hasNewBill() {
  // TODO: do this
  return true;
}

function getBillData() {
  // TODO
  return {
    energy_needed: 1234,
    tokens_earned: 10,
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

app.use("/css", express.static(__dirname + '/css'));
app.use("/js", express.static(__dirname + '/js'));

app.set('view engine', 'pug');

app.route('/').get(function(req, res) {
  res.render('index');
});

app.route('/bill').get(function(req, res) {
  if(hasNewBill()) {
    res.render('bill',getBillData());
  } else {
    res.render('no_bill');
  }
})

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

app.listen(3000);
