#ifndef VOTE_PROTOCOL_H
#define VOTE_PROTOCOL_H

#include <cstdint>

#define MAX_CANDIDATE 1000
#define MAX_WIRE_SIZE 500

typedef struct {
	uint64_t count;
	uint16_t candidate;
	bool	 is_inquiry;
	bool	 is_response;
} VoteInfo;

#endif // VOTE_PROTOCOL_H