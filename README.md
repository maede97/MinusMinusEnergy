# MinusMinusEnergy
#### A BETH2019 project
Further Information is available here:

**[Project Report](#)**

**[Pitch Presentation](#)**

**[Demo Application](#)**

**[Github wiki](https://github.com/maede97/MinusMinusEnergy/wiki/Client)**

# The Team
Matthias Busenhart

Philip Wiese

Marie-Louise Achart

Yannick Niedermayr

Manuel Winkler

# Feature demo

### Web interface that allows the user to select the amount of tokens he wants to pay and how much should go to the fond:
![billPayment](doc/demo/billPayment.gif)

# Using the project
The project has the following dependencies, make sure you have them all installed:
- Make
- Cmake 3.1
- A C++ compiler such as gcc or clang
- libsqlite3-dev
- sqlite3
- nodeJS
- web3js
- Python 3

On a Raspberry Pi, run the following commands:
```sh
$ git clone https://github.com/maede97/MinusMinusEnergy.git
$ cd MinusMinusEnergy
$ ./install.sh
```

# Compiling the presentation & report
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
