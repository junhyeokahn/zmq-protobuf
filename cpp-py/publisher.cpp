#include <iostream>
#include <chrono>
#include <thread>

#include <build/msg.pb.h>

#include <zmq.hpp>

int main(int argc, char *argv[])
{
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind("tcp://127.0.0.1:5557");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int i (0);
    while(true) {
        std::string encoded_msg;
        RL::DataSet msg;
        msg.set_count(i);
        msg.add_joint_position(1.1);
        msg.add_joint_position(2.1);
        msg.add_joint_velocity(-1.1);
        msg.add_joint_velocity(-2.1);

        msg.SerializeToString(&encoded_msg);

        zmq::message_t zmq_msg(encoded_msg.size());
        memcpy ((void *) zmq_msg.data(), encoded_msg.c_str(),
                encoded_msg.size());
        publisher.send(zmq_msg);

        ++i;
    }

    std::cout << "pub done" << std::endl;
    return 0;
}
