#!/usr/bin/env python3
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

def rc_time (pin_to_circuit):
    count = 0    #Output on the pin for
    GPIO.setup(pin_to_circuit, GPIO.OUT)
    GPIO.output(pin_to_circuit, GPIO.LOW)
    time.sleep(2) #the script sleep for 10 secondes    #Change the pin back to input
    GPIO.setup(pin_to_circuit, GPIO.IN)    #Count until the pin goes high
    while (GPIO.input(pin_to_circuit) == GPIO.LOW):
        count += 1
    return count#Catch when script is interupted, cleanup correctly

def avg():
    GPIO.setmode(GPIO.BOARD) #all the numbering we use in this script will refer to the physical numbering of the pins.#define the pin that goes to the circuit
    pin_to_circuit = 7 #GPIO4 P7 (light sensor signal AO)
    try:
        # Main loop
        sum = 0
        for i in range(6):
	    temp = rc_time(pin_to_circuit)
	    print(temp)
            sum += temp
        return sum // 6
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()

def main():
    database = "./database.db"

    # create a database connection
    conn = create_connection(database)
    datum = (datetime.now().strftime("%Y-%m-%dT%H:%M:00"),avg())
    with conn:
        create_datum(conn,datum)

if __name__ == '__main__':
    main()
