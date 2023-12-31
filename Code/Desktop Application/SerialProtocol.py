# Author: Kevin Imlay

import SerialConnection
import SerialPacket
import serial


# Defines message parameters
HEADER_LENGTH = 4
MESSAGE_LENGTH = 32


class SerialProtocol:
    # 

    # connection object
    _connection = None


    def __new__(cls, port):
        # Attempts to open a connection on the port provided.  If successful,
        # a SerialProtocol object is created.  If not, an exception is thrown.

        def _handshake(connection):
            # 

            # compose acknowledge message
            synMessage = SerialPacket.SerialPacket(MESSAGE_LENGTH, 
                HEADER_LENGTH, 'SNYC', '')
            sendData = synMessage.format()
            
            # send acknowledge message
            connection.send(sendData)
            
            # listen for echo back
            receivedData = connection.receive(MESSAGE_LENGTH)
            synackMessage = SerialPacket.SerialPacket(MESSAGE_LENGTH, 
                HEADER_LENGTH, receivedData)

            # test that received message is echoed correctly
            if synackMessage == synMessage:
                # return successful handshake
                return True

            else:
                # return handshake unsuccessful
                return False

        # Check port parameter.
        if not isinstance(port, str): raise TypeError

        # Create new UART Connection on port.
        tempConnection = SerialConnection.SerialConnection()

        # Attempt to open port.  If opening is unsuccessful, a
        # serial.SerialException is thrown.
        tempConnection.openPort(port)

        # Attempt handshake with port.  If handshake successful, then create 
        # object.
        if _handshake(tempConnection):
            instance = super().__new__(cls)
            instance.__init__(port)
            instance._connection = tempConnection
            return instance

        # If handshake unsuccessful, return None.
        else:
            return None


    def __init__(self, port):
        # 

        #
        pass


    def __del__(self):
        # 

        # close connection
        self._connection.closePort()


    def send(self, commandStr, dataStr):
        # 

        # Test command is of valid type.
        if not isinstance(commandStr, str): raise TypeError

        # Test that data is of valid type.
        if not isinstance(dataStr, str): raise TypeError

        message = SerialPacket.SerialPacket(
            MESSAGE_LENGTH, HEADER_LENGTH, commandStr, dataStr)
        self._connection.send(message.format())
        
    def receive(self):
        # 

        return self._connection.receive(MESSAGE_LENGTH)[HEADER_LENGTH:]
