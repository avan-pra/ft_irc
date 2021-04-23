import socket
import os
from threading import Thread
import parser as parser
import sys

class Proxy2Server(Thread):

    def __init__(self, host, port):
        super(Proxy2Server, self).__init__()
        self.game = None # game client socket not known yet
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((host, port))

    # run in thread
    def run(self):
        while True:
            data = self.server.recv(4096)
            if data:
                #print "[{}] <- {}".format(self.port, data[:100].encode('hex'))
                try:
                    pass
                    reload(parser)
                    parser.parse(data, self.port, 'server')
                except Exception as e:
                    print 'server[{}]'.format(self.port), e
                # forward to client
                self.game.sendall(data)


class Game2Proxy(Thread):

    def __init__(self, host, port):
        super(Game2Proxy, self).__init__()
        self.server = None # real server socket not known yet
        self.port = port
        self.host = host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        sock.listen(1)
        # waiting for a connection
        self.game, addr = sock.accept()

    def run(self):
        while True:
            data = self.game.recv(4096)
            if data:
                #print "[{}] -> {}".format(self.port, data[:100].encode('hex'))
                try:
                    pass
                    reload(parser)        
                    parser.parse(data, self.port, 'client')
                except Exception as e:
                    print 'client[{}]'.format(self.port), e
                # forward to server
                self.server.sendall(data)


class Proxy(Thread):

    def __init__(self, from_host, to_host, port_server, port_proxy):
        super(Proxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port_proxy = port_proxy
        self.port_server = port_server

    def run(self):
        while True:
            print "[Awaiting connection on port {}]".format(self.port_proxy)
            self.g2p = Game2Proxy(self.from_host, self.port_proxy) # waiting for a client
            self.p2s = Proxy2Server(self.to_host, self.port_server)
            print "[Connection established with " + self.to_host + " on port " + str(self.port_server)
            self.g2p.server = self.p2s.server
            self.p2s.game = self.g2p.game

            self.g2p.start()
            self.p2s.start()

if len(sys.argv) is not 4:
    print("---------------------------------------------------\nUsage: python proxy.py <ip> <port> <port_to_listen>\n---------------------------------------------------")
    sys.exit(1)
else:
    ip = sys.argv[1]
    port_server = int(sys.argv[2])
    port_proxy = int(sys.argv[3])

    master_server = Proxy('0.0.0.0', ip, port_server, port_proxy)
    master_server.start()
