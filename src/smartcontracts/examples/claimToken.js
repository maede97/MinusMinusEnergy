/*

Example script to claim token for consumer with keypair defined below

change the lines between the EDIT tag

if you claimed some tokens, add 1 to nonce, otherwise your call will be rejected

*/

// IMPORTS
const fs = require('fs');
const Web3 = require("web3");
const config = require("./config.json");
// Local Ganache Provider
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));


// EDIT HERE
var amount = web3.utils.toWei("1","ether");
var nonce = 0;
// STOP EDIT

// SETTINGS
const MMETOKEN_ABI = "./build/contracts/MMEToken.json"


// Read and parse ABI from file
var jsonFile = MMETOKEN_ABI;
var parsed= JSON.parse(fs.readFileSync(jsonFile));
var abi = parsed.abi;

var MMEToken = new web3.eth.Contract(abi, config.MMETokenAddress);
// Enough gas to prevent revert of contract
MMEToken.defaultGas = 6721975;

// Import private key
const defaultAccount = web3.eth.accounts.privateKeyToAccount(config.privatekey);
web3.eth.defaultAccount = defaultAccount.address;

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
