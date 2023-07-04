# Defines the character to postfix a packet's body segment with
emptyBodyFillerChar = '*'


class UartPacket:
	# A UART Packet encapsulates the necessary parameters for building a
	# packet to be sent to the MCU over UART communication running the
	# Desktop Application UART Communicator module.
	#
	# A packet is formatted simply as a header of N characters contatenated
	# with a body of M characters.  The total packet length is O, and the
	# MCU expects packets of a fixed size so there is the additional
	# constraint that N+M = O, where O is the expected packet length.  To
	# achieve this, the body must not exceed O-N characters, and the body
	# is postfixed with null characters if it does not consume the full
	# O-N length.

	# Packet parameters.
	# Expected length of the packet
	_packetLength = None

	# Packet contents.
	# Text of the header segment
	_headerText = None
	# Text of the body segment
	_bodyText = None

	def __init__(self, *args):
		# test the number or arguments
		assert len(args) == 3 or len(args) == 4, '''UartPacket parameter args must be of length 3 (from string) or 4 (from individual header/body)'''

		# if the number of arguments is 4, then create packet from header
		# text, body text, and packet size information
		if len(args) == 4:
			self._fromParams(args[0], args[1], args[2], args[3])

		# if the number of arguments is 1, then create packet from string
		# format of a packet
		elif len(args) == 3:
			self._fromString(args[0], args[1], args[2])


	def format(self):
		# add header and body
		formatStr = self._headerText + self._bodyText

		# postfix null characters if needed
		remainingCount = self._packetLength - len(formatStr)
		for _ in range(remainingCount):
			formatStr += emptyBodyFillerChar

		# return formatted string
		return formatStr


	def _fromParams(self, headerText, bodyText, headerLength, packetLength):
		# Initializer for a packet object.  Performs error checks that
		# packet conforms to O=N+M and info stored in packet are strings.

		# test info parameters
		assert isinstance(headerText, str), '''UartPacket parameter headerText must be a string.'''
		assert isinstance(bodyText, str), '''UartPacket parameter bodyText must be a string.'''

		# test length parameters
		assert isinstance(headerLength, int), '''UartPacket parameter headerLength must be an integer'''
		assert isinstance(packetLength, int), '''UartPacket parameter packetLength must be an integer'''

		# test header and body length
		assert headerLength > 0, '''UartPacket parameter headerLength must be greater than 0'''
		assert packetLength > headerLength, '''UartPacket parameter packetLength must be greater than headerLength'''
		assert len(headerText) == headerLength, '''UartPacket parameter headerText must be of length headerLength'''
		assert len(bodyText) <= packetLength - len(headerText), '''UartPacket parameter bodyText must not be longer than packetLength - headerLength'''

		# all tests passed, store parameters
		self._packetLength = packetLength
		self._headerText = headerText
		self._bodyText = bodyText


	def _fromString(self, packetString, headerLength, packetLength):
		# Initializer for packet object.  Parses packet from string.  Performs
		# error checks that string conforms to packet length requirements.

		# test info parameters
		assert isinstance(headerLength, int), '''UartPacket parameter headerLength must be an integer'''
		assert isinstance(packetLength, int), '''UartPacket parameter packetLength must be an integer'''

		# test string parameter
		assert isinstance(packetString, str), '''UartPacket parameter packetString must be a string'''

		# test packet length requirements
		assert headerLength > 0, '''UartPacket parameter headerLength must be greater than 0'''
		assert packetLength > headerLength, '''UartPacket parameter packetLength must be greater than headerLength'''

		# test string conforms to packet length requirements
		assert len(packetString) == packetLength, '''UartPacket parameter packetString must be as long as packetLength.'''

		# all tests passed, parse and store parameters
		self._packetLength = packetLength
		self._headerText = packetString[0:headerLength]
		self._bodyText = packetString[headerLength:].split(emptyBodyFillerChar)[0]


	def __str__(self):
		packetNameString = str(id(self))
		packetLengthString = "Packet Length: " + str(self._packetLength)
		HeaderLengthString = "Header Length: " + str(len(self._headerText))
		BodyLengthString = "Body Length: " + str(len(self._bodyText))
		headerTextString = "Header Text: " + self._headerText
		bodyTextString = "Body Text: " + self._bodyText

		formattedString = "Packet " + packetNameString + '\n' \
		+ '\t' + packetLengthString + '\n' \
		+ '\t' + HeaderLengthString + '\n' \
		+ '\t' + BodyLengthString + '\n' \
		+ '\t' + headerTextString + '\n' \
		+ '\t' + bodyTextString

		return formattedString
