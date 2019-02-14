const Fond = artifacts.require("Fond");
const Bill = artifacts.require("Bill");
const MMEToken = artifacts.require("MMEToken");

contract('Fond', (accounts) => {
  it('Recieve Token', async () => {
    const sticksInstance = await Fond.deployed();
  });
});
