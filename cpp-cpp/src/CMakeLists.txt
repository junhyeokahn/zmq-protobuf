add_subdirectory(proto)

add_executable(pub "publisher.cpp")
target_link_libraries(pub proto ${Protobuf_LIBRARIES}
                                ${ZMQ_LIBRARIES})

add_executable(sub "subscriber.cpp")
target_link_libraries(sub proto ${Protobuf_LIBRARIES}
                                ${ZMQ_LIBRARIES})

add_executable(my_pub "my_publisher.cpp")
target_link_libraries(my_pub my_proto ${Protobuf_LIBRARIES}
                                      ${ZMQ_LIBRARIES})

add_executable(my_sub "my_subscriber.cpp")
target_link_libraries(my_sub my_proto ${Protobuf_LIBRARIES}
                                      ${ZMQ_LIBRARIES})
