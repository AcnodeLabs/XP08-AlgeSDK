from use_nats import init_connection, publish, deinit_now
import time

init_connection('127.0.0.1', 'counter')

i =0 
while True:
    i = i + 1
    publish(str(i))
    time.sleep(1)

deinit_now()
