#!/usr/bin/env python3
import random
import sqlite3
from sqlite3 import Error
import numpy as np

def create_connection(db_file):
    """ create a database connection to the SQLite database
        specified by db_file
    :param db_file: database file
    :return: Connection object or None
    """
    try:
        conn = sqlite3.connect(db_file)
        return conn
    except Error as e:
        print(e)

    return None

def create_datum(conn, datum):
    """
    Create a new data point into the data table
    :param conn:
    :param datapoint:
    """
    sql = ''' INSERT INTO sensor_data(id,time,data)
              VALUES(?,?,?) '''
    cur = conn.cursor()
    cur.execute(sql, datum)

def main():
    """
    Fills bulk data in a sine wave form into the database throughout the entirety of 2019.
    """
    database = "./database.db"

    # create a database connection
    conn = create_connection(database)
    with conn:
        monthDays = [31,28,31,30,31,30,31,31,30,31,30,31]
        last = 0
        for m in range(12):
            temp = last
            for i in range (monthDays[m]*1440):
                minute = str(i % 60)
                hour = str((i // 60) % 24)
                day = str(i // 1440 + 1)
                month = str(m+1)
                if len(month) == 1:
                    month = '0' + month
                if len(day) == 1:
                    day = '0' + day
                if len(hour) == 1:
                    hour = '0' + hour
                if len(minute) == 1:
                    minute = '0' + minute
                timestamp = '2019-' + month + '-' + day + 'T' + hour + ':' + minute + ':00'
                datum = (i+temp,timestamp,0.5*np.sin(0.001*(i+temp))+0.85)
                create_datum(conn,datum)
            last = temp + monthDays[m]*1440

if __name__ == '__main__':
    main()
