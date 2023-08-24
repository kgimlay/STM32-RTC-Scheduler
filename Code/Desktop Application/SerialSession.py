# Author: Kevin Imlay

import SerialProtocol

# Define session parameters.
NUM_HANDSHAKE_ATTEMTPS = 3

class STM32SerialCom:
	# STM32 Serial Communication maps actions on the application level to
	# messages passed between the MCU and the desktop application.

	# class fields
	_connection = None

	def __new__(cls, port):
		# Attempt to open connection on port.
		tempStm32McuConnection = None
		for attempt_num in range(1, NUM_HANDSHAKE_ATTEMTPS + 1):
			tempStm32McuConnection = SerialProtocol.SerialProtocol(port)
			if tempStm32McuConnection is not None:
				break

		# Check if connection was opened.
		if tempStm32McuConnection is not None:
			instance = super().__new__(cls)
			instance.__init__(port)
			instance._connection = tempStm32McuConnection
			return instance
		else:
			return None

	def __init__(self, port):
		# All initialization was performed in __new__().
		pass

	def __del__(self):
		# Deleting connection object will perform disconnection handshake
		# and close the connection.
		del self._connection

	def setMcuTime():
		pass