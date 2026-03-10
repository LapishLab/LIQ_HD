print("new message")

import wifi
wifi.connect_to_wifi("TP-IoT","",10)

import tcp
sock = tcp.connect_tcp("10.1.1.0", "5000")
tcp.send_data(sock, "Hey, how ya doin?")

sock.close()