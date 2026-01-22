#!/usr/bin/env python3

from scapy.all import *

#spoof packets here
print("sending spoofed packets..\n")
ip = IP(src = "", dst = "")
icmp = ICMP()
packet = ip/icmp
packet.show()
send(packet, verbose=0)

#show packets
#def show_packets(packet):
#	print(packet.summary())

#packet = sniff(iface = 'eth0', filter= 'icmp', prn= show_packet)
