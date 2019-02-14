const Fond = artifacts.require("Fond");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");

const truffleAssert = require('truffle-assertions');

contract('Token', (accounts) => {
  web3.eth.defaultAccount = accounts[1];

  function signClaim(amount, nonce) {
    var hash = web3.utils.soliditySha3(
      {
        type: "address",
        value: web3.eth.defaultAccount
      },
      {
        type: "uint256",
        value: amount
      },
      {
        type: "uint256",
        value: nonce
    }).toString("hex");
    console.log(hash);
    return web3.eth.sign(hash, web3.eth.defaultAccount);
  }

  it.only('Claim Token', async () => {
    const MMETokenInstance = await MMEToken.at("0x6112C28Ad790Bc7EFd2Df4C573D9574631445fAB");
    const amount = 100;
    const nonce = 0;

    // Calculate Signature for claim
    const signature = await signClaim(amount, nonce);
    console.log(web3.eth.defaultAccount);

    MMETokenInstance.claimToken(
      web3.eth.defaultAccount,
      amount,
      nonce,
      signature
    );

    const balance = await MMETokenInstance.balanceOf(web3.eth.defaultAccount);
    assert.equal(balance, amount, "owner doesn't have the tokens");
  });

  it('Prevent Replay', async () => {
    const MMETokenInstance = await MMEToken.deployed();
    const amount = 100;
    const nonce = 0;

    // Calculate Signature for claim
    const signature = await signClaim(amount, 0);

    await truffleAssert.fails(
      MMETokenInstance.claimToken(
        web3.eth.defaultAccount,
        amount,
        nonce,
        signature
      ),
      truffleAssert.ErrorType.REVERT,
      null,
      'This method should revert'
    );
  });

  it('Reject tampered data', async () => {
    const MMETokenInstance = await MMEToken.deployed();
    const amount = 100;
    const nonce = 1;

    // Calculate Signature for claim
    const signature = await signClaim(amount, 0);

    await truffleAssert.fails(
      MMETokenInstance.claimToken(
        web3.eth.defaultAccount,
        amount+100,
        nonce,
        signature
      ),
      truffleAssert.ErrorType.REVERT,
      null,
      'This method should revert'
    );
  });
});
