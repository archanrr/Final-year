import paho.mqtt.client as mqtt #, os, urlparse
import time

# Define event callbacks
    
def on_connect(mosq, obj, rc):
    print ("on_connect:: Connected with result code "+ str ( rc ) )
    print("rc: " + str(rc))
    print("" )

def on_message(mosq, obj, msg):
    print ("on_message:: this means  I got a message from brokerfor this topic")
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    print ("")

def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))
    print ("")

def on_subscribe(mosq, obj, mid, granted_qos):
    print("This means broker has acknowledged my subscribe request")
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(mosq, obj, level, string):
    print(  string)


client = mqtt.Client()
# Assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_publish = on_publish
client.on_subscribe = on_subscribe

# Uncomment to enable debug messages
client.on_log = on_log


# user name has to be called before connect - my notes.
client.username_pw_set("qsphffba", "_isTLKdXFKhb")

client.connect('m16.cloudmqtt.com', 17750, 60)

# Continue the network loop, exit when an error occurs
#rc = 0
#while rc == 0:
#    rc = client.loop()
#print("rc: " + str(rc))

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
#client.loop_forever()

client.loop_start()

client.subscribe ("/frommothership" ,0 )

run = True
while run:
    #client.publish ( "/toclientloud", "from python code")

    client.subscribe  ("archan")
    time.sleep(2)

    client.publish ( "archan", "HI I AM ARCHAN")
    time.sleep(2)
