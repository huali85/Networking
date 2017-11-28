cmake_minimum_required(VERSION 3.0)
project(tcpip_sample)

set(CMAKE_CXX_STANDARD 11)

# 01 Echo
add_executable(echo_server 01_Echo/util.h 01_Echo/echo_server.cpp)
add_executable(echo_client 01_Echo/util.h 01_Echo/echo_client.cpp)