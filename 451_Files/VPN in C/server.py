#1/usr/bin/env python3

import socket, ssl, pprint

html = """
HTTP/1.1 200 OK\r\nContext-Type: text/html\r\n\r\n
<!DOCTYPE html><html><body><h1>This is a website. Welcome to ABC.com</h1></body></html>
"""

SEVER_CERT = ''
SERVER_PRIVATE = ''

#TLS context
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain(SERVER_CERT, SEVER_PRIVATE)

#tcp server
sock = socket.socket(socketAF_INET, socket.SOCK_STREAM, 0)
sock.bind(('0.0.0.0', 443))
sock.listen(5)

while True:
	newsock, fromaddr = sock.accept()
	try:
	    ssock = context.wrap_socket(newsock, server_side = True)
	    print("TLS connection extablshed...\n")

	    data = ssock.recv(1024)
	    pprint.pprint("Request: {}".format(data))
	    ssock.sendall(html.encode('utf-8'))

	    ssock.shutdown(socket.SHUR_RDWR)
	    ssock.close()

	except Exception:
	    print("TLS connection failed...\n")
	    continue

