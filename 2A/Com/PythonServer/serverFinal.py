#check serverTiers
from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading
from queue import Queue


PORT = 25565
#HOST = "192.168.1.100"
HOST = "192.168.1.12"
SERVER = socket.gethostbyname(HOST)
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

queue = Queue()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

ADDRphob = ("192.168.1.22",35565)
ADDRdeim = ("192.168.1.125",45565)

def handle_cient(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    connected = True
    while connected:
        content = conn.recv(11)
        if content!=0:
            queue.put(content)
        else:
            connected = False
        if len(content) == 0:
           break
        else:
            print(str(content,FORMAT))
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
            if clac.decode(FORMAT)[0]=='a':
                conn.sendto(clac,ADDRdeim)
        queue.empty()

print("[STARTING] server is starting...")
start()