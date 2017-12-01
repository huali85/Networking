if (DELIM_FRAMER)
	set(UTIL_SRC util.h
				 util.cpp
				 frame.h
			     delim_framer.cpp)
else()
	set(UTIL_SRC util.h
				 util.cpp
				 frame.h
			     length_framer.cpp)
endif()

message(STATUS ${UTIL_SRC})
add_library(util ${UTIL_SRC})