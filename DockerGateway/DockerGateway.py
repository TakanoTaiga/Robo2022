import socket
import rospy_ex

class UDPHandler():
    portTables = [] #[[port,topic]]

    def recv(self):
        while True:
            data , _ = self.UDPServerSocket.recvfrom(1024)
            stringData = data.decode()
            dataArray = rospy_ex.dataConverter(stringData)
            print(dataArray)

            if dataArray[0] == "DNS":
                #DNS:port:topicName
                ipTable = ["",""]
                ipTable[0] = dataArray[1]
                ipTable[1] = dataArray[2]
                self.portTables.append(ipTable)
            elif dataArray[0] == "EX":
                #EX:TopicName:Data
                for ipTable in self.portTables:
                    if ipTable[1] == dataArray[1]:
                        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                        sendData = dataArray[2]
                        sock.sendto(sendData.encode('utf-8'), (rospy_ex.hostName , int(ipTable[0])))
                
    def __init__(self):
        self.UDPServerSocket = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
        self.UDPServerSocket.bind((rospy_ex.hostName , 14161))

def main():
    UDPHandlerObject = UDPHandler()
    UDPHandlerObject.recv()

if __name__ == "__main__":
    main()