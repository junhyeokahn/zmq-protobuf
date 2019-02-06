import zmq
import sys
import os
import time

sys.path.append(os.getcwd() + '/build')

from msg_pb2 import *

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://127.0.0.1:5557")
socket.setsockopt_string(zmq.SUBSCRIBE, "")
time.sleep(1)

msg = DataSet()

while True:

    encoded_msg = socket.recv()
    msg.ParseFromString(encoded_msg)
    print(str(msg))
