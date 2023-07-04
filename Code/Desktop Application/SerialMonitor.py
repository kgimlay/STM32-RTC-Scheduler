import UartConnection
import UartPacket
import platform
import os
import re


def getPorts():
    # get a list of the ports on the machine

    # get the OS that this code is running on
    operatingSystem = platform.system()

    # test that OS is Linux or OSX
    # I don't have Windows machien to test this code on...  so no Windows.
    # If you want Windows compatability, add implementation.
    assert operatingSystem == 'Darwin', 'Only Darwin operating systems supported.'

    # list of serial ports to return
    serialPorts = None

    # MAC OS
    if operatingSystem == 'Darwin':
        # list all device ports
        devicePorts = ['/dev/' + port for port in os.listdir('/dev/')]
        # remove all device ports that are not tty or cu ports
        serialPorts = [port for port in devicePorts if re.search(r'tty\.usb', port)]

    # return list of serial ports
    return serialPorts



if __name__ == '__main__':
    testPorts = getPorts()

    connection = UartConnection.UartConnection(read_timeout=None, write_timeout=None)
    for port in testPorts:
        # print(port)
        connection.openPort(port)
        message = UartPacket.UartPacket('ECHO', 'Hello!', 4, 32).format()

        for _ in range(3):
            print(message)
            connection.send(message)
            inMessage = connection.receive(32)
            print(inMessage)
            # print(len(inMessage))

        connection.closePort()