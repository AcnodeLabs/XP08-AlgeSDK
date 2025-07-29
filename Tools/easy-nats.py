import socket
import threading
import time
import platform

enc = 'ISO-8859-1'

# Function to handle incoming messages from the server
def receive_from_server(sock):
    global enc
    
    while True:
        try:
            message = sock.recv(1024).decode(enc)
            if message:
                print(f"Server: {message.strip()}")
                if 'PING' in message:
                    # Respond to server PING with a PONG
                    sock.sendall(b'PONG\r\n')
                    print("Sent: PONG")
        except Exception as e:
            print(f"Error receiving data: {e}")
            break

# Function to handle sending periodic PINGs
def send_ping(sock):
    while True:
        try:
            time.sleep(60)  # Wait for 1 minute
            sock.sendall(b'ping\r\n')
            print("Sent: ping")
        except Exception as e:
            print(f"Error sending ping: {e}")
            break

def main():
    global enc
    delim = '/'
    if 'indows' in platform.platform():
        delim = '\\'

    APPNAME = input("Enter APPNAME which is running (or just press ENTER to pick automatically from source): ")
    if (len(APPNAME)<1):
        with open('..'+delim+'Platforms'+delim+'candidate.h', 'r') as fn:
            for line in fn.readlines():
                if '#include "../Apps/' in line and 'pp"' in line:
                    APPNAME = line.split('/')[2]
                    print("Found AppName=" + APPNAME)

    # Connect to the NATS server
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect(('nats-server', 4222))
            
            # Read the initial banner until '}' is reached
            banner = sock.recv(2048).decode(enc)
            if '}' in banner:
                print(f"Connected to NATS server: {banner.strip()}")
            else:
                print("Failed to receive full banner.")
                return

            # Send the SUBSCRIBE command
            len1 = 8 + len(APPNAME)
            subscribe_command = f"sub xal.{APPNAME}.out "+str(len1)+"\r\n"
            sock.sendall(subscribe_command.encode(enc))
            print(f"Sent: {subscribe_command.strip()}")

            # Expect the server to respond with '+OK'
            response = sock.recv(1024).decode(enc)
            if '+OK' in response:
                print("Server responded: +OK")
            else:
                print("Did not receive +OK from server.")
                return

            # Start thread to handle incoming messages from server
            threading.Thread(target=receive_from_server, args=(sock,), daemon=True).start()

            # Start thread to send PING every 60 seconds
            threading.Thread(target=send_ping, args=(sock,), daemon=True).start()

            # Handle user input for sending data
            while True:
                user_data = input("Enter data to send to App="+APPNAME+" (!h for help): ")
                	# pub xal.AppName 2    [Press Enter]
	                # !h	  			[Press Enter]
                if user_data:
                    l2 = str(len(user_data))
                    pubcmd = f"pub xal.{APPNAME} {l2}\r\n{user_data}\r\n"
                    sock.sendall(pubcmd.encode(enc))
                    print(f"Sent: {pubcmd}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
