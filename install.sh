#!/usr/bin/env bash

bepath=src/client/broker

#setup database
sqlite3 src/client/database.db "CREATE TABLE 'sensor_data' (id INTEGER, time TIMESTAMP UNIQUE NOT NULL, data NUMERIC NOT NULL, PRIMARY KEY(id));"

# select database mode
PS3='Do you want to use an empty or pre-filled database? '
options=("1: empty database" "2: pre-generated data")
select opt in "${options[@]}"
do
    case $opt in
        "1: empty database")
            echo "The install will use a new, empty database."
			sqlite3 src/client/database.db "DELETE FROM sensor_data;"
			break
            ;;
        "2: pre-generated data")
            echo "The install will use pre-generated data."
			cd ${bepath}
			./fillBulkData.py
			cd ../../../
			break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done

# compile back-end data handler
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

# setup server
npm install
