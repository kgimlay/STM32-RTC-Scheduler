# Author: Kevin Imlay


import UartPacket
import serial
import time


class SerialConnection:
    #

    # serial connection parameters
    _connection = None
    _baudrate = 9600
    _bytesize = serial.SEVENBITS
    _parity = serial.PARITY_ODD
    _stopbits = serial.STOPBITS_ONE
    _read_timeout = 1.0
    _xonxoff = False
    _rtscts = False
    _dsrdtr = False
    _write_timeout = 1.0
    _inter_byte_timeout = None
    _exclusive = False

    # communication parameters
    _packet_size = 32
    _header_size = 4


    def __init__(self):
        # parameters for communicating with the tag over serial
        # corresponds with UART settings on tag
        self._connection = serial.Serial()
        self._connection.baudrate = self._baudrate
        self._connection.bytesize = self._bytesize
        self._connection.parity = self._parity
        self._connection.stopbits = self._stopbits
        self._connection.timeout = self._read_timeout
        self._connection.xonxoff = self._xonxoff
        self._connection.rtscts = self._rtscts
        self._connection.dsrdtr = self._dsrdtr
        self._connection.write_timeout = self._write_timeout
        self._connection.inter_byte_timeout = self._inter_byte_timeout
        self._connection.exclusive = self._exclusive


    def send(self, header_info, text_info):
        # create packet to send
        packet = UartPacket.UartPacket(header_info, text_info, self._header_size, self._packet_size)
        # print('Sending:\n' + str(packet))
        try:
            self._connection.write(packet.format().encode('ascii'))
            self._connection.flush()
            # time.sleep(0.05)
            return True
        except serial.serialutil.SerialTimeoutException:
            print('Serial send time out.')
            return False
        except ValueError:
            print('Packet too long.')
            return False


    def receive(self):
        # time.sleep(0.05)
        received_data = self._connection.read(self._packet_size)
        try:
            return UartPacket.UartPacket(received_data.decode('ascii'), self._header_size, self._packet_size)
        except UnicodeDecodeError:
            print('Error decoding')
            return None
        except AssertionError:
            print('Malformed packet: ', end='')
            print(received_data)
            return None


    def handshake(self, port):
        self._connection.port = port
        try:
            self._connection.open()
        except serial.SerialException:
            pass
            # todo: put some proper handling of exception here!

        # handshake
        self.send('ECHO', 'Hello!')
        # time.sleep(0.1)
        read_packet = self.receive()

        # connection successful
        if read_packet is not None:
            return True

        # connection failed
        self._connection.close()
        self._connection.port = None
        return False
