const MMETokenContract = artifacts.require("MMEToken")
const BillContract = artifacts.require("Bill");
const FondContract = artifacts.require("Fond");

module.exports = async function(deployer) {
  MMETokenInstance = await MMETokenContract.deployed()
  MMETokenInstance.setBillContract(BillContract.address);
  MMETokenInstance.setFondContract(FondContract.address);
}
