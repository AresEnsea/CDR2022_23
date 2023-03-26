import threading
import socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("10.20.1.1", 25565))


#def client_receive():
 #   while True:
  #      try:
   #         message = client.recv(1024).decode('utf-8')
    #        print(message)
     #   except:
     #       print('Error!')
      #      client.close()
       #     break


def client_send():
    while True:
        message = f'{input("")}'
        if(message=='1'):
            client.send(b'\0xC0')


#receive_thread = threading.Thread(target=client_receive)
#receive_thread.start()

send_thread = threading.Thread(target=client_send)
send_thread.start()