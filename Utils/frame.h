#ifndef NETWORK_FRAME_H
#define NETWORK_FRAME_H

#include <cstdint>
#include <cstdio>

int GetNextMsg(FILE *in, uint8_t *buf, size_t buf_size);
int PutMsg(uint8_t *buf, size_t msg_size, FILE *out);

#endif // NETWORK_FRAME_H