
module.exports = async function() {

  var BillInstance = await Bill.deployed()
  var MMETokenInstance = await MMEToken.deployed()
  var FundInstance = await Fund.deployed()

  var accounts = await web3.eth.getAccounts()

  BillInstance.addEnergyProducer(accounts[1])
  BillInstance.createBill(accounts[2], web3.utils.toWei("10", "ether"), {from: accounts[1]})
}
