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
content=b"apizza"

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

def handle_client(conn, addr):
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
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        conn.sendall(content)
        #print(f"[ACTIVATE CONNECTION] {threading.active_count() -1}")

print("[STARTING] server is starting...")
start()