const readline = require("readline-sync");
const process = require("process");
const child = require("child_process");
const fs = require("fs");
const chalk = require('chalk');

process.chdir( __dirname );

function askUser() {
  console.log("");
  readline.question("Please press Enter to continue.\t");
  console.log("");
}

console.log("");
console.log(chalk.blue.bold("=============== Blockchain-Setup for MinusMinusEnergy ================"));
console.log("");
console.log(chalk.blue.bold("Description : Local demo setup script "));
console.log(chalk.blue.bold("Author      : Matthias Busenhart, Philip Wiese,"));
console.log(chalk.blue.bold("            : Marie-Louise Achart, Yannick Niedermayr, Manuel Winkler"));
console.log("");
console.log(chalk.blue.bold("======================================================================"));
console.log("");

console.log(chalk.yellow("(1) Startup"));
console.log("    (Re)start Ganache and open Chrome");
console.log("    In Chrome, go to \"localhost:3000\"");
console.log("        (No connection possible)");

askUser();
console.log(chalk.yellow("(2) Reset MetaMask:"));
console.log("   - Settings->Reset Account");
console.log("   - Switch to another network");
console.log("   - Switch back :-)");
console.log("   - Select \"Account 3\"");
askUser();

console.log(chalk.yellow("(3) Setting up contracts"));
process.chdir("../smartcontracts");
console.log("    Compiling...");
child.execSync("truffle compile", {stdio: 'ignore'});
console.log("    Deploying...");
child.execSync("truffle migrate --reset", {stdio: 'ignore'});
console.log("");
console.log("    Getting Address of BillContract");
var BillABI = require("../smartcontracts/build/contracts/Bill.json");
console.log(chalk.green("    Address: " + BillABI.networks['5777'].address));
console.log("    Getting Address of MMETokenContract");
var MMETokenABI = require("../smartcontracts/build/contracts/MMEToken.json");
console.log(chalk.green("    Address: " + MMETokenABI.networks['5777'].address));
console.log("");

console.log(chalk.yellow("(4) Keys / Address (use Keys from Ganache (eg. Account 3))"));

var address = readline.question(chalk.green("    Please enter your address: (with 0x-prefix):\n")).toString();
var key = readline.question(chalk.green("    Please enter your private key (without 0x-prefix):\n")).toString();
console.log("");
var json_content = {}
json_content["port"] = 3000;
json_content["address"] = address;
json_content["privatekey"] = "0x"+key;
json_content["test_env"] = true;
json_content["BillAddress"] = BillABI.networks['5777'].address;
json_content["MMETokenAddress"] = MMETokenABI.networks['5777'].address;
fs.writeFileSync('examples/config.json',JSON.stringify(json_content, null, "\t"));

console.log(chalk.yellow("(5) Creating Fake Data"));
console.log("    Creating Fake Bill");
try {
  response = child.execSync("truffle exec examples/fakeBill.js", {stdio: 'ignore'});
} catch (err) {
  console.log(chalk.red("   Error occured"));
}
console.log("    Creating Fake Tokens");
try {
  response = child.execSync("node examples/claimToken.js", {stdio: 'ignore'});
} catch (err) {
  console.log(chalk.red("    Error occured"));
}

console.log(chalk.yellow("(6) Writing JSON to webserver"));
process.chdir("../client/webserver/");
child.execSync("cp ../../smartcontracts/build/contracts/Bill.json tokens").toString();
child.execSync("cp ../../smartcontracts/build/contracts/MMEToken.json tokens").toString();
fs.writeFileSync('config.json',JSON.stringify(json_content,null,'\t'), 'utf8',function(){});
console.log("");

console.log(chalk.green("(Finished) Please run now 'npm start'!"));
console.log("    Then, reload \"locahost:3000\" in Chrome");
