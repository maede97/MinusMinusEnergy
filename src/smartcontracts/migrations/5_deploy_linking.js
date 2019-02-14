const MMETokenContract = artifacts.require("MMEToken");
const BillContract = artifacts.require("Bill");
const FondContract = artifacts.require("Fond");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address
  MMETokenContract.deployed()
    .then(function(MMETokenInstance) {
    FondContract.deployed()
      .then(function(FondContractInstance) {
        BillContract.deployed()
          .then(function(BillContractInstance) {
            MMETokenInstance.setBillContract(BillContractInstance.address);
            MMETokenInstance.setFondContract(FondContractInstance.address);
            BillContract.setFondContract(FondContractInstance.address);
            BillContract.setMMETokenContract(MMETokenInstance.address);
        });
    });
  })
};
