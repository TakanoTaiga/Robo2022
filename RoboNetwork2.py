import socket
import netifaces
import argparse #Command line option

class setupTasks():
    def parser(self):
        parser = argparse.ArgumentParser() # Create object
        parser.add_argument("--nwiface", help="Please write your network interface" , default="lo0" , type=str)
        args = parser.parse_args()
        return args

class GCText():
    GCInfoMsgs = {
        "leftJoystic" : ['J' , [1.0 , 0.0]] ,
        "rightJoystic" : ['J' ,[0.0 , 0.0]] ,
        "leftTrigger" : ['T' , 0.0] ,
        "rightTrigger" : ['T' , 0.0] ,
        "dpadLeft" : ['B' , False] ,
        "dpadUp" : ['B' , False] ,
        "dpadRight" : ['B' , False] ,
        "dpadDown" : ['B' , False] ,
        "buttonX" : ['B' , False] ,
        "buttonY" : ['B' , False] ,
        "buttonB" : ['B' , False] ,
        "buttonA" : ['B' , False] ,
        "leftThumbstickButton" : ['B' , False] ,
        "rightThumbstickButton" : ['B' , False] ,
        "optionButton" : ['B' , False] ,
        "menuButton" : ['B' , False] ,
        "leftShoulderButton" : ['B' , False] ,
        "rightShoulderButton" : ['B' , False] ,
        "leftTriggerButton" : ['B' , False] ,
        "rightTriggerButton" : ['B' , False]
    }

    def NWtoGCmsg(self , NWMessage):
        lists = NWMessage.rsplit(",")
        del lists[0]
        del lists[len(lists) - 1]
        for text in lists:
            info = text.rsplit(":")
            GCInfoMsg = self.GCInfoMsgs[info[0]]
            if GCInfoMsg[0] == 'J':
                self.GCInfoMsgs[info[0]][1][0] = float(info[1])
                self.GCInfoMsgs[info[0]][1][1] = float(info[2])
            elif GCInfoMsg[0] == 'T':
                self.GCInfoMsgs[info[0]][1] = float(info[1])
            elif GCInfoMsg[0] == 'B':
                if info[1] == 'false':
                    self.GCInfoMsgs[info[0]][1] = False
                elif info[1] == 'true':
                    self.GCInfoMsgs[info[0]][1] = True

class UDPHandler(GCText , setupTasks):
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
                sendData = str("MYNODEIP" + self.address[0])
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
            pos = stringData.find('GCINFO')
            if pos != -1 :
                GCText.NWtoGCmsg(self, NWMessage=stringData)
                print(GCText.GCInfoMsgs)

            pos = -1
            pos = stringData.find('CLOSESESSION')
            if pos != -1 :
                self.UDPServerSocket.close()                     

    def __init__(self):
        args = setupTasks.parser(self)
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
