import asyncio
from websockets.sync.client import connect

import websockets

def hello():
    # Use 'ws://' for standard connections and 'wss://' for secure (TLS) connections
    uri = "ws://192.168.0.61:8765" 

    
    try:
        with connect(uri) as websocket:
            websocket.send("Hello world!")
            message = websocket.recv()
            print(f"Received: {message}")
    except ConnectionRefusedError:
        print(f"Connection failed. Is a server running at {uri}?")

hello()
