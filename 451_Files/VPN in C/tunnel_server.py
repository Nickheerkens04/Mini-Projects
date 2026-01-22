#!/usr/bin/env python3

from scapy.all import *

server_ip = "0.0.0.0"
server_port = 9090

os.system("ip.addr add 192.168.70.70/24 dev {}".format(ifname))
os.system("ip link set dev {} up".format(ifname))

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((server_ip, server_port))

while True:
	data, (ip, port) = sock.recv(2048)
	print("{}:{} --> {}:{}".format(ip, port, server_ip, server_port))
	pkt = IP(data)
	print("Inside: {} --> {}".format(pkt,src, pkt.dest))
	os.write(tun, data)
