# MinusMinusEnergy
#### A BETH2019 project
Further Information is available here:

**[Project Report](#)**

**[Pitch Presentation](#)**

**[Demo Application](#)**


# The Team
Matthias Busenhart

Philip Wiese

Marie-Louise Achart

Yannick Niedermayr

Manuel Winkler

# Components
### Client
#### Sensor
The sensor is the component that is collecting the data relevant to the entire project. We envision this to be an electricity measurement device of some sort, but currently we are using a light sensor for simplicity.
#### Back-end (Arduino Nano)
The Arduino nano is hooked up directly to the sensor and converts the sensor data from analog to digital in order to process it with the Raspberry Pi back-end.
##### Back-end (Raspberry Pi 3+)
The back-end is written in C++/JavaScript and designed to run on a Raspberry Pi or a similar device.
Currently the back-end is responsible for:
- collecting the digital sensor data
- writing the sensor data into a SQLite3 database
- signing the sensor data
- writing the sensor data onto the Ethereum Ropsten test blockchain.

These actions are triggered via various cronjobs.

#### Front-end (JavaScript)
The front-end is a web interface written in JavaScript that is used by the consumer to interact with the system.
It currently has the following features:
- Displaying a consumer's energy usage over a selectable time period
- Notifying a customer of a new energy invoice
  - The invoice screen features a payment tool (Ethereum integrated)
  - Selection tool: How much of the saved money does the consumer want to give to the reward fond contract?

Installing the front-end server:
```sh
$ npm install
```
Running the front-end server:
```sh
$ npm run dev
```

### Blockchain interactions
There are a total of three smart contracts that are used by the system.

#### Token Bank
This smart contract is responsible to hand out tokens to a consumer upon receiving sensor data at the end of a billing cycle.
Depending on how much energy has been saved by the consumer, more tokens will be available to the consumer.

#### Fond Contract
#### Billing Contract

# TODOs
- Implement a voting system that takes into account how many tokens a certain consumer has contributed to the total amount.
    - More contributions = more voting weight
    - some form of entry mask to bring new project ideas to the table
- build system to simplify installation process
