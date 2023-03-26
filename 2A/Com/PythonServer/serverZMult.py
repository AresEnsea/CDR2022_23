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
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
server.bind((host, port))
server.listen()

Log_Format = "%(levelname)s %(asctime)s - %(message)s"
logging.basicConfig(filename = "logfile.log",
                    filemode = "w",
                    format = Log_Format, 
                    level = logging.INFO)
logger = logging.getLogger()

def broadcast(message,addr):
    select=int.from_bytes(message, "big")
    if(addr[0]==ADDRphob and select > 127):
        for index, cosa in enumerate(addrs):
            if(cosa[0]==ADDRdeim):
                clients[index].send(message)
                logger.info(f"phobos sended {message} to deimos")
                print(f"phobos sended {message} to deimos")
    elif(addr[0]==ADDRdeim and select > 127):
        for index, cosa in enumerate(addrs):
            if(cosa[0]==ADDRphob):
                clients[index].send(message)
                logger.info(f"deimos sended {message} to phobos")
                print(f"deimos sended {message} to phobos")
    elif(addr[0]==host and select > 127):
        for index, cosa in enumerate(addrs):
            if(cosa[0]==ADDRphob):
                clients[index].send(message)
                logger.info(f"server sended {message} to phobos")
                print(f"server sended {message} to phobos")
    elif(addr[0]==host and select <= 127):
        for index, cosa in enumerate(addrs):
            if(cosa[0]==ADDRdeim):
                clients[index].send(message)
                logger.info(f"server sended {message} to deimos")
                print(f"server sended {message} to deimos")
    else:
        logger.info(f"{message} sended to log")
        print(f"{message} sended to log")
    #logger.info(f"{message>>8} sended to server")
    #if((addr==ADDRdeim) and ((message>>8)==b'\x01')):
        #index=addrs.index(ADDRphob)
        #clients[index].send(message)
        #logger.info(f"{message} sended to {ADDRphob}")
    #elif((addr==ADDRphob) and ((message>>8)==b'\x01')):
        #index=addrs.index(ADDRdeim)
        #clients[index].send(message)
        #logger.info(f"{message} sended to {ADDRdeim}")
    #else:
        #logger.info(f"{message>>8} sended to server")

# Function to handle clients'connections


def handle_client(client,addr):
    while True:
        try:
            message = client.recv(1)
            broadcast(message,addr)
        except:
            clients.remove(client)
            addrs.remove(addr)
            client.close()
            print(f'{str(addr)} : : deconnected')
            break
# Main function to receive the clients connection

def receive():
    while True:
        client, addr = server.accept()
        print(f'{str(addr)} : : connected')
        clients.append(client)
        addrs.append(addr)
        thread = threading.Thread(target=handle_client, args=(client,addr))
        thread.start()  


if __name__ == "__main__":
    print('Server is running and listening ...')
    receive()
