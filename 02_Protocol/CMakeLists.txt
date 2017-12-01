if (TEXT_ENCODING)
	set(02_SERVER_SRC vote_encoding_text.cpp
					  vote_server.cpp)
	set(02_CLIENT_SRC vote_encoding_text.cpp
					  vote_client.cpp)
else()
	set(02_SERVER_SRC vote_encoding_bin.cpp
					  vote_server.cpp)
	set(02_CLIENT_SRC vote_encoding_bin.cpp
					  vote_client.cpp)
endif()

message(STATUS ${02_SERVER_SRC})
message(STATUS ${02_CLIENT_SRC})
add_executable(protocol_server ${02_SERVER_SRC})
target_link_libraries(protocol_server util)
add_executable(protocol_client ${02_CLIENT_SRC})
target_link_libraries(protocol_client util)