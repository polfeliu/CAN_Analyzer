import socket

HOST = '192.168.1.34'  # The server's hostname or IP address
PORT = 1234        # The port used by the server


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    i = 0
    while True:
        s.sendall(b'Hello, world')
        data = s.recv(1024)
        print('Received', data)

        i += 1
        if i > 10:
            s.close()
            break