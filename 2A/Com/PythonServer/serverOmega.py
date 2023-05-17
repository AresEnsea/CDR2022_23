import threading
import socket
import logging
import struct

host = "10.20.1.1"
port = 25565

class DestAddr:
    PHOBOS = 1
    DEIMOS = 2

ADDRphob = "10.20.1.44"
ADDRdeim = "10.20.1.13"

destinataires = {}
addr_to_destinataire = {}

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

def broadcast(addr, destinataire, message):
    print("broadcast to dest:", destinataire)
    try:
        destinataire_socket = destinataires[destinataire] # except ici si destinataire inconnu
        # destinataire_socket.send(struct.pack("!L", len(message))) # la taille en octets, en format reseau
        destinataire_socket.send(message) # le reste du message, de taille envoyee avant
        logger.info(f"{addr} sent {message} to {destinataire}")
        print(f"{addr} sent {message} to {destinataire}")
    except KeyError:
        logger.info(f"{message} from {addr} sended to log")
        print(f"{message} from {addr} sended to log")

# Function to handle clients'connections

def handle_client(client,addr):
    try:
        begindata_arr = []
        begindata = None
        while True:
            # header de 5 octets
            begindata_arr.append(client.recv(1))
            if len(begindata_arr) == 5:
                begindata = b"".join(begindata_arr)
                begindata_arr.clear()
                print("begindata from", addr, ":", begindata)

            if begindata is None:
                continue

            # l'id du receveur, et la taille en octets du message
            destinataire_id, recvlen = struct.unpack("!BL", begindata)
            message = bytearray()
            while len(message) != recvlen:
                messagepart = client.recv(1)
                if len(messagepart) == 0:
                    break
                message += messagepart
            
            if len(message) != recvlen:
                break

            broadcast(addr, destinataire_id, message)
            begindata = None
    except:
        pass
    finally:
        client.close()
        # si le client etait un destinataire connu
        who_am_i = addr_to_destinataire.get(addr)
        if who_am_i is not None:
            # alors on retire ce destinataire de la liste
            del destinataires[who_am_i]
        print(f'{str(addr)} : : deconnected')

# Main function to receive the clients connection

def receive():
    while True:
        client, addr = server.accept()
        print(f'{str(addr)} : : connected')
        if addr[0] == ADDRdeim:
            destinataires[DestAddr.DEIMOS] = client
            addr_to_destinataire[addr] = DestAddr.DEIMOS
            print(f': : : is DEIMOS')
        elif addr[0] == ADDRphob:
            destinataires[DestAddr.PHOBOS] = client
            addr_to_destinataire[addr] = DestAddr.PHOBOS
            print(f': : : is PHOBOS')
        thread = threading.Thread(target=handle_client, args=(client,addr))
        thread.start()  

if __name__ == "__main__":
    print('Server is running and listening ...')
    receive()
