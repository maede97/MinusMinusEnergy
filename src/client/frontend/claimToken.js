// IMPORTS
const fs = require('fs');
const Web3 = require("web3");
const config = require('./config.json');

// SETTINGS
const MMETOKEN_ABI = "./tokens/MMEToken.json"

// Local Ganache Provider
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));
// Ropsten testnet
// var ropsten = new HDWalletProvider(MNEMONIC, "https://ropsten.infura.io/v3/1de4b23aea044238ab6c8500d2420f87");
// const web3 = new Web3(ropsten);

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

console.log(web3.eth.defaultAccount);

var amount = web3.utils.toWei("1","ether");
var nonce = 0;

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
