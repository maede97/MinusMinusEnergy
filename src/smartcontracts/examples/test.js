const HDWalletProvider = require('truffle-hdwallet-provider');
const Web3 = require('web3');

const walletMnemonic = 'extra topple scout old light cherry ginger either coast grape spoon purchase'; // Your mnemonic
const walletAPIUrl = 'http://localhost:7545'; // Your Infura URL

const provider = new HDWalletProvider(
    walletMnemonic,
    walletAPIUrl
);

const web3 = new Web3(provider);

const deploy = async () => {
    const accounts = await web3.eth.getAccounts();
    console.log(accounts);
    // Do stuff
}
