const MMETokenContract = artifacts.require("MMEToken")
const BillContract = artifacts.require("Bill");
const FondContract = artifacts.require("Fond");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address
  deployer.deploy(MMETokenContract);
  deployer.deploy(BillContract).then(function() {
      return deployer.deploy(FondContract, BillContract.address);
  });

};
