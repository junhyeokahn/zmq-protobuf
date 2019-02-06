#include <iostream>
#include <chrono>
#include <thread>


#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "build/src/proto/my_msg.pb.h"

//#include <zmq.hpp>
#include "zmq.hpp"


int main(int argc, char *argv[])
{
    using namespace google::protobuf::io;
    // publish
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind("tcp://127.0.0.1:5557");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    uint32_t idx(0);
    // sending (jpos, jvel) 2 times
    while(1) {
        std::string encoded_message;

        ZeroCopyOutputStream* raw_output =
            new StringOutputStream(&encoded_message);
        CodedOutputStream* coded_output = new CodedOutputStream(raw_output);

        coded_output->WriteLittleEndian32(2);
        JointPosition pos_msg;
        pos_msg.set_count(idx);
        pos_msg.add_q(2.1);
        pos_msg.add_q(2.2);
        std::string pos_msg_serialized;
        pos_msg.SerializeToString(&pos_msg_serialized);

        coded_output->WriteVarint32(pos_msg_serialized.size());
        coded_output->WriteString(pos_msg_serialized);

        JointVelocity vel_msg;
        vel_msg.set_count(idx);
        vel_msg.add_qdot(-1.1);
        vel_msg.add_qdot(-2.1);
        vel_msg.add_qdot(-3.1);
        std::string vel_msg_serialized;
        vel_msg.SerializeToString(&vel_msg_serialized);

        coded_output->WriteVarint32(vel_msg_serialized.size());
        coded_output->WriteString(vel_msg_serialized);

        delete coded_output;
        delete raw_output;

        zmq::message_t message(encoded_message.size());
        memcpy ((void *) message.data(), encoded_message.c_str(), 
                encoded_message.size());
        publisher.send(message);

        ++idx;
    }

    std::cout << "done pub" << std::endl;
    return 0;
}
