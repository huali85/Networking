cmake_minimum_required(VERSION 3.0)
project(tcpip_sample)

set(CMAKE_CXX_STANDARD 11)

add_executable(echo_server util.h echo_server.cpp)
add_executable(echo_client util.h echo_client.cpp)