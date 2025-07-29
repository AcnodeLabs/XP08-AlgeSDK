import socket
import threading
import time


enc = 'ISO-8859-1'

# Function to handle incoming messages from the server
def receive_from_server(sock):
    global enc
    
    while True:
        if gSock==None:
            break
        try:
            message = sock.recv(1024).decode(enc)
            if message:
                if 'PING' in message:
                    # Respond to server PING with a PONG
                    sock.sendall(b'PONG\r\n')
        except Exception as e:
            break

# Function to handle sending periodic PINGs
def send_ping(sock):
    while True:
        try:
            time.sleep(60)  # Wait for 1 minute
            sock.sendall(b'ping\r\n')
        except Exception as e:
            print(f"\nError sending ping: {e}")
            break

gSock = None
gCHANNEL = ''

def publish(user_data):
    global enc
    global gSock
    global gCHANNEL
    ln = str(len(user_data))
    pubcmd = f"pub {gCHANNEL} {ln}\r\n{user_data}\r\n"
    print(pubcmd)
    gSock.sendall(pubcmd.encode(enc))
    
def init_connection(nats_server, CHANNEL, alsoSubscribe = False):
    global gSock
    global gCHANNEL
    # Connect to the NATS server
    try:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((nats_server, 4222))
            
            # Read the initial banner until '}' is reached
            banner = sock.recv(2048).decode(enc)
            if '}' in banner:
                gSock = sock
                
            else:
                gSock = None
                return

            if alsoSubscribe:
            # Send the SUBSCRIBE command
            #len1 = 8 + len(CHANNEL)
            #subscribe_command = f"sub xal.{APPNAME}.out "+str(len1)+"\r\n"
                subscribe_command = f"sub {CHANNEL} "+str(len(CHANNEL))+"\r\n"
            
                sock.sendall(subscribe_command.encode(enc))
    
            # Expect the server to respond with '+OK'
                response = sock.recv(1024).decode(enc)
                if '+OK' in response:
                    gCHANNEL = CHANNEL
                else:
                    gCHANNEL = ''
                return
            
            else:
                gCHANNEL = CHANNEL
            # Start thread to handle incoming messages from server
            
            threading.Thread(target=receive_from_server, args=(sock,), daemon=True).start()

            # Start thread to send PING every 60 seconds
            threading.Thread(target=send_ping, args=(sock,), daemon=True).start()
        
        except:
            pass

    except Exception as e:
        print(f"\nError: {e}")

def deinit_now():
    if gSock!=None:
        gSock.close()
    pass

if __name__ == "__main__":
    init_connection('127.0.0.1', 'xal.ework.upwork-daemon')
    while True:
        publish('hi')
        time.sleep(5)
    deinit_now()