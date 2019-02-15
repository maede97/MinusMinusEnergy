# MinusMinusEnergy

### A BETH2019 project for the SwissEnergy Challenge

![Full Demo](doc/demo/fullDemo.gif)
#### Web interface that allows the user to select the amount of tokens he wants to pay and how much should go to the fond

#### Demo ####

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

* **[Project Report](#)**
* **[Pitch Presentation](https://github.com/maede97/MinusMinusEnergy/blob/master/doc/presentation/presentationMinusMinusEnergy.pdf)**
* **[Github wiki](https://github.com/maede97/MinusMinusEnergy/wiki/Client)**

# The Team
**Matthias Busenhart**  
*Webserver (JS), Blockchain (Solidity)* - [maede97](https://github.com/maede97)  

**Philip Wiese**  
*Webserver (JS), Blockchain (Solidity)* - [Xeratec](https://github.com/Xeratec)  

**Marie-Louise Achart**  
*Backend (Python), Documentation (LaTeX)* - [MarieLouiseAchart](https://github.com/MarieLouiseAchart)

**Yannick Niedermayr**  
*Database (C++), Misc.* - [guetzli32](https://github.com/guetzli32)  

**Manuel Winkler**  
*Database (C++)* - [manuel5975p](https://github.com/manuel5975p)  

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
$ pdflatex mmenergy_presentation.tex
```
### Report
Compile the final report with:
```sh
$ cd /doc/report
$ pdflatex mmenergy_report.tex
```

# ToDo
* Security improvement
* Voting system for Fond
* Bug fixing
