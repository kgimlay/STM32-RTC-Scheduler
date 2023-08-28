# Author: Kevin Imlay

import SerialSession
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
    # ---------------------------------------
    # ---------- Application Setup ----------
    # ---------------------------------------

    # get a list of the ports available on the machine
    testPorts = getPorts()

    # test is there are no ports
    if len(testPorts) == 0:
        print('There are no serial ports available on the machine.')
        exit(0)

    # There are available ports, try to handshake with each one until one
    # successfully handshakes.
    connectionFlag = False
    for availablePort in testPorts:
            # attempt to make connection
            Stm32Session = SerialSession.STM32SerialCom(availablePort)

            # if the connection was successful, print message
            if Stm32Session is not None:
                # report connection
                print('Connected to port {}'.format(availablePort))
                # and set flag to enter application loop
                connectionFlag = True
                # and exit loop
                break

            # if the message was unsuccessful, print message
            else:
                print('Connection could not be made with port {}'.format(availablePort))

    # If a connection was not established, report.
    if not connectionFlag:
        print('No connection could be established with MCU.')
    
    # Else, a connection was established, enter application loop.
    else:
        try:
            # --------------------------------------
            # ---------- Application Loop ----------
            # --------------------------------------
            while True:
                command, data = Stm32Session._connection.receive()
                print('[' + command + '] : ' + data)

        # Handle when a keyboard interrupt occurs, to make things tidy.
        except KeyboardInterrupt as e:
            print('\n\nUser terminated program.')

    # -----------------------------------------
    # ---------- Application Cleanup ----------
    # -----------------------------------------
    
    # disconnect
    if Stm32Session is not None:
        del Stm32Session
        # and report disconnection
        print('Disconnected from port {}'.format(availablePort))
