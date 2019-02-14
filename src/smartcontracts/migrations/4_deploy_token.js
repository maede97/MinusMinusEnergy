const MMETokenContract = artifacts.require("MMEToken");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address
  deployer.deploy(MMETokenContract);
};
