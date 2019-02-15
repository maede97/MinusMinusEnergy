#!/usr/bin/env bash

# create sensor database
sqlite3 src/client/database.db "CREATE TABLE 'sensor_data' (id INTEGER, time TIMESTAMP UNIQUE NOT NULL, data NUMERIC NOT NULL, PRIMARY KEY(id));"

# compile back-end data handler
bepath=src/client/broker
mkdir ${bepath}/build
cmake -H${bepath} -B${bepath}/build
make -C ${bepath}/build

mv ${bepath}/build/datahandler ${bepath}
rm -rf ${bepath}/build

# create the cronjobs
dir=$(pwd)
#write out current crontab
crontab -l > newcron
crontab newcron
#echo new cron into cron file
(crontab -l && echo -e "*/2 * * * * python3 ${dir}/${bepath}/sensorDataHandler.py\n 0 * * * * ${dir}/${bepath}/datahandler") > newcron
#install new cron file
crontab newcron
rm newcron

# initialize webserver server
cd src/client/webserver
npm install
