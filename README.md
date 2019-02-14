# MinusMinusEnergy

### A BETH2019 project for the SwissEnergy Challenge

![Full Demo](doc/demo/fullDemo.gif)
#### Web interface that allows the user to select the amount of tokens he wants to pay and how much should go to the fond

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
- nodeJS
- Python 3
- Ganache

### Demo
To demo run demo
```sh
$ git clone https://github.com/maede97/MinusMinusEnergy.git
$ cd MinusMinusEnergy
$ npm install
$ npm test
$ npm start
```

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
