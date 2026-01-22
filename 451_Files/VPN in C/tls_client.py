#!/usr/bin/env python3

import socket, ssl, sys, pprint

hostname = sys.argv[1] # get hostname from user

# set up TLS context

context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
context.load_verify_locations(capath = 'etc/ssl/certs')
context.verify_mode = ssl.CERT_REQUIRED
context.check_hostname = True

#create tcp connection
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((hostname, 443))

# Bind TLS to our TCP connection, init HS
ssock = context.wrap_socket(sock, server_hostname = hostname, do_handshake_on_connect = False)
ssock.do_handshake()
# <-- initialization of the TLS Handshake

# print TLS information, prettily lol
print("--- Cipher Used: {} ---\n".format(ssock.cipher()))
print("=== Server Cert ===\n")
pprint.pprint(ssock.getpeercert())

#Send HTTP get request
request = b"GET / HTTP/1.0\r\nHost: " + hostname.encode('utf-8') + b"\r\n\r\n" 
ssock.sendall(request)

response = ssock.recv(2048)
while responce:
        pprint.pprint(responce.split(b"\r\n"))
        responce = ssock.rec(2048)


# Close TLS conection
ssock.shutdown(socket.SHUT_RDWR) 
# <-- this goes to the way sockets were built, RDWR stays open in case we need to have data we need to recieve will go to this file.
ssock.close() 
# <- doesnt take anything in its the last line of file for now (closes file descriptor)

