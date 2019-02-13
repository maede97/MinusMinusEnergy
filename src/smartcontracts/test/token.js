const Fond = artifacts.require("Fond");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");

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

  it('Claim Token', async () => {
    const MMETokenInstance = await MMEToken.deployed();
    const amount = 100;
    const nonce = 0;
    const signature = await signClaim(amount, 0);

    MMETokenInstance.claimToken(
      web3.eth.defaultAccount,
      amount,
      0,
      signature
    ).then(res => console.log(res.logs[0].args))

  });
});
