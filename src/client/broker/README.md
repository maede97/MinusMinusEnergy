# Databroker for sensor

### Compile datahandler
```sh
$ cmake .
$ make
```

### Usage
**Generate Fake Data**
```sh
$ python3 fillBulkData.py
```
**Generate Data from Sensor**
(This only works on RPI3+)
```sh
$ python3 sensorDataHandler.py
```

**Claim Token**
Datahander creates signed transaction from sensor values and sends them to the MMEToken contract.  
The address and publickey are fetched from *../webserver/tokens/MMEToken.json*
```sh
$ ./datahandler
```
