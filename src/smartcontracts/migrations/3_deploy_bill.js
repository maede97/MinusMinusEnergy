const BillContract = artifacts.require("Bill");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FondContract, passing in BillContract address

    deployer.deploy(BillContract);
};
