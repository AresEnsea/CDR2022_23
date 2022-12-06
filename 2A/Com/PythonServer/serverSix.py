#check serverTiers
from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading
from queue import LifoQueue
import logging

#Creating and Configuring Logger

Log_Format = "%(levelname)s %(asctime)s - %(message)s"

logging.basicConfig(filename = "logfile.log",
                    filemode = "w",
                    format = Log_Format, 
                    level = logging.INFO)

logger = logging.getLogger()

PORT = 25565
HOST = "10.20.1.1"
#HOST = "192.168.1.12"
SERVER = socket.gethostbyname(HOST)
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

queue = LifoQueue()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

#ADDRphob = ("192.168.1.22",35565)
#ADDRdeim = ("192.168.1.125",45565)

ADDRphob = ("10.20.1.44",35565)
ADDRdeim = ("10.20.1.13",45565)

def handle_rcv(conn, addr):
    connected = True
    while connected:
        content = conn.recv(32)
        if str(content,FORMAT)!="":
            queue.put(content)
            msg=str(content,FORMAT)
            print(f"recieved from {addr} : {msg}")
        else:
            connected = False

def handle_send(conn, addr):
    connected = True
    while connected:
        if not queue.empty():
            clac=queue.get()
            if (clac.decode(FORMAT)[0]=='a') and (addr[0] == ADDRdeim[0]):
                conn.send(clac)
                msg=str(clac,FORMAT)
                print(f"                                                                            emited to {addr} : {msg}")
            elif (clac.decode(FORMAT)[0]=='b') and (addr[0] == ADDRphob[0]):
                conn.send(clac)
                msg=str(clac,FORMAT)
                print(f"                                                                            emited to {addr} : {msg}")
            elif (clac.decode(FORMAT)[0]=='d') and (addr[0] == ADDRphob[0]):
                #conn.send(clac)
                msg=str(clac,FORMAT)
                logger.info(msg)
                print(f"                                                                            debug to {addr} : {msg}")
            else:
                queue.put(clac)
        connected = False


def start():
    server.listen(5)
    while True:
        conn, addr = server.accept()
        thread1 = threading.Thread(target=handle_rcv, args=(conn, addr))
        thread2 = threading.Thread(target=handle_send, args=(conn, addr))
        thread1.start()
        thread2.start()

print("[STARTING] server is starting...")
start()