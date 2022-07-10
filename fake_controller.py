import socket
import netifaces
import argparse #Command line option

class setupTasks():
    def parser(self):
        parser = argparse.ArgumentParser() # Create object
        parser.add_argument("--target", help="Please write your target ip" , default="127.0.0.0" , type=str)
        args = parser.parse_args()
        return args

class GCText():
    GCInfoMsgs = [
        ["leftJoystic" , 'J' , ['0.0' , '0.0']] ,
        ["rightJoystic" , 'J' ,['0.0' , '0.0']] ,
        ["leftTrigger" , 'T' , '0.0'] ,
        ["rightTrigger" , 'T' , '0.0'] ,
        ["dpadLeft" , 'B' , 'false'] ,
        ["dpadUp" , 'B' , 'false'] ,
        ["dpadRight" , 'B' , 'false'] ,
        ["dpadDown" , 'B' , 'false'] ,
        ["buttonX" , 'B' , 'false'] ,
        ["buttonY" , 'B' , 'false'] ,
        ["buttonB" , 'B' , 'false'] ,
        ["buttonA" , 'B' , 'false'],
        ["leftThumbstickButton" ,'B' , 'false'] ,
        ["rightThumbstickButton" , 'B' , 'false'] ,
        ["optionButton" , 'B' , 'false'] ,
        ["menuButton" , 'B' , 'false'] ,
        ["leftShoulderButton" , 'B' , 'false'] ,
        ["rightShoulderButton" , 'B' , 'false'] ,
        ["leftTriggerButton" , 'B' , 'false'] ,
        ["rightTriggerButton" , 'B' , 'false']
    ]

class UDPHandler(GCText , setupTasks):
    msgs = GCText.GCInfoMsgs

    def sendGCText(self):
        packet = "GCINFO,"
        for msg in self.msgs:
            if msg[1] == 'J':
                packet += msg[0] + ':' + msg[2][0] + ':' + msg[2][1]
            else:
                packet += msg[0] + ':' + msg[2]
            
            packet += ','
        packet += 'END'

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(packet.encode('utf-8'), (self.args.target , 64201))
        sock.close()

    def rmNode(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto("CLOSESESSION".encode('utf-8'), (self.args.target , 64201))
        sock.close()

    def __init__(self):
        self.args = setupTasks.parser(self)
        

def main():
    UDPHandlerObject = UDPHandler()

    while True:
        commands = input(">>")
        command = commands.rsplit(" ")
        for msg in UDPHandlerObject.msgs:
            pos = -1
            if command[0] == msg[0]:
                if msg[1] == 'J':
                    msg[2] = [command[1] , command[2]]
                else:
                    msg[2] = command[1]

                UDPHandlerObject.sendGCText()

        pos = -1
        pos = command[0].lower().find('exit')
        if pos != -1:
            break

        pos = -1
        pos = command[0].lower().find('ls')
        if pos != -1:
            print(UDPHandlerObject.msgs)
        
        pos = -1
        pos = command[0].lower().find('demo')
        if pos != -1:
            UDPHandlerObject.sendGCText()
        
        pos = -1
        pos = command[0].lower().find('rm')
        if pos != -1:
            UDPHandlerObject.rmNode()
            

if __name__ == "__main__":
    main()
