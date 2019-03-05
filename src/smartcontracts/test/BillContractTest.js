const Fund = artifacts.require("Fund");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");

const truffleAssert = require('truffle-assertions');


contract('Bill Contract', async(accounts) => {
  async function getInstances() {
    MMETokenInstance = await MMEToken.deployed();
    BillInstance = await Bill.deployed();
    FundInstance = await Fund.deployed();
  }
  function signClaim(amount, _nonce) {
    var hash = web3.utils.soliditySha3(
      {
        type: "address",
        value: accounts[2]
      },
      {
        type: "uint256",
        value: amount
      },
      {
        type: "uint256",
        value: _nonce
    }).toString("hex");
    return web3.eth.sign(hash, accounts[2]);
  }

  let MMETokenInstance;
  let BillInstance;
  let FundInstance;
  getInstances();

  const MMEEXCHANGERATE = 100;
  const TOKENAMOUNT = web3.utils.toWei("1", "wei");
  const BILLAMOUNT = web3.utils.toWei("1000","wei");
  let nonce = 0;
  let expectedFundBalance = 0;

  beforeEach(async() => {
    // Claim token
    const signature = await signClaim(TOKENAMOUNT, nonce);
    MMETokenInstance.claimToken(
      accounts[2],
      TOKENAMOUNT,
      nonce,
      signature,
      {from: accounts[2]}
    );

    // Create Bill
    BillInstance.addEnergyProducer(accounts[1], {from: accounts[0]});

    await BillInstance.createBill(
      accounts[2],
      BILLAMOUNT,
      {from: accounts[1]}
    );

    nonce++;
  });

  it('Pay Bill normal (40/60)', async() => {
    const balance = await MMETokenInstance.balanceOf(accounts[2]);
    const bill = await BillInstance.getInvoiceAmount({from: accounts[2]})
    const ratio = 0.4;
    var amount = bill - MMEEXCHANGERATE * ratio * balance;

    const tx = await BillInstance.payBill(ratio * 100, {from: accounts[2], value: amount});

    const tokenBalance = await MMETokenInstance.balanceOf(accounts[2]);
    const hasBill = await BillInstance.hasBill(accounts[2]);
    const fundBalance = await web3.eth.getBalance(FundInstance.address);

    expectedFundBalance += (1-ratio) * MMEEXCHANGERATE * balance;

    assert.equal(tokenBalance, 0, "Token amount should be zero.");
    assert.equal(hasBill, false, "There should be no bill");
    assert.equal(fundBalance, expectedFundBalance, "Not valid amount in fund");
  });

  it('Pay Bill normal (60/40)', async() => {
    const balance = await MMETokenInstance.balanceOf(accounts[2]);
    const bill = await BillInstance.getInvoiceAmount({from: accounts[2]})
    const ratio = 0.6;
    var amount = bill - MMEEXCHANGERATE * ratio * balance;

    const tx = await BillInstance.payBill(ratio * 100, {from: accounts[2], value: amount});

    const tokenBalance = await MMETokenInstance.balanceOf(accounts[2]);
    const hasBill = await BillInstance.hasBill(accounts[2]);
    const fundBalance = await web3.eth.getBalance(FundInstance.address);

    expectedFundBalance += (1-ratio) * MMEEXCHANGERATE * balance;

    assert.equal(tokenBalance, 0, "Token amount should be zero.");
    assert.equal(hasBill, false, "There should be no bill");
    assert.equal(fundBalance, expectedFundBalance, "Not valid amount in fund");

  });

  it('Pay Bill too much', async() => {
    const balance = await MMETokenInstance.balanceOf(accounts[2]);
    const bill = await BillInstance.getInvoiceAmount({from: accounts[2]})
    const ratio = 0.6;
    var amount = bill - MMEEXCHANGERATE * ratio * balance;

    const tx = await BillInstance.payBill(ratio * 100, {from: accounts[2], value: amount+10});

    const tokenBalance = await MMETokenInstance.balanceOf(accounts[2]);
    const hasBill = await BillInstance.hasBill(accounts[2]);
    const fundBalance = await web3.eth.getBalance(FundInstance.address);

    expectedFundBalance += (1-ratio) * MMEEXCHANGERATE * balance;

    assert.equal(tokenBalance, 0, "Token amount should be zero.");
    assert.equal(hasBill, false, "There should be no bill");
    assert.equal(fundBalance, expectedFundBalance, "Not valid amount in fund");
  });

  it('Pay Bill too less', async() => {
    const balance = await MMETokenInstance.balanceOf(accounts[2]);
    const bill = await BillInstance.getInvoiceAmount({from: accounts[2]})
    const ratio = 0.6;
    var amount = bill - MMEEXCHANGERATE * ratio * balance;

    await truffleAssert.fails(
      BillInstance.payBill(ratio * 100, {from: accounts[2], value: amount-10}),
      truffleAssert.ErrorType.REVERT,
      null,
      'This method should revert'
    );
  });
});
