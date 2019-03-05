const MMETokenContract = artifacts.require("MMEToken");
const BillContract = artifacts.require("Bill");
const FundContract = artifacts.require("Fund");

module.exports = async function(deployer) {
  const BillContractInstance = await BillContract.deployed();
  const MMETokenInstance = await MMETokenContract.deployed();

  MMETokenInstance.setBillContract(BillContract.address);
  MMETokenInstance.setFundContract(FundContract.address);
  BillContractInstance.setFundContract(FundContract.address);
  BillContractInstance.setMMETokenContract(MMETokenInstance.address);
  BillContractInstance.setMMEExchangeRate(100);

};
