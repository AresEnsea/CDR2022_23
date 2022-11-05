from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading
import time

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

ADDRphob = ("192.168.1.22",35565)
ADDRdeim = ("192.168.1.125",45565)

c = threading.Condition()
flag = 0
contentPhob = b"hello"
contentDeim = b"hello"

class SthreadPhob(threading.Thread):
    def __init__(self, *args):
        threading.Thread.__init__(self)
        self.args = args

    def run(self):
        global flag
        global contentPhob
        global contentDeim
        while True :
            print(f"Phobos connected")
            conn = self.args[0]
            c.acquire()
            if flag == 0:
                conn.send(contentDeim)
                flag = 1
                contentPhob = conn.recv(20)
                print(str(contentPhob,FORMAT))
                c.notify_all()
            else:
                c.wait()
            c.release()
            conn.close()

class SthreadDeim(threading.Thread):
    def __init__(self, *args):
        threading.Thread.__init__(self)
        self.args = args

    def run(self):
        global flag
        global contentPhob
        global contentDeim
        while True :
            print(f"Deimos connected")
            conn = self.args[0]
            c.acquire()
            if flag == 1:
                conn.send(contentPhob)
                flag = 0
                contentDeim = conn.recv(20)
                print(str(contentDeim,FORMAT))
                c.notify_all()
            else:
                c.wait()
            c.release()
            conn.close()


def start():
    server.listen(5)
    while True:
        conn, addr = server.accept()
        if (addr[0] == ADDRphob[0]) :
            thread1 = SthreadPhob(conn)
            thread1.start()
        elif (addr[0] == ADDRdeim[0]) :
            thread2 = SthreadDeim(conn)
            thread2.start()
print("[STARTING] server is starting...")
start()