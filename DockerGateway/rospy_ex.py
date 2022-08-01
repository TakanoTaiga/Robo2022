import random
import socket
import netifaces

def dataConverter(data):
    rtData = [""]
    counter = 0
    for char in data:
        if char == "\n":
            continue
        elif char != ":":
            rtData[counter] += char
        else:
            rtData.append("")
            counter += 1

    return rtData    

hostName = "127.0.0.1"

myPort = 0
UDPServerSocket = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)

topicNameGlobal = ""

_exSetup = False
def EXsetup(topicName):
    global myPort
    global UDPServerSocket
    global hostName
    global topicNameGlobal
    global _exSetup

    myPort = random.randint(14170, 14570)
    UDPServerSocket.bind((hostName , myPort))

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sendData = "DNS:" + str(myPort) + ":" + topicName
    sock.sendto(sendData.encode('utf-8'), (hostName , 14161))

    topicNameGlobal = topicName

    _exSetup = True

_dgSetup = False
def DGsetup(topicName):
    global topicNameGlobal
    global _dgSetup
    global UDPServerSocket

    address = (netifaces.ifaddresses('eth0')[netifaces.AF_INET][0]['addr'] , 14160)
    UDPServerSocket.bind(address)
    
    topicNameGlobal = topicName
    _dgSetup = True

def publisher(data):
    global topicNameGlobal
    global _exSetup
    global _dgSetup

    if _exSetup:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sendData = "DATA:" + topicNameGlobal + ":" + str(data)
        sock.sendto(sendData.encode('utf-8'), (hostName , 14160))
    elif _dgSetup:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sendData = "EX:" + topicNameGlobal + ":" + str(data)
        sock.sendto(sendData.encode('utf-8'), ("gateway.docker.internal" , 14161))

subData = ""
def subscription(function):
    global UDPServerSocket
    global subData
    while True:
        data , _ = UDPServerSocket.recvfrom(1024)
        subData = data.decode()
        function()