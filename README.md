# MinusMinusEnergy

### A BETH2019 project for the SwissEnergy Challenge
MinusMinusEnergy is a blockchain project which was created in February 2019 during the BETH Hackathon at ETH Zurich.

![Full Demo](doc/demo/fullDemo.gif)
#### Web interface that allows the user to select the amount of tokens he wants to pay and how much should go to the fond

# Demo #

**Run Demo**
* Run these commands
```sh
$ git clone https://github.com/maede97/MinusMinusEnergy.git
$ cd MinusMinusEnergy
$ npm install
$ npm test
$ npm start
```
* Or watch the GIF above (shortened, hence the black flicker in the middle)

#### Further Information is available here:

* **[Project Report](doc/report/reportMinusMinusEnergy.pdf)**
* **[Pitch Presentation](doc/presentation/presentationMinusMinusEnergy.pdf)**
* **[Repo wiki](https://github.com/maede97/MinusMinusEnergy/wiki/Client)**
* [SmartContracts (how to use truffle)](src/smartcontracts/README.md)
* [DataBroker](src/client/broker/README.md)

# The Team
**Matthias Busenhart** (ETHZ Bsc RW/CSE)  
*Webserver (JS), Blockchain (Solidity)* - [maede97](https://github.com/maede97)  

**Philip Wiese** (ETHZ Bsc ETIT)  
*Webserver (JS), Blockchain (Solidity)* - [Xeratec](https://github.com/Xeratec)  

**Marie-Louise Achart** (ETHZ Bsc RW/CSE)  
*Broker (Python), Documentation (LaTeX)* - [MarieLouiseAchart](https://github.com/MarieLouiseAchart)

**Yannick Niedermayr** (ETHZ Bsc RW/CSE)  
*Broker (C++, Python), Misc.* - [guetzli32](https://github.com/guetzli32)  

**Manuel Winkler** (ETHZ Bsc RW/CSE)  
*Broker (C++)* - [manuel5975p](https://github.com/manuel5975p)  

# Usage
### Dependencies
The project has the following dependencies, make sure you have them all installed:
- Make
- Cmake 3.1
- A C++ compiler such as gcc or clang
- libsqlite3-dev
- sqlite3
- Python 3
- nodeJS [Link](https://nodejs.org/en/)
- npm [Link](https://www.npmjs.com/)
- Ganache [Link](https://truffleframework.com/ganache)  

**Arch Linux**  
```sh
arch$ sudo pacman -S sqlite3 make cmake python gcc
```

**Ubuntu**  
```bash
ubuntu$ sudo apt install sqlite3 libsqlite3-dev make cmake python3 gcc
```

### Demo
**Description**  
The demo deploys the MMEToken, Bill and Fond contract on a local Ganache instance and adds an energy provider to the bill contract. The energy provider can issue new bills to a specific address.

The demo script also generates a signed transaction with the trusted private key of the sensor to claim MMETokens on the MMEToken contract for reduced energy consumption.

On the webinterface the client can analyze his energy consumption and pay his bills. He can decide to use some of his MMEToken to pay for his bill or invest them into a fond. The money in the fond could be used to finance local clima-pro projects, optionally with a integrated voting system (not implemented).

##### Information
* **Ganache**  
Port: 7545  
Network ID: 5777  
Hostname: 127.0.0.1 - lo  
Number of Accounts: >=3  
Account 1: Contract Owner (subsidize eg. government)  
Account 2: Energy Provider (eg. EWZ)  
Account 3: Client  
* **Webserver**  
URL: http://localhost:3000
* **Metamask**  
Selected Account: Account 3

### Installation
On a Raspberry Pi, run the following commands:
```sh
$ git clone https://github.com/maede97/MinusMinusEnergy.git
$ cd MinusMinusEnergy
$ ./install.sh
```

# Presentation & Report
### Presentation
Compile the presentation with:
```sh
$ cd /doc/presentation
$ pdflatex presentationMinusMinusEnergy.tex
```
### Report
Compile the final report with:
```sh
$ cd /doc/report
$ pdflatex reportMinusMinusEnergy.tex
```

# ToDo
* Security improvement
* Voting system for Fond
* Bug fixing
* Tweak frontend appearance

# Links for developers
**Ethereum**
* [Ethereum Doc](http://ethdocs.org/en/latest/index.html#)
* [Solidity Doc](https://solidity.readthedocs.io/en/v0.5.3/structure-of-a-contract.html#functions)
* [Truffle Doc](https://truffleframework.com/docs/truffle/getting-started/running-migrations)
* [OpenZeppelin Doc](https://docs.openzeppelin.org/docs/token_erc20_erc20#transfer)

**Javascript**
* [Web3.js Doc](https://web3js.readthedocs.io/en/1.0/web3.html)
* [NodeJS Doc](https://nodejs.org/api/)
* [PugJS Doc](https://pugjs.org/api/getting-started.html)
* [Express JS Doc](http://expressjs.com/de/api.html)
