#!/usr/bin/python3
import socket
import sys

#se connect sur le port 6666 et se co a 6665 et 6668 (faut avoir donc les 3 serv de lance)

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('127.0.0.1', 6666)
sock.connect(server_address)
payload = 'NICK tmp\r\nUSER tmp * tmp :Tmp\r\nOPER admin lol\r\nCONNECT irc.ircgod.com\r\nCONNECT irc2.ircgod.com\r\nQUIT\r\n'
sock.sendall(str.encode(payload))
sock.close()
