#include <iostream>


#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "build/src/proto/msg.pb.h"

#include <zmq.hpp>

int main(int argc, char *argv[])
{
    // publish
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind("tcp://127.0.0.1:5557");

    while(1)
    {
        // encoded_message : protobuf all
        // serialized_update : protobuf one segment
        // message : zmq
        // update : my msg
        using namespace google::protobuf::io;
        std::string encoded_message;
        ZeroCopyOutputStream* raw_output = 
            new StringOutputStream(&encoded_message);
        CodedOutputStream* coded_output = new CodedOutputStream(raw_output);

        int num_updates = 10;

        // prefix the stream with the number of updates
        coded_output->WriteLittleEndian32(num_updates);
        int update_nbr;

        // create a bunch of updates, serialize them and add them
        // to the stream
        for(update_nbr = 0; update_nbr < num_updates; update_nbr++) {

            ZmqPBExampleWeather update;
            update.set_zipcode( 78731 );
            update.set_temperature( 78 );
            update.set_relhumidity( 10 );

            std::string serialized_update;
            update.SerializeToString(&serialized_update);

            coded_output->WriteVarint32(serialized_update.size());
            coded_output->WriteString(serialized_update);
        }
        // clean up
        delete coded_output;
        delete raw_output;

        zmq::message_t message(encoded_message.size());
        memcpy ((void *) message.data(), encoded_message.c_str(), 
                encoded_message.size());

        publisher.send(message);
    }

    std::cout << "done pub" << std::endl;
    return 0;
}
