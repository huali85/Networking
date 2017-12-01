#ifndef VOTE_ENCODING_H
#define VOTE_ENCODING_H

#include "vote_protocol.h"

bool Decode(uint8_t *i_buf, size_t size, VoteInfo *info);
size_t Encode(VoteInfo *info, uint8_t *o_buf, size_t buf_size);

#endif // VOTE_ENCODING_H