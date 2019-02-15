# Solidity Smart Contracts

### Tests
#### Ganache has to be already running
**Port:** 7545  
**Network ID:** 5777  
**Hostname:** 127.0.0.1 - lo  
**Number of Accounts:** >=3  
**Account 1:** Contract Owner (subsidizer eg. government)  
**Account 2:** Energy Provider (eg. EWZ)  
**Account 3:** Client  

```sh
$ truffle test
```

### Interaction with Contracts
#### Ganache has to be already running
```sh
$ truffle console
truffle(development)> compile
truffle(development)> migrate --reset

## Get accounts on network
truffle(development)> accounts = await web3.eth.getAccounts()
# Get deployed Contracts
truffle(development)> MMETokenInstance = await MMEToken.deployed()
truffle(development)> BillInstance = await Bill.deployed()
truffle(development)> FondInstance = await Fond.deployed()

# Get Token balance of account[2]
truffle(development)> (await MMETokenInstance.balanceOf(accounts[2])).toString()
'0'

# Add Energy Producer
BillInstance.addEnergyProducer(accounts[1])

# Check for Bill
truffle(development)> await BillInstance.hasBill(accounts[2])
false

# Add Bill
truffle(development)> BillInstance.createBill(accounts[2], web3.utils.toWei("10","ether"), {from: accounts[1]})
truffle(development)> await BillInstance.hasBill(accounts[2])
true

# Check for Bill amount
truffle(development)> (await BillInstance.getInvoiceAmount({from: accounts[2]})).toString()
'10000000000000000000'

truffle(development)> BillInstance.payBill(0, {from: accounts[2]})
truffle(development)> await BillInstance.hasBill(accounts[2])
false
```
##### Claim Tokens
To successfully claim tokens you have to adjust **config.json** in ***src/smartcontracts/examples*** and then run:
```sh
$ node examples/claimToken.js
```
If you receive an *VM Exception while processing transaction: revert* message the signature of the data is probably wrong or used two time the same nonce.
