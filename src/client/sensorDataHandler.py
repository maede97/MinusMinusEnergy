#!/usr/bin/env python3
# REQUIRES PYTHON >= 3.0 for integer division

import random
import sqlite3
from sqlite3 import Error
import RPi.GPIO as GPIO #import the GPIO package thus we can communicate with the GPIO pins.
import time #import the time package to put the script to sleep when needed.GPIO.setmode(GPIO.BOARD) #all the numbering we use in this script will refer to the physical numbering of the pins.#define the pin that goes to the circuit
from datetime import datetime


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
    :param datum:
    """
    sql = ''' INSERT INTO sensor_data(time,data)
              VALUES(?,?) '''
    cur = conn.cursor()
    cur.execute(sql, datum)

def rc_time (pin_to_circuit, delay):
    """
    Returns measured sensor data. Currently a light sensor.
    :param pin_to_circuit:
    :param delay: Number of seconds between measurements
    :return: measurement
    """
    count = 0    #Output on the pin for
    GPIO.setup(pin_to_circuit, GPIO.OUT)
    GPIO.output(pin_to_circuit, GPIO.LOW)
    time.sleep(delay) #the script sleep for 10 secondes    #Change the pin back to input
    GPIO.setup(pin_to_circuit, GPIO.IN)    #Count until the pin goes high
    while (GPIO.input(pin_to_circuit) == GPIO.LOW):
        count += 1
    return count#Catch when script is interupted, cleanup correctly

def avg(iter,delay):
    """
    Measures and takes average over a number of measurements.
    :param iter:
    :param delay: Number of seconds between measurement sets
    :return: average
    """
    GPIO.setmode(GPIO.BOARD) #all the numbering we use in this script will refer to the physical numbering of the pins.#define the pin that goes to the circuit
    pin_to_circuit = 7 #GPIO4 P7 (light sensor signal AO)
    try:
        # Main loop
        sum = 0
        for i in range(iter):
	    temp = rc_time(pin_to_circuit,delay)
	    print(temp)
            sum += temp
        return sum // iter # takes the average
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()

def main():
    """
    Connects to database, receives sensor data and stores the data in the database.
    """
    database = "./database.db"

    # create a database connection
    conn = create_connection(database)
    # create the tuple that will be inserted
    datum = (datetime.now().strftime("%Y-%m-%dT%H:%M:00"),avg(6,2))
    with conn:
        create_datum(conn,datum)

if __name__ == '__main__':
    main()
