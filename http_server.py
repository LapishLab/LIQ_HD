import socket
import network

# Connect to WiFi first (assuming wifi.py has the connection code)
# from wifi import connect_wifi
# connect_wifi()

def parse_http_request(request):
    """Parse the HTTP request to extract method, path, and body."""
    lines = request.decode('utf-8').split('\r\n')
    request_line = lines[0]
    method, path, _ = request_line.split(' ')
    
    # Find the body (after empty line)
    body_start = request.find(b'\r\n\r\n') + 4
    body = request[body_start:] if body_start < len(request) else b''
    
    return method, path, body

def start_http_server(port=80):
    """Start a simple HTTP server to receive requests."""
    # Get address info
    addr = socket.getaddrinfo('0.0.0.0', port)[0][-1]
    
    # Create socket
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(5)
    
    print(f'HTTP server listening on {addr}')
    
    while True:
        try:
            cl, addr = s.accept()
            print(f'Client connected from {addr}')
            
            # Receive request
            request = b''
            while True:
                data = cl.recv(1024)
                if not data:
                    break
                request += data
                if b'\r\n\r\n' in request:
                    break
            
            if request:
                method, path, body = parse_http_request(request)
                print(f'Method: {method}, Path: {path}')
                if body:
                    print(f'Received data: {body.decode("utf-8")}')
                
                # Send response
                response = b'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nData received successfully'
                cl.send(response)
            
            cl.close()
            
        except Exception as e:
            print(f'Error: {e}')
            if 'cl' in locals():
                cl.close()

# Example usage
# if __name__ == '__main__':
#     start_http_server()</content>
# <parameter name="filePath">pico:/http_server.py