const BillContract = artifacts.require("Bill");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FundContract, passing in BillContract address

    deployer.deploy(BillContract);
};
