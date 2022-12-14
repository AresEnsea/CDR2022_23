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

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")

    connected = True
    while connected:
        #msg_lentgh = conn.recv(HEADER).decode(FORMAT)
        #msg_lentgh = int(msg_lentgh)
        
        content = conn.recv(1024)
        print(content)
        if len(content) ==0:
           break
        if str(content,FORMAT) == '\r\n':
            continue
        else:
            print(str(content,FORMAT))
            conn.sendto(content,("192.168.1.102",PORT))
    conn.close()

def start():
    server.listen()
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        #print(f"[ACTIVATE CONNECTION] {threading.active_count() -1}")

print("[STARTING] server is starting...")
start()