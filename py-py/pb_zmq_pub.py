import zmq
import time
import sys
import binascii
import google.protobuf.text_format

# protoc output for testmsg.proto:
from msg_pb2 import *

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5556")

i = 0
# create example protobuf message:
c = Container()
c.type = MT_TEST1
c.note.append("beipacktext zeile 1")
c.note.append("beipacktext zeile 2")

p = c.pin.add()
p.name = "pi"
p.type = HAL_FLOAT
p.halfloat = 3.14

p = c.pin.add()
p.name = "e"
p.type = HAL_FLOAT
p.halfloat = 2.71828

print (c.ByteSize())
print (str(c))
serialized_msg = c.SerializeToString()

while True:

    i += 1
    print(i)
    socket.send(serialized_msg)
    time.sleep(1)
