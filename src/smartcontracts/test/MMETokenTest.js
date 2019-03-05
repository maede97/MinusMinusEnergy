const Fund = artifacts.require("Fund");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");

const truffleAssert = require('truffle-assertions');

contract('MMEToken', async(accounts) => {
  async function getInstances() {
    MMETokenInstance = await MMEToken.deployed();
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
  getInstances();

  let amount = web3.utils.toWei("1", "wei");
  let nonce = 0;

  it('Claim Token', async () => {
    // Calculate Signature for claim
    const signature = await signClaim(amount, nonce);

    MMETokenInstance.claimToken(
      accounts[2],
      amount,
      nonce,
      signature,
      {from: accounts[2]}
    );

    const balance = await MMETokenInstance.balanceOf(accounts[2]);
    assert.equal(balance, amount, "owner doesn't have the tokens");
  });

  it('Prevent Replay', async () => {
    // Calculate Signature for claim
    const signature = await signClaim(amount, nonce);

    await truffleAssert.fails(
      MMETokenInstance.claimToken(
        accounts[2],
        amount,
        nonce,
        signature,
        {from: accounts[2]}
      ),
      truffleAssert.ErrorType.REVERT,
      null,
      'This method should revert'
    );
  });

  it('Reject tampered data', async () => {
    nonce++;

    // Calculate Signature for claim
    const signature = await signClaim(amount, nonce);

    await truffleAssert.fails(
      MMETokenInstance.claimToken(
        accounts[2],
        amount+100,
        nonce,
        signature,
        {from: accounts[2]}
      ),
      truffleAssert.ErrorType.REVERT,
      null,
      'This method should revert'
    );
  });
});
