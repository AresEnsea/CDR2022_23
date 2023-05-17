import socket
import struct
import sys
import time

host = "10.20.1.1"
port = 25565

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))
print("client connected to", host, port)

# format: bezier coeffs, vitesse (avec signe moins si backwards)
data_to_send = [
    "1093.197, 1248.688, 812.245, 1250.632, 824.781, 1124.295, 815.938, 937.707, 1000.0",
    "815.938, 937.707, 807.094, 751.118, 1101.555, 604.470, 1101.555, 604.470, 1000.0",
    "1101.555, 604.470, 1101.555, 604.470, 1737.123, 522.838, 2242.468, 771.623, 1000.0",
]

def make_data(destinataire, data_packed):
    return struct.pack("!BL", destinataire, len(data_packed)) + data_packed

def pack_u32(value_u32):
    val_7bytes = (
        ((value_u32 >> (7 * 4)) & 0x7f),
        ((value_u32 >> (7 * 3)) & 0x7f),
        ((value_u32 >> (7 * 3)) & 0x7f),
        ((value_u32 >> (7 * 1)) & 0x7f),
        ((value_u32 >> (7 * 0)) & 0x7f),
    )
    return struct.pack("5B", *val_7bytes)
def pack_f32(value):
    packed = struct.pack("<f", value)
    return pack_u32(struct.unpack("<L", packed)[0])

def send_path_strarr(p):
    print(f"Sending data 0xD1 to start a new path (length {len(p)})")
    client.send(make_data(2, b"\xD1" + pack_u32(len(p))))
    for s in p:
        sp = s.split(', ')
        for i, spv in enumerate(map(float, sp)):
            octet_value = 0xD2 + i
            print(f"Sending data {hex(octet_value)}")
            client.send(make_data(2, struct.pack('<B', octet_value) + pack_f32(spv)))

def send_path_floatarr(p):
    client.send(make_data(2, b"\xD1" + pack_u32(len(p))))
    for sp in p:
        for i, spv in enumerate(sp):
            octet_value = 0xD2 + i
            client.send(make_data(2, struct.pack('<B', octet_value) + pack_f32(spv)))


def controle_clavier(scr):
    while True:
        k = scr.getch()
        dx = 0
        dy = 0
        if k == curses.KEY_UP:
            dy += 1
        if k == curses.KEY_DOWN:
            dy -= 1
        if k == curses.KEY_LEFT:
            dx -= 1
        if k == curses.KEY_RIGHT:
            dx += 1

        if k == ord(' '):
            return

        if dx != 0 or dy != 0:
            scr.addstr(0,0,f"dx,dy: {dx}, {dy}          ")
            scr.refresh()
            dest = (50.0 * dx, 50.0 * dy)
            ctrl1 = (10.0 * dx, 10.0 * dy)            
            ctrl2 = (dest[0] - 10.0 * dx, dest[1] - 10.0 * dy)

            pathfloat = [[
                0.0, 0.0,
                ctrl1[0], ctrl1[1],
                ctrl2[0], ctrl2[1],
                dest[0], dest[1],
                200 if dy < 0 else 200 # speed, negative if going backwards
            ]]
            send_path_floatarr(pathfloat)
            curses.napms(1000)
        else:
            scr.refresh()
        

if len(sys.argv) >= 2:
    if sys.argv[1] == "control":
        import curses
        curses.wrapper(controle_clavier)
    elif sys.argv[1] == "data":
        send_path_strarr(data_to_send)
else:
    client.send(make_data(2, struct.pack('<B', 0xC0)))

client.close()
print("done sending")
