const MMETokenContract = artifacts.require("MMEToken");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FundContract, passing in BillContract address
  deployer.deploy(MMETokenContract);
};
