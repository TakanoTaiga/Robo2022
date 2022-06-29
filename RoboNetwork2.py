from audioop import add
import socket
import netifaces
import time

class UDPHandler():
    def __init__(self):
        address = (netifaces.ifaddresses('en0')[netifaces.AF_INET][0]['addr'] , 64201)
        self.Buffsize = 1024
        self.UDPServerSocket = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
        self.UDPServerSocket.bind(address)

        self.hostname = socket.gethostname()

    def recv(self):
        while True:
            data , addr = self.UDPServerSocket.recvfrom(self.Buffsize)
            ip , port = addr
            stringData = data.decode()
            print(stringData)


            pos = -1
            pos = stringData.find('WHOAREYOU')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock.sendto(str("MYNAMEIS" + self.hostname).encode('utf-8'), (ip , 64201))
                print(str("MYNAMEIS" + self.hostname).encode('utf-8'), (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('WHATISNODEIP')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock.sendto(str("MYNODEIP" + netifaces.ifaddresses('en0')[netifaces.AF_INET][0]['addr']).encode('utf-8'), (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('WHATISYORNODE')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock.sendto(str("MYNODEIS" + "ROS2 Node").encode('utf-8'), (ip , 64201))
                print(str("MYNODEIS" + "ROS2 Node").encode('utf-8'), (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('CLOSESESSION')
            if pos != -1 :
                self.close()

                

    def close(self):
        self.UDPServerSocket.close()


def main():
    UDPHandlerObject = UDPHandler()
    UDPHandlerObject.recv()
    UDPHandlerObject.close()

if __name__ == "__main__":
    main()
