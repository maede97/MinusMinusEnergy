
module.exports = async function() {
  
  var BillInstance = await Bill.deployed()
  var MMETokenInstance = await MMEToken.deployed()
  var FondInstance = await Fond.deployed()

  var accounts = await web3.eth.getAccounts()

  BillInstance.addEnergyProducer(accounts[1])
  BillInstance.createBill(accounts[2], web3.utils.toWei("10", "ether"), {from: accounts[1]})

   #debug:
   BillInstance.hasBill(accounts[1]) // false
   BillInstance.hasBill(accounts[2]) // true
   BillInstance.getInvoiceAmount({from: accounts[2]}) // 10 ether

  check Bill Page --> invoice exists! (0 tokens)
}
