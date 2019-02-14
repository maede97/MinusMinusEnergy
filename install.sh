#!/usr/bin/env bash

# create sensor database
sqlite3 src/client/database.db "CREATE TABLE 'sensor_data' (id INTEGER, time TIMESTAMP UNIQUE NOT NULL, data INTEGER NOT NULL, PRIMARY KEY(id));"

# compile back-end data handler
mkdir src/client/backend/build
cmake -Hsrc/client/backend -Bsrc/client/backend/build >/dev/null
make -C src/client/backend/build >/dev/null

mv src/client/backend/build/datahandler src/client/backend/
rm -rf src/client/backend/build

# initialize frontend server
cd src/client/frontend
npm install
npm run dev

# create the cronjobs
dir=$(pwd)
#write out current crontab
crontab -l > newcron
crontab newcron
#echo new cron into cron file
(crontab -l && echo -e "*/2 * * * * ${dir}/src/client/sensorDataHandler.py \n 0 * * * * ${dir}/src/client/backend/datahandler") > newcron
#install new cron file
crontab newcron
rm newcron
