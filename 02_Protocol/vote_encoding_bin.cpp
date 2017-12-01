/* Routines for binary encoding of vote messages
 * Wire Format:
 *                                1  1  1  1  1  1
 *  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |     Magic       |Flags|       ZERO            |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                  Candidate ID                 |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                                               |
 * |         Vote Count (only in response)         |
 * |                                               |
 * |                                               |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 */

#include "vote_encoding.h"
#include "util.h"

#define REQUEST_SIZE	4
#define RESPONSE_SIZE	12
#define COUNT_SHIFT		32
#define INQUIRE_FLAG	0x0100
#define RESPONSE_FLAG	0x0200
#define MAGIC			0x5400
#define MAGIC_MASK		0xfc00

typedef struct {
	uint16_t header;
	uint16_t candidate;
	uint32_t count_high;
	uint32_t count_low;
} VoteMsgBin;

size_t Encode(VoteInfo *v, uint8_t *o_buf, size_t buf_size) {
	if ((v->is_response && buf_size < sizeof(VoteMsgBin)) ||
		buf_size < 2 * sizeof(uint16_t)) {
		ErrorHandling("Encode", "Output buffer too small");
	}
	
	VoteMsgBin *bin = (VoteMsgBin *)o_buf;
	memset(o_buf, 0, sizeof(VoteMsgBin));
	bin->header = MAGIC;
	if (v->is_inquiry)
		bin->header |= INQUIRE_FLAG;
	if (v->is_response)
		bin->header |= RESPONSE_FLAG;
	// Convert to network byte order
	bin->header = htons(bin->header);
	bin->candidate = htons(v->candidate);
	if (v->is_response) {
		bin->count_high = htonl(v->count >> COUNT_SHIFT);
		bin->count_low = htonl((uint32_t)v->count);
		return RESPONSE_SIZE;
	} else {
		return REQUEST_SIZE;
	}
}

bool Decode(uint8_t *i_buf, size_t size, VoteInfo *v) {
	VoteMsgBin *vm = (VoteMsgBin *)i_buf;
	uint16_t header = ntohs(vm->header);
	if ((size < REQUEST_SIZE) ||
		((header & MAGIC_MASK) != MAGIC))
		return false;
	v->is_response = ((header & RESPONSE_FLAG) != 0);
	v->is_inquiry = ((header & INQUIRE_FLAG) != 0);
	v->candidate = ntohs(vm->candidate);
	if (v->is_response && size >= RESPONSE_SIZE) {
		v->count = ((uint64_t)ntohl(vm->count_high) << COUNT_SHIFT) | (uint64_t)ntohl(vm->count_low);
	}
	return true;
}
