import sys
from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR


def server(address):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

    sock.bind(address)
    sock.listen(5)
    while True:
        client, addr = sock.accept()
        handler(client)


def handler(client):
    while True:
        req = client.recv(100)

        if not req:
            break
        print("received %s", str(req).encode('ascii'))
        # client.send(b'1')


server(('localhost',25000))
