import socket
import struct
import pyautogui


# Define the UDP server address (empty string for localhost) and port
server_address = ('', 7301)

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server address and port
udp_socket.bind(server_address)

try:
    while True:
        # Receive data from the client
        data, client_address = udp_socket.recvfrom(4)  # We expect 2 short values (2 bytes each)

        # Unpack the received data into two short values (little-endian)
        x, y = struct.unpack('<hh', data)
        pyautogui.moveTo(x,y)
        # Print the received values
        print(f"Received and set x: {x}, y: {y} from {client_address}")

except KeyboardInterrupt:
    print("UDP server stopped.")
finally:
    udp_socket.close()
