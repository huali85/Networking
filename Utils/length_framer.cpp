#include "frame.h"
#include "util.h"

// Read up to buf_size bytes or until delimiter, copying into the given buffer as we go
// Returns the bumber of bytes placed in buf (delimiter NOT transferred)
int GetNextMsg(FILE *in, uint8_t *buf, size_t buf_size) {
	uint16_t size = 0;
	if (fread(&size, sizeof(uint16_t), 1, in) != 1)
		return -1;
	size = ntohs(size);
	uint16_t extra = 0;
	if (size > buf_size) {
		extra = size - buf_size;
		size = buf_size; // Truncate
	}
	if (fread(buf, sizeof(uint8_t), size, in) != size) {
		fprintf(stderr, "Framing error: expected %d, read less\n", size);
		return -1;
	}
	if (extra > 0) { // Message was truncated
		uint8_t waste[BUF_SIZE];
		fread(waste, sizeof(uint8_t), extra, in); // Try to flush the channel
		return -(size + extra); // Negation of indicated size
	} else {
		return size;
	}
}

// Write the given message to the output stream
// Return number of bytes written, or -1 on failure
int PutMsg(uint8_t *buf, size_t msg_size, FILE *out) {
	if (msg_size > UINT16_MAX)
		return -1;
	uint16_t payload_size = htons(msg_size);
	if ((fwrite(&payload_size, sizeof(uint16_t), 1, out) != 1) ||
		(fwrite(buf, sizeof(uint8_t), msg_size, out) != msg_size))
		return -1;
	fflush(out);
	return msg_size;
}