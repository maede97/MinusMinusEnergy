const readline = require("readline-sync");
const process = require("process");
const child = require("child_process");
const fs = require("fs");

function askUser() {
  console.log("");
  readline.question("Please press Enter to continue.\t");
  console.log("");
}

console.log("Blockchain-Setup for MinusMinusEnergy");
console.log("=====================================");
console.log("");
console.log("(1) Restart Ganache");

askUser();
console.log("(2) Please reset MetaMask:");
console.log("\tSettings->Reset Account");
console.log("\tSwitch to another network");
console.log("\tSwitch back");
console.log("\tSelect \"Account 3\"");
askUser();
console.log("(3) Setting up your contracts");
process.chdir("../smartcontracts");
console.log("(3.1) Compiling...")
console.log(child.execSync("truffle compile").toString());
console.log("(3.2) Deploying...");
console.log(child.execSync("truffle migrate --reset").toString());
console.log("");
console.log("(3.3) Getting Address of BillContract");
var BillABI = require("../smartcontracts/build/contracts/Bill.json");
console.log("\tAddress: " + BillABI.networks['5777'].address);
console.log("(3.4) Getting Address of MMETokenContract");
var MMETokenABI = require("../smartcontracts/build/contracts/MMEToken.json");
console.log("\tAddress: " + MMETokenABI.networks['5777'].address);
console.log("");
console.log("(4) Keys/Address");
var address = readline.question("Please enter your address: ").toString();
var key = readline.question("Please enter your private key: ").toString();
console.log("");
var json_content = {}
json_content["port"] = 3000;
json_content["address"] = address;
json_content["privatekey"] = "0x"+key;
json_content["test_env"] = true;
json_content["BillAddress"] = BillABI.networks['5777'].address;
json_content["MMETokenAddress"] = MMETokenABI.networks['5777'].address;
fs.writeFileSync('examples/config.json',JSON.stringify(json_content, null, "\t"));
console.log("(5) Creating Fake Data");
console.log("(5.1) Creating Fake Bill")
console.log(child.execSync("truffle exec examples/fakeBill.js"));
console.log("(5.2) Creating Fake Tokens");
console.log(child.execSync("node examples/claimToken.js"));
askUser();
console.log("(6) Writing JSON to webserver")
process.chdir("../client/webserver/");
child.execSync("cp ../../smartcontracts/build/contracts/Bill.json tokens").toString();
child.execSync("cp ../../smartcontracts/build/contracts/MMEToken.json tokens").toString();
fs.writeFileSync('config.json',JSON.stringify(json_content,null,'\t'), 'utf8',function(){});
console.log("");
console.log("(7) Starting Node server");
child.execSync("node app.js").toString();
