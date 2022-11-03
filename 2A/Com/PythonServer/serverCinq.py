from ctypes.wintypes import CHAR
from encodings import utf_8
import socket
import threading

c = threading.Condition()
flag = 0
content = "hello"