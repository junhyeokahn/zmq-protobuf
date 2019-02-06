cmake_minimum_required(VERSION 3.5.1)
project(py-py)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cpp-cpp/cmake")
set (CMAKE_CXX_STANDARD 14)

find_package(Protobuf)

#message(${ZMQ_INCLUDE_DIRS})
#message(${ZMQ_LIBRARIES})
#message(${Protobuf_INCLUDE_DIRS})
#message(${Protobuf_LIBRARIES})
#message(${Protobuf_PROTOC_LIBRARIES})
#message(${Protobuf_LITE_LIBRARIES})

include_directories(${Protobuf_INCLUDE_DIR})

PROTOBUF_GENERATE_PYTHON(PROTO_PY msg.proto)
add_custom_target(myTarget ALL DEPENDS ${PROTO_PY})
#add_custom_target(myTarget ${PROTO_PY})
