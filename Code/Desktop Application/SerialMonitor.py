import SerialProtocol
import platform
import os
import re
import random
import time
from datetime import datetime, timedelta


if __name__ == '__main__':
    serial_connection = SerialProtocol.SerialConnection()

    # choose test ports on operating system
    operatingPlatform = platform.system()

    # MAC OS or LINUX OS
    if operatingPlatform == 'Darwin':
        # todo: handle SerialTimeoutException
        serial_ports = ['/dev/' + port for port in os.listdir('/dev/') if re.search(r"tty.usb", port) is not None]
    elif operatingPlatform == 'Linux':
        # todo: handle SerialTimeoutException
        serial_ports = ['/dev/' + port for port in os.listdir('/dev/') if re.search(r"tty", port) is not None]
    elif operatingPlatform == 'Windows':
        print('Operating system {} is unsupported.'.format(operatingPlatform))
        exit(1)
    elif operatingPlatform == 'Java':
        print('Operating system {} is unsupported.'.format(operatingPlatform))
        exit(1)
    else:
        print('Operating system {} is unsupported.'.format(operatingPlatform))
        exit(1)

    if len(serial_ports) == 0:
        print('No serial ports connected to machine!')
        exit(0)
    for port in serial_ports:
        if serial_connection.handshake(port):
            break
    if serial_connection._connection.isOpen():
        print("Success!")

        send_recv_count = 10000

        received_messages = []
        for i in range(send_recv_count):
            serial_connection.send('ECHO', str(i))
            received_messages.append(serial_connection.receive())

        for i in range(len(received_messages)):
            print(str(i) + "--" + received_messages[i]._bodyText)

        print("Done")
