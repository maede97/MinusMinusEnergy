const FundContract = artifacts.require("Fund");

module.exports = function(deployer) {
  // Deploy BillContract, then deploy FundContract, passing in BillContract address

    deployer.deploy(FundContract);
};
