const MMETokenContract = artifacts.require("MMEToken")
const BillContract = artifacts.require("Bill");
const FondContract = artifacts.require("Fond");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address
  deployer
    .deploy(MMETokenContract)
    .then(function(MMETokenInstance) {
    deployer
      .deploy(FondContract)
      .then(function(FondContractInstance) {
        deployer
          .deploy(BillContract, MMETokenContract.address, FondContract.address)
          .then(function(BillContractInstance) {
            MMETokenInstance.setBillContract(BillContractInstance.address);
            MMETokenInstance.setFondContract(FondContractInstance.address);
        });
    });
  })
};
