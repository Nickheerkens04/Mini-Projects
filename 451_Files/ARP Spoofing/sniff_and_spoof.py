#!/usr/bin/env

from scapy.all import *

if ICMP in packet and packet[ICMP].type == 8:
	print("Original Packet... \n")
	print("Source IP: ", packet[IP].src)
	print("Destination: ", packet[IP].dst)

	ip = IP(src = packet[IP].dst, dst = packet.[IP].src, ihl = packet[IP].ihl)
	icmp = ICMP(type = 0, id = packet[ICMP].id, seq = packet[ICMP].seq)
	data = packet[Raw].load
	new_packet = ip/icmp/data

	print("spoofed packet... \n")
	print("Source IP: ", new packet[IP].src)
	print("Destination IP:", new_packet[IP].dst)
	send(new_packet, verbose = 0)

packet = sniff(filter = 'icmp and src host 192.168.65.128', prn = spoof_packet )
