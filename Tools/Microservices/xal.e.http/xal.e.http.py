import requests
from pynats import NATSClient

srv_name = 'xal.e.http'

def SrvFunc(requestId, url):
    try:
        responce = ''
        print(url+'{', end='')
        r = requests.get(url = url, params = '')
        responce = str(r.content).replace("b'",'')#'{text1=,text2=}'
        print('}')
    except requests.exceptions.HTTPError as errh:
        print ("Http Error:",errh)
    except requests.exceptions.ConnectionError as errc:
        print ("Error Connecting:",errc)
    except requests.exceptions.Timeout as errt:
        print ("Timeout Error:",errt)
    except requests.exceptions.RequestException as err:
        print ("OOps: Something Else",err)
    except:
        print('unknown exception')
        responce = ''
    return srv_name+"|"+str(requestId)+"|"+responce


import requests
from pynats import NATSClient

def GetParam(msg, i):
    url = 'null'
    try:
        url = str(msg).split('|')[i].replace("b'",'')
    except:
        pass
    return url

serv_url = 'nats://nats-server:4222'
my_in = srv_name
my_out = my_in + '.out'

with NATSClient(serv_url) as client:
    # Connect
    client.connect()
    print('using via ' + serv_url)
    # Subscribe
    def callback(msg):
        param0 = GetParam(msg.payload,0)
        param1 = GetParam(msg.payload,1)
        print(param0 + ',' + param1)
        param1 = param1.replace("'","")
        client.publish(subject=my_out, payload=SrvFunc(param0,param1))

    client.subscribe(subject=my_in, callback=callback)
    msg = 'listening on subject in=' + my_in + ' out = '+my_out
    print(msg)

    # Publish a message
    #client.publish(subject=my_in, payload=msg)

    # wait for 1 message
    client.wait(count=99999)

