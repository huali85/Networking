cmake_minimum_required(VERSION 3.0)
project(tcpip_sample)

set(CMAKE_CXX_STANDARD 11)

option(DELIM_FRAMER "Use delimiter-based frame" ON)
option(TEXT_ENCODING "Use text-based message encoding" ON)

include_directories("${PROJECT_SOURCE_DIR}/Utils")
add_subdirectory(Utils)

# 01 Echo
add_subdirectory(01_Echo)

#02 Protocol
if (NOT WIN32)
	add_subdirectory(02_Protocol)
endif()
