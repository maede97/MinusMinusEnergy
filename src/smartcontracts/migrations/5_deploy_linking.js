const MMETokenContract = artifacts.require("MMEToken");
const BillContract = artifacts.require("Bill");
const FondContract = artifacts.require("Fond");

module.exports = async function(deployer) {
  const BillContractInstance = await BillContract.deployed();
  const MMETokenInstance = await MMETokenContract.deployed();

  MMETokenInstance.setBillContract(BillContract.address);
  MMETokenInstance.setFondContract(FondContract.address);
  BillContractInstance.setFondContract(FondContract.address);
  BillContractInstance.setMMETokenContract(MMETokenInstance.address);
  BillContractInstance.setMMEExchangeRate(100);

};
