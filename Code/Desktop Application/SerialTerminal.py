# Author: Kevin Imlay

import SerialProtocol
import platform
import os
import re
import random
import time
from datetime import datetime, timedelta
import IcsEvents


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
        # set time and date
        # set once the milliseconds is exactly 0
        while True:
            now = datetime.now()
            # now = datetime(year=2022, month=12, day=31, hour=23, minute=59, second=30)
            if now.microsecond == 0:
                break
        print('Now: ' + now.strftime("%y;%m;%d;%H;%M;%S"))
        serial_connection.send('STDT', now.strftime("%y;%m;%d;%H;%M;%S"))
        time.sleep(1)

        # parse ics file for events
        fileData = IcsEvents.loadCalendar('test_events.ics')
        if fileData is not None:
            event_list = [exported.export() for exported in IcsEvents.parseCalendarFromICS(fileData)]
            event_list = sorted(event_list)

            # upload to tag
            for event in event_list:
                print(event)
                serial_connection.send('AEVT', str(event))

        print('System Time --- Tag Time (start or end event)')
        while True:
            print(datetime.now().strftime("%y;%m;%d;%H;%M;%S") + ' --- ' + serial_connection.receive())
