const FondContract = artifacts.require("Fond");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address

    deployer.deploy(FondContract);
};
