#include <iostream>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "build/src/proto/msg.pb.h"

#include <zmq.hpp>

int main(int argc, char *argv[])
{
    using namespace google::protobuf::io;

    zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

    int update_nbr;
    for (update_nbr = 0; update_nbr < 10; update_nbr++) {
        // encoded_message : protobuf all
        // serialized_update : protobuf one segment
        // message : zmq
        // update : my msg

        zmq::message_t update;
        subscriber.recv(&update);

        // use protocol buffer's io stuff to package up a bunch of stuff into 
        // one stream of serialized messages. the first part of the stream 
        // is the number of messages. next, each message is preceeded by its
        // size in bytes.
        ZeroCopyInputStream* raw_input = 
            new ArrayInputStream(update.data(), update.size());
        CodedInputStream* coded_input = new CodedInputStream(raw_input);

        // find out how many updates are in this message
        uint32_t num_updates;
        coded_input->ReadLittleEndian32(&num_updates);
        std::cout << "received update " << update_nbr << std::endl;

        // now for each message in the stream, find the size and parse it ...
        for(int i = 0; i < num_updates; i++) {

            std::cout << "\titem: " << i << std::endl;

            std::string serialized_update;
            uint32_t serialized_size;
            ZmqPBExampleWeather weather_update;

            // the message size
            coded_input->ReadVarint32(&serialized_size);
            // the serialized message data
            coded_input->ReadString(&serialized_update, serialized_size);

            // parse it
            weather_update.ParseFromString(serialized_update);
            std::cout << "\t\tzip: " << weather_update.zipcode() << std::endl;
            std::cout << "\t\ttemperature: " << weather_update.temperature() << 
                std::endl;
            std::cout << "\t\trelative humidity: " << 
                weather_update.relhumidity() << std::endl;
        }

        delete coded_input;
        delete raw_input;
    }

    std::cout << "done sub" << std::endl;
    return 0;
}
