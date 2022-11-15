#check serverTiers
from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading
from queue import Queue


PORT = 25565
HOST = "10.20.1.1"
#HOST = "192.168.1.12"
SERVER = socket.gethostbyname(HOST)
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

queue = Queue()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

#ADDRphob = ("192.168.1.22",35565)
#ADDRdeim = ("192.168.1.125",45565)

ADDRphob = ("10.20.1.44",35565)
ADDRdeim = ("10.20.1.13",45565)

def handle_cient(conn, addr):
    connected = True
    while connected:
        content = conn.recv(11)
        if content!=0:
            queue.put(content)
            if str(content,FORMAT)!="":
                msg=str(content,FORMAT)
                print(f"recieved from {addr} : {msg}")
        else:
            connected = False
        if len(content) == 0:
           break
    conn.close()

def start():
    server.listen(5)
    while True:
        conn, addr = server.accept()
        if (addr[0] == ADDRphob[0]) :
            thread = threading.Thread(target=handle_cient, args=(conn, ADDRphob))
            thread.start()
        elif (addr[0] == ADDRdeim[0]) :
            thread = threading.Thread(target=handle_cient, args=(conn, ADDRdeim))
            thread.start()
        clac=queue.get()
        if len(clac) != 0:
            if (clac.decode(FORMAT)[0]=='a') and (addr[0] == ADDRdeim[0]):
                conn.sendto(clac,ADDRdeim)
                msg=str(clac,FORMAT)
                print(f"                                        emited to {addr} : {msg}")
            elif clac.decode(FORMAT)[0]=='b' and (addr[0] == ADDRphob[0]):
                conn.sendto(clac,ADDRdeim)
                msg=str(clac,FORMAT)
                print(f"                                        emited to {addr} : {msg}")
        queue.empty()

print("[STARTING] server is starting...")
start()