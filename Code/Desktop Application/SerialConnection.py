# Author: Kevin Imlay

import serial


# Defines communication parameters.  Same as what has been programmed to MCU.
DEFAULT_BAUD = 9600
DEFAULT_BYTESIZE = serial.SEVENBITS
DEFAULT_PARITY = serial.PARITY_ODD
DEFAULT_STOPBITS = serial.STOPBITS_TWO
DEFAULT_READ_TIMEOUT = 1.0
DEFAULT_WRITE_TIMEOUT = 1.0
DEFAULT_SOFT_FLOW_CONTRL = False
DEFAULT_RTSCTS_FLOW_CONTRL = False
DEFAULT_DSRDTR_FLOW_CONTRL = False
DEFAULT_INTER_BYTE_TIMEOUT = None
DEFAULT_EXCLUSIVE = False


class SerialConnection:
	# UART Connection encapsulates the most basic functions for sending and
	# receiving data over the UART to the STM32 MCU.  Communication is
	# performed using the pySerial package.

	# serial connection parameters
    _connection = None


    def __init__(self):
        #

    	# parmeters valid
    	# create serial object
    	self._connection = serial.Serial()
    	self._connection.baudrate = DEFAULT_BAUD
    	self._connection.bytesize = DEFAULT_BYTESIZE
    	self._connection.parity = DEFAULT_PARITY
    	self._connection.stopbits = DEFAULT_STOPBITS
    	self._connection.timeout = DEFAULT_READ_TIMEOUT
    	self._connection.write_timeout = DEFAULT_WRITE_TIMEOUT
    	self._connection.xonxoff = DEFAULT_SOFT_FLOW_CONTRL
    	self._connection.rtscts = DEFAULT_RTSCTS_FLOW_CONTRL
    	self._connection.dsrdtr = DEFAULT_DSRDTR_FLOW_CONTRL
    	self._connection.inter_byte_timeout = DEFAULT_INTER_BYTE_TIMEOUT
    	self._connection.exclusive = DEFAULT_EXCLUSIVE


    def openPort(self, port):
        #

        #
    	self._connection.port = port
    	self._connection.open()


    def closePort(self):
        #

        #
    	self._connection.close()


    def send(self, message):
        #

        #
    	self._connection.write(message.encode('ascii'))
    	self._connection.flush()


    def receive(self, length):
        #

        #
    	return self._connection.read(length).decode('ascii')
