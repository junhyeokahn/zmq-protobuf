cmake_minimum_required(VERSION 3.5.1)
project(cpp-py)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")
set (CMAKE_CXX_STANDARD 14)

find_package(ZMQ)
find_package(Protobuf)

message(${ZMQ_INCLUDE_DIRS})
message(${ZMQ_LIBRARIES})
message(${Protobuf_INCLUDE_DIRS})
message(${Protobuf_LIBRARIES})
message(${Protobuf_PROTOC_LIBRARIES})
message(${Protobuf_LITE_LIBRARIES})

include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}")

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER msg.proto)
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})

PROTOBUF_GENERATE_PYTHON(PROTO_PY msg.proto)
add_custom_target(myTarget ALL DEPENDS ${PROTO_PY})

add_executable(pub "publisher.cpp")
target_link_libraries(pub proto
                          ${Protobuf_LIBRARIES}
                          ${ZMQ_LIBRARIES})
