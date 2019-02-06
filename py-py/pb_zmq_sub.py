import zmq
import sys
import google.protobuf.text_format
# protoc output for testmsg.proto:
from msg_pb2 import *

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:5556")
socket.setsockopt_string(zmq.SUBSCRIBE, "")

c = Container()

while True:

    msg = socket.recv()
    c.ParseFromString(msg)
    print(str(c))
