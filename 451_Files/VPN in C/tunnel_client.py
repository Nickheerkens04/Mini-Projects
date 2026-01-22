#!/usr/bin/env python3

import struct
import fcntl
import os
import time
from scapy.all import *

TUNSETIFF = 0x400454ca #hex value for import outport and handles a file descriptor
IFF_TUN = 0x0001
IFF_TAP = 0x0002
IFF_NO_PI = 0x1000

tun = os.open("/dev/net/tun", os.O_RDWR) # read and write to /dev/net/tun
ifr = struct.pack('16sH', b'tun%d', IFF_TUN | IFF_NO_PI)
ifname_bytes = fcntl.ioctl(tun, TUNSETIFF, ifr)
ifname = ifname_bytes.decode('UTF-8')[:16].strip("\x00")
print("Interface Name: {}".format(ifname))

os.system("ip addr add 192.168.70.110/24 dev {}".format(ifname))
os.system("ip link set dev {} up".format(ifname))

while True:
	packet = os.read(tun, 2048) # get a packet
	if packet:
		pkt = IP(packet)
		print(pkt.summary()) # display packet
	sock.sendto(packet, ("0.0.0.0", 9090))
