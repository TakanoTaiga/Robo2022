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
    def createMsg(GCInfoMsgs , now):
        msg = Joy()
        msg.header.frame_id = "sc3"
        print(now)
        msg.header.stamp = now
        for GCInfoMsg in GCInfoMsgs:
            if GCInfoMsg[1] == 'J':
                msg.axes.append(GCInfoMsg[2][0])
                msg.axes.append(GCInfoMsg[2][1])
            elif GCInfoMsg[1] == 'T':
                msg.axes.append(GCInfoMsg[2])
            elif GCInfoMsg[1] == 'B':
                msg.buttons.append(GCInfoMsg[2])

        return msg


class GCText():
    GCInfoMsgs = [
        ["1" , 'J' , [0.0 , 0.0]] ,
        ["2" , 'J' ,[0.0 , 0.0]] ,
        ["3" , 'T' , 0.0] ,
        ["4" , 'T' , 0.0] ,
        ["5" , 'B' , 0] ,
        ["6" , 'B' , 0] ,
        ["7" , 'B' , 0] ,
        ["8" , 'B' , 0] ,
        ["9" , 'B' , 0] ,
        ["A" , 'B' , 0] ,
        ["B" , 'B' , 0] ,
        ["C" , 'B' , 0] ,
        ["D" , 'B' , 0] ,
        ["E" , 'B' , 0] ,
        ["F" , 'B' , 0] ,
        ["G" , 'B' , 0] ,
        ["H" , 'B' , 0] ,
        ["I" , 'B' , 0] ,
        ["J" , 'B' , 0] ,
        ["K" , 'B' , 0]
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
                        GCInfoMsg[2] = int(infoList[1])
                            
                        
        
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
                sendData = str("MYNODEIS" + "SMC_publiser")
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
                self.pub.publish(ROS_msg.createMsg(GCText.GCInfoMsgs , self.my_node.get_clock().now().to_msg()))

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
        self.my_node = Node('SMC_publiser')
        self.pub = self.my_node.create_publisher(Joy , '/joy' , 10)


def main():
    UDPHandlerObject = UDPHandler()
    UDPHandlerObject.recv()

if __name__ == "__main__":
    main()
