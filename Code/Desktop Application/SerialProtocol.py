import UartConnection
import UartPacket


headerLength = 4
messageLength = 32


class PortException(Exception):
	def __init__(self, errMessage):
		super().__init__(self, errMessage)


def _openPort(port):
	# create new UART Connection on port
	tempConnection = UartConnection.UartConnection()
	try:
		# attempt to open port
		tempConnection.openPort(port)

	# raise an exception that the port could not be opened
	except serial.SerialException:
		raise errMessage('Port {} could not be opened.'.format(port))

	# return connection
	return tempConnection


def _closePort(connection):
	# close the port
	connection.closePort()


def _handshake(connection):
	try:
		# compose acknowledge message
		synMessage = UartPacket.UartPacket('SNYC', '', headerLength, messageLength)
		sendData = synMessage.format()
		
		# send acknowledge message
		connection.send(sendData)
		
		# listen for echo back
		receivedData = connection.receive(messageLength)
		synackMessage = UartPacket.UartPacket(receivedData, headerLength, messageLength)

		# test that received message is echoed correctly
		if synackMessage == synMessage:
			# return successful handshake
			return True

		else:
			# return handshake unsuccessful
			return False

	# Raise PortException if a serial exception occured
	except serial.SerialException:
		raise errMessage('Handshake failed due to port becomming unavailable or unresponsive.')

	# Raise a PortException is a timeout exception occured
	except serial.SerialTimeoutException:
		raise errMessage('Handshake failed due to timeout.')


class SerialProtocol:

	# connection object
	_connection = None

	# port
	_port = None


	def __new__(cls, port):
		# Attempts to open a connection on the port provided.  If successful,
		# a SerialProtocol object is created.  If not, an exception is thrown.

		# check parameter
		assert isinstance(port, str), 'Port parameter must be a string.'

		# Create new UART Connection on port.
		# Throws an exception if the port could not be opened, which is
		# left unhandled here to be passed up to caller.
		tempConnection = _openPort(port)

		# attempt handshake with port
		# if handshake successful, then create object
		if _handshake(tempConnection):
			instance = super().__new__(cls)
			instance.__init__(port)
			instance._connection = tempConnection
			return instance

		# if handshake unsuccessful, return None
		else:
			return None


	def __init__(self, port):
		# save port simply for referencing
		self._port = port


	def __del__(self):
		# close connection
		_closePort(self._connection)


	def echo(self, messageText):
		message = UartPacket.UartPacket('ECHO', '{}'.format(messageText), headerLength, messageLength).format()
		self._connection.send(message)
		
	def receive(self):
		return self._connection.receive(messageLength)[headerLength:]
