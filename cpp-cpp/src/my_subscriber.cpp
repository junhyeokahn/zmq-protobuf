#include <iostream>
#include <chrono>
#include <thread>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "build/src/proto/my_msg.pb.h"

#include <zmq.hpp>

int main(int argc, char *argv[])
{
    using namespace google::protobuf::io;

    zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (1) {
        zmq::message_t update;
        subscriber.recv(&update);

        ZeroCopyInputStream* raw_input = 
            new ArrayInputStream(update.data(), update.size());
        CodedInputStream* coded_input = new CodedInputStream(raw_input);

        uint32_t num_msg;
        coded_input->ReadLittleEndian32(&num_msg);

        std::string serialized_msg;
        uint32_t serialized_size;
        JointPosition jpos_msg;
        coded_input->ReadVarint32(&serialized_size);
        coded_input->ReadString(&serialized_msg, serialized_size);
        jpos_msg.ParseFromString(serialized_msg);
        std::cout << "joint position" << std::endl;
        std::cout << "count : " << jpos_msg.count() << std::endl;
        for (int i = 0; i < jpos_msg.q_size(); ++i) {
            std::cout << i << " th q : " << jpos_msg.q(i) << std::endl;
        }

        std::string serialized_msg_2;
        uint32_t serialized_size2;
        JointVelocity jvel_msg;
        coded_input->ReadVarint32(&serialized_size2);
        coded_input->ReadString(&serialized_msg_2, serialized_size2);
        jvel_msg.ParseFromString(serialized_msg_2);
        std::cout << "joint velocity" << std::endl;
        std::cout << "count : " << jvel_msg.count() << std::endl;
        for (int i = 0; i < jvel_msg.qdot_size(); ++i) {
            std::cout << i << " th qdot : " << jvel_msg.qdot(i) << std::endl;
        }

        delete raw_input;
        delete coded_input;
        if (jvel_msg.count() == 12) {
            break;
        }
    }

    std::cout << "done sub" << std::endl;
    return 0;
}
