import serial
import serial.tools.list_ports
import time


class UartConnection:
	# UART Connection encapsulates the most basic functions for sending and
	# receiving data over the UART to the STM32 MCU.  Communication is
	# performed using the pySerial package.

	# serial connection parameters
    _connection = None


    def __init__(self, baudrate=9600, bytesize=serial.SEVENBITS, parity=serial.PARITY_ODD, stopbits=serial.STOPBITS_TWO, read_timeout=5.0, write_timeout=5.0):
    	# test parameters
    	# baud rate
    	assert isinstance(baudrate, int), ''
    	# byte size
    	assert bytesize == serial.SIXBITS or bytesize == serial.SEVENBITS or bytesize == serial.EIGHTBITS, ''
    	# parity
    	assert parity == serial.PARITY_NONE or parity == serial.PARITY_EVEN or parity == serial.PARITY_ODD, ''
    	# stop bits
    	assert stopbits == serial.STOPBITS_ONE or stopbits == serial.STOPBITS_TWO, ''
    	# read timeout
    	assert read_timeout == None or read_timeout >= 0, ''
    	# write timeout
    	assert write_timeout == None or write_timeout >= 0, ''

    	# parmeters valid
    	# create serial object
    	self._connection = serial.Serial()
    	self._connection.baudrate = baudrate
    	self._connection.bytesize = bytesize
    	self._connection.parity = parity
    	self._connection.stopbits = stopbits
    	self._connection.timeout = read_timeout
    	self._connection.write_timeout = write_timeout
    	self._connection.xonxoff = False
    	self._connection.rtscts = False
    	self._connection.dsrdtr = False
    	self._connection.inter_byte_timeout = None
    	self._connection.exclusive = False


    def openPort(self, port):
    	self._connection.port = port
    	self._connection.open()


    def closePort(self):
    	self._connection.close()


    def send(self, message):
    	self._connection.write(message.encode('ascii'))
    	self._connection.flush()


    def receive(self, length):
    	return self._connection.read(length).decode('ascii')
