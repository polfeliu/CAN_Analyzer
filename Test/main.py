import socket
import sys
import time
from pprint import pprint
HOST = '192.168.1.34'  # The server's hostname or IP address
PORT = 1234        # The port used by the server
### Must be run as admin
'''
with socket.socket(socket.AF_INET, socket.SOCK_RAW) as s:
    try:
        s.connect((HOST, PORT))
        i = 0
        while True:
            s.send(b'asdf')
            print("sent")
            data = s.recv(1024)
            print('Received', data)

            i += 1
            if i > 10:
                s.close()
                break
    except KeyboardInterrupt:
        print("asedffadsfd")
        s.close()
        sys.exit(1)
'''


import can

bus = can.interface.Bus(bustype='serial', channel='socket://192.168.1.34:1234')

notifier = can.Notifier(bus, [can.Printer()])

while True:
    '''msg = bus.recv(0.1)
    print("message")
    pprint(msg)'''
    #print(msg.dlc, msg.data, msg.arbitration_id)
    message = can.Message(arbitration_id=123, is_extended_id=False,
                          data=[0x11, 0x22, 0x33])
    bus.send(message, timeout=0.2)

    time.sleep(0.1)





