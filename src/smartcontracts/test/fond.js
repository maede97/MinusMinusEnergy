const Fond = artifacts.require("Fond");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");
const Web3 = require("web3");
const web3 = new Web3("");



contract('Fond', (accounts) => {
  it('Recieve Token', async () => {
    const sticksInstance = await Fond.deployed();
  });
});
