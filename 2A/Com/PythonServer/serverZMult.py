import threading
import socket
import logging


host = "10.20.1.1"
port = 25565

ADDRphob = "10.20.1.44"
ADDRdeim = "10.20.1.13"

clients = []
addrs = []

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host, port))
server.listen()

Log_Format = "%(levelname)s %(asctime)s - %(message)s"
logging.basicConfig(filename = "logfile.log",
                    filemode = "w",
                    format = Log_Format, 
                    level = logging.INFO)
logger = logging.getLogger()

def broadcast(message):
    for address in addrs:
        index = addrs.index(address)
        client=clients[index]
        decoded=message.decode('utf-8')
        if(decoded[0]=='a') and (address[0] == ADDRdeim):        #if((message[0]>>6)==b'0x0') and (address[0] == ADDRdeim):
            client.send(message)
            logger.info(f"{decoded} sended to {address}")
            break
        elif(decoded[0]=='b') and (address[0] == ADDRphob):        #if((message[0]>>6)==b'0x0') and (address[0] == ADDRdeim):
            client.send(message)
            logger.info(f"{decoded} sended to {address}")
            break

# Function to handle clients'connections


def handle_client(client):
    while True:
        try:
            message = client.recv(1024)
            broadcast(message)
        except:
            clients.remove(client)
            client.close()
            break
# Main function to receive the clients connection

def receive():
    while True:
        print('Server is running and listening ...')
        client, address = server.accept()
        print(f'{str(address)} : : connected')
        clients.append(client)
        addrs.append(address)
        thread = threading.Thread(target=handle_client, args=(client,))
        thread.start()  


if __name__ == "__main__":
    receive()
