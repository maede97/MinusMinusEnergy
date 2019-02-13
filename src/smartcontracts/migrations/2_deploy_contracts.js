var FondContract = artifacts.require("Fond");

module.exports = function(deployer) {
  // Deploy the METoken contract as our only task
  deployer.deploy(FondContract);
};
