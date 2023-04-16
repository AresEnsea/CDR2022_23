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
        try:
            print('Enter command, 1 = start, 2 = turn off debug, 3 = turn on debug')
            slector=f'{input("")}'
            if(slector=='1'):
                client.send(b'\x84')
            elif(slector=='2'):
                client.send(b'\x8E')
            elif(slector=='3'):
                client.send(b'\x8F')
        except:
            client.close()
            print('deconnected')
            break


#receive_thread = threading.Thread(target=client_receive)
#receive_thread.start()

send_thread = threading.Thread(target=client_send)
send_thread.start()