# Author: Kevin Imlay

import SerialConnection
import SerialPacket
import serial


# Defines message parameters
HEADER_LENGTH = 4
MESSAGE_LENGTH = 32


class PortException(Exception):
    # 
    def __init__(self, errMessage):
        super().__init__(self, errMessage)


def _openPort(port):
    # 

    # create new UART Connection on port
    tempConnection = SerialConnection.SerialConnection()
    try:
        # attempt to open port
        tempConnection.openPort(port)

    # raise an exception that the port could not be opened
    except serial.SerialException:
        raise errMessage('Port {} could not be opened.'.format(port))

    # return connection
    return tempConnection


def _closePort(connection):
    #

    # close the port
    connection.closePort()


def _handshake(connection):
    # 
    try:
        # compose acknowledge message
        synMessage = SerialPacket.SerialPacket(MESSAGE_LENGTH, HEADER_LENGTH, 'SNYC', '')
        sendData = synMessage.format()
        
        # send acknowledge message
        connection.send(sendData)
        
        # listen for echo back
        receivedData = connection.receive(MESSAGE_LENGTH)
        synackMessage = SerialPacket.SerialPacket(MESSAGE_LENGTH, HEADER_LENGTH, receivedData)

        # test that received message is echoed correctly
        if synackMessage == synMessage:
            # return successful handshake
            return True

        else:
            # return handshake unsuccessful
            return False

    # Raise PortException if a serial exception occured
    except serial.SerialException:
        raise PortException('Handshake failed due to port becomming unavailable.')

    # Raise Port Exception if malformed response from MCU
    except AssertionError:
        raise PortException('Malformed message from MCU.')

    # Raise a PortException is a timeout exception occured
    except serial.SerialTimeoutException:
        raise PortException('Handshake failed due to timeout.')


class SerialProtocol:
    # 

    # connection object
    _connection = None


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
        # 

        #
        pass


    def __del__(self):
        # 

        # close connection
        _closePort(self._connection)


    def echo(self, messageText):
        # 

        message = SerialPacket.SerialPacket(
            MESSAGE_LENGTH, HEADER_LENGTH, 'ECHO', '{}'.format(messageText))
        self._connection.send(message.format())
        
    def receive(self):
        # 

        return self._connection.receive(MESSAGE_LENGTH)[HEADER_LENGTH:]
