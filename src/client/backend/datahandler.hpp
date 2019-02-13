#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

/* PRE:  Data on energy consumption sent by sensor
 * POST: Data stored internally
 */
bool readData();

// POST: Data signed using public key
bool signData();

// POST: Data stored in SQLite DB for eternity
bool storeData();

// POST: Data sent to token handler smart contract
bool sendData();

#endif /* DATAHANDLER_HPP */
