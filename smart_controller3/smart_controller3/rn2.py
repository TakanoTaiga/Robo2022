import socket
import netifaces
import argparse #Command line option
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy

class setupTasks():
    def parser(self):
        parser = argparse.ArgumentParser() # Create object
        parser.add_argument("--nwiface", help="Please write your network interface" , default="lo" , type=str)
        args = parser.parse_args()
        return args

class ROS_msg():
    def createMsg(GCInfoMsgs):
        msg = Joy()
        for GCInfoMsg in GCInfoMsgs:
            if GCInfoMsg[1] == 'J':
                msg.axes.append(GCInfoMsg[2][0])
                msg.axes.append(GCInfoMsg[2][1])
            elif GCInfoMsg[1] == 'T':
                msg.axes.append(GCInfoMsg[2])
            elif GCInfoMsg[1] == 'B':
                if GCInfoMsg[2]:
                    msg.buttons.append(1)
                else:
                    msg.buttons.append(0)
        return msg


class GCText():
    GCInfoMsgs = [
        ["leftJoystic" , 'J' , [0.0 , 0.0] , 1] ,
        ["rightJoystic" , 'J' ,[0.0 , 0.0] , 2] ,
        ["leftTrigger" , 'T' , 0.0 , 3] ,
        ["rightTrigger" , 'T' , 0.0 , 4] ,
        ["dpadLeft" , 'B' , False , 0] ,
        ["dpadUp" , 'B' , False , 1] ,
        ["dpadRight" , 'B' , False , 2] ,
        ["dpadDown" , 'B' , False , 3] ,
        ["buttonX" , 'B' , False , 4] ,
        ["buttonY" , 'B' , False , 5] ,
        ["buttonB" , 'B' , False , 6] ,
        ["buttonA" , 'B' , False , 7] ,
        ["leftThumbstickButton" , 'B' , False , 8] ,
        ["rightThumbstickButton" , 'B' , False , 9] ,
        ["optionButton" , 'B' , False , 10] ,
        ["menuButton" , 'B' , False , 11] ,
        ["leftShoulderButton" , 'B' , False , 12] ,
        ["rightShoulderButton" , 'B' , False , 13] ,
        ["leftTriggerButton" , 'B' , False , 14] ,
        ["rightTriggerButton" , 'B' , False , 15]
    ]

    def NWtoGCmsg(self , NWMessage):
        dataList = NWMessage.rsplit(",")
        del dataList[0]
        del dataList[len(dataList) - 1]
        for data in dataList:
            infoList = data.rsplit(':')
            for GCInfoMsg in self.GCInfoMsgs:
                if infoList[0] == GCInfoMsg[0]:
                    if GCInfoMsg[1] == 'J':
                        GCInfoMsg[2][0] = float(infoList[1])
                        GCInfoMsg[2][1] = float(infoList[2])
                    elif GCInfoMsg[1] == 'T':
                        GCInfoMsg[2] = float(infoList[1])
                    elif GCInfoMsg[1] == 'B':
                        if infoList[1] == 'true':
                            GCInfoMsg[2] = True
                        elif infoList[1] == 'false':
                            GCInfoMsg[2] = False
                            
                        
        
class UDPHandler(GCText , setupTasks , ROS_msg):
    def recv(self):
        while True:
            data , addr = self.UDPServerSocket.recvfrom(1024)
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
                self.pub.publish(ROS_msg.createMsg(GCText.GCInfoMsgs))

            pos = -1
            pos = stringData.find('CLOSESESSION')
            if pos != -1 :
                self.UDPServerSocket.close()  
                rclpy.shutdown() 
                break                 

    def __init__(self):
        args = setupTasks.parser(self)
        self.address = (netifaces.ifaddresses(args.nwiface)[netifaces.AF_INET][0]['addr'] , 64201)
        print(self.address)
        self.UDPServerSocket = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
        self.UDPServerSocket.bind(("" , 64201))
        self.hostname = socket.gethostname()
        rclpy.init(args=None)
        my_node = Node('SMC_publiser')
        self.pub = my_node.create_publisher(Joy , '/joy' , 10)


def main():
    UDPHandlerObject = UDPHandler()
    UDPHandlerObject.recv()

if __name__ == "__main__":
    main()
