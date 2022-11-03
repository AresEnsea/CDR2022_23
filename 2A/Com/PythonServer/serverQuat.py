from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading

c = threading.Condition()
flag = 0
content = "hello"

class Thread_A(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name

    def run(self):
        global flag
        global content
        while True :
            c.acquire()
            if flag == 0:
                print(f"A:{content}")
                time.sleep(2)
                flag = 1
                content = "goodbye"
                c.notify_all()
            else:
                c.wait()
            c.release()

class Thread_B(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name

    def run(self):
        global flag
        global content
        while True :
            c.acquire()
            if flag == 1:
                print(f"B:{content}")
                time.sleep(2)
                flag = 0
                content = "hello"
                c.notify_all()
            else:
                c.wait()
            c.release()

a = Thread_A("myThreadA")
b = Thread_B("myThreadB")

a.start()
b.start()

a.join()
b.join()