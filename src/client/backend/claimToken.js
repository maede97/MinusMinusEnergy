// IMPORTS
const fs = require('fs');
const Web3 = require("web3");
const config = require("../webserver/config.json");


// SETTING
const PRIVATE_KEY = config.privatekey;
const MMETOKEN_ADDR = config.MMETokenAddress;
const MMETOKEN_ABI = "../webserver/tokens/MMEToken.json"

const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));

// Read and parse ABI from file
var jsonFile = MMETOKEN_ABI;
var parsed= JSON.parse(fs.readFileSync(jsonFile));
var abi = parsed.abi;

var MMEToken = new web3.eth.Contract(abi, MMETOKEN_ADDR);
// Enough gas to prevent revert of contract
MMEToken.defaultGas = 6721975;

// Import private key
const defaultAccount = web3.eth.accounts.privateKeyToAccount(PRIVATE_KEY);
web3.eth.defaultAccount = defaultAccount.address;

var amount = process.argv[2];
var nonce = parseInt(process.argv[3]);

// Calculate Signature for claim
var signature = signClaim(amount, nonce);

// Send claim to contract
MMEToken.methods.claimToken(
  web3.eth.defaultAccount,
  amount,
  nonce,
  signature
).send({from: web3.eth.defaultAccount}, (err,res) => {
  // Show output
  console.log(res);
});

// Signing of data packagets
function signClaim(amount, nonce, callback) {
  var hash = web3.utils.soliditySha3(
    {
      type: "address",
      value: web3.eth.defaultAccount
    },
    {
      type: "uint256",
      value: amount
    },
    {
      type: "uint256",
      value: nonce
  }).toString("hex");

  return defaultAccount.sign(hash).signature;
}
