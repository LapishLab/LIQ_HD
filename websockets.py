import network
import asyncio
from ws import AsyncWebsocketClient

# Wi-Fi connection details

WEBSOCKET_URL = "ws://echo.websocket.org" # A public echo test server

async def websocket_client_test():
    """Connects to a WebSocket server and exchanges messages."""
    try:
        # Connect to the WebSocket server
        # The WSClient class handles the connection handshake
        websocket = AsyncWebsocketClient(WEBSOCKET_URL)
        await websocket.connect()
        print(f"Connected to {WEBSOCKET_URL}")

        # Send a message
        message_to_send = "Hello from MicroPython!"
        await websocket.send(message_to_send)
        print(f"Sent: {message_to_send}")

        # Receive a message (the echo from the server)
        received_message = await websocket.recv()
        print(f"Received: {received_message}")
        
        # Close the connection manually
        await websocket.close()
        print("Connection closed")

    except Exception as e:
        print(f"WebSocket error: {e}")

async def main():
    await websocket_client_test()

# Run the asynchronous main function
asyncio.run(main())
