from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading


HEADER = 64
PORT = 25565
#HOST = "192.168.1.100"
HOST = "192.168.1.12"
SERVER = socket.gethostbyname(HOST)
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

lock = threading.Lock()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

ADDRphob = ("192.168.1.22",35565)
ADDRdeim = ("192.168.1.125",45565)

def handle_espPhob(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    connected = True
    while connected:
        conn.send(b"Ayes")
        content = conn.recv(20)
        if len(content) == 0:
           break
        else:
            print(str(content,FORMAT))
            #conn.sendall(b"yes")
        if content == DISCONNECT_MESSAGE :
            connected = False
    conn.close()

def handle_espDeim(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    connected = True
    while connected:
        content = conn.recv(20)
        if len(content) == 0:
           break
        else:
            print(str(content,FORMAT))
        if content == DISCONNECT_MESSAGE :
            connected = False
    conn.close()

def start():
    server.listen(5)
    while True:
        conn, addr = server.accept()
        if (addr[0] == ADDRphob[0]) :
            thread1 = threading.Thread(target=handle_espPhob, args=(conn, ADDRphob))
            thread1.start()
        elif (addr[0] == ADDRdeim[0]) :
            thread2 = threading.Thread(target=handle_espDeim, args=(conn, ADDRdeim))
            thread2.start()
        #conn.sendall(b"Ayes")
        #print(f"[ACTIVATE CONNECTION] {threading.active_count() -1}")

print("[STARTING] server is starting...")
start()