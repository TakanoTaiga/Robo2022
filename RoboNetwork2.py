import socket
import netifaces
import argparse #Command line option

class UDPHandler():
    def parser(self):
        parser = argparse.ArgumentParser() # Create object
        parser.add_argument("--nwiface", help="Please write your network interface" , default="lo0" , type=str)
        args = parser.parse_args()
        return args

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
                sendData = str("MYNAMEIS" + self.hostname)
                sock.sendto(sendData.encode('utf-8'), (ip , 64201))
                print(str("MYNAMEIS" + self.hostname).encode('utf-8'), (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('WHATISNODEIP')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sendData = str("MYNODEIP" + self.address)
                sock.sendto(sendData.encode('utf-8'), (ip , 64201))
                print(sendData , (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('WHATISYORNODE')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sendData = str("MYNODEIS" + "ROS2 Node")
                sock.sendto(sendData.encode('utf-8'), (ip , 64201))
                print(sendData, (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('PING')
            if pos != -1 :
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sendData = "CHECK"
                sock.sendto(sendData.encode('utf-8'), (ip , 64201))
                print(sendData, (ip , 64201))
                sock.close()

            pos = -1
            pos = stringData.find('CLOSESESSION')
            if pos != -1 :
                self.close()                

    def close(self):
        self.UDPServerSocket.close()

    def __init__(self):
        args = self.parser()
        self.address = (netifaces.ifaddresses(args.nwiface)[netifaces.AF_INET][0]['addr'] , 64201)
        self.Buffsize = 1024
        self.UDPServerSocket = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
        self.UDPServerSocket.bind(self.address)

        self.hostname = socket.gethostname()


def main():
    UDPHandlerObject = UDPHandler()
    UDPHandlerObject.recv()
    UDPHandlerObject.close()

if __name__ == "__main__":
    main()
