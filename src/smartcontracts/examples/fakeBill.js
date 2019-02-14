var MMEToken = artifacts.require("../contracts/MMEToken.sol");
var Bill = artifacts.require("../contracts/Bill.sol");
module.exports = function(callback) {
  Bill.deployed().then(function(BillInstance){
    MMEToken.deployed().then(function(MMETokenInstance){
      web3.eth.getAccounts().then(function(accounts){
        BillInstance.addEnergyProducer(accounts[1]).then(function(){
          BillInstance.createBill(accounts[2], web3.utils.toWei("10", "ether"), {from: accounts[1]});
        });
      });
    });
  });
}
