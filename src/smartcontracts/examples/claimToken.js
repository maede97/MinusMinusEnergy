// IMPORTS
const fs = require('fs');
const Web3 = require("web3");
var HDWalletProvider = require("truffle-hdwallet-provider");

// SETTINGS
const PRIVATE_KEY = "0x5b48a68bed1b56ef47ad84f52d37c3d3f48f38a6ddedbf8252dfee85f3d81030";
const MMETOKEN_ADDR = "0x6112C28Ad790Bc7EFd2Df4C573D9574631445fAB"
const MMETOKEN_ABI = "../build/contracts/MMEToken.json"

const MNEMONIC = "extra topple scout old light cherry ginger either coast grape spoon purchase";



// Local Ganache Provider
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));
// Ropsten testnet
// var ropsten = new HDWalletProvider(MNEMONIC, "https://ropsten.infura.io/v3/1de4b23aea044238ab6c8500d2420f87");
// const web3 = new Web3(ropsten);

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

var amount = 100;
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
