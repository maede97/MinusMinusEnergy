#!/usr/bin/env bash

# create sensor database
sqlite3 src/client/database.db "CREATE TABLE 'sensor_data' (id INTEGER, time TIMESTAMP UNIQUE NOT NULL, data INTEGER NOT NULL, PRIMARY KEY(id));"

# compile back-end data handler
bepath=src/client/backend
mkdir ${bepath}/build
cmake -H${bepath} -B${bepath}/build >/dev/null
make -C ${bepath}/build >/dev/null

mv ${bepath}/build/datahandler ${bepath}
rm -rf ${bepath}/build

# create the cronjobs
dir=$(pwd)
#write out current crontab
crontab -l > newcron
crontab newcron
#echo new cron into cron file
(crontab -l && echo -e "*/2 * * * * python3 ${dir}/src/client/sensorDataHandler.py\n 0 * * * * ${dir}/src/client/backend/datahandler") > newcron
#install new cron file
crontab newcron
rm newcron

# initialize frontend server
cd src/client/frontend
npm install
node app.js
