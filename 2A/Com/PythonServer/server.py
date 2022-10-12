from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading


HEADER = 64
PORT = 25565
HOST = "192.168.1.100"
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
        msg_lentgh = 20
        msg = conn.recv(msg_lentgh).decode(FORMAT)
        if msg == DISCONNECT_MESSAGE :
            connected = False
        print(f"{addr} {msg}")
        conn.sendto(bytes(5),addr)
    conn.close()

def start():
    server.listen()
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        print(f"[ACTIVATE CONNECTION] {threading.active_count() -1}")

print("[STARTING] server is starting...")
start()