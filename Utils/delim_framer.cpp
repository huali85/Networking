#include "frame.h"
#include "util.h"

const char kDelimiter = '\n';

// Read up to buf_size bytes or until delimiter, copying into the given buffer as we go
// Returns the bumber of bytes placed in buf (delimiter NOT transferred)
int GetNextMsg(FILE *in, uint8_t *buf, size_t buf_size) {
	int count = 0;
	int next_char;
	while (count < buf_size) {
		next_char = getc(in);
		if (next_char == EOF) {	// EOF is not a valid delimiter
			if (count > 0)
				ErrorHandling("GetNextMsg()", "Stream ended prematurely");
			else
				return -1;
		}
		if (next_char == kDelimiter)
			break;
		buf[count++] = next_char;
	}
	if (next_char != kDelimiter) { // Out of space: count == buf_size
		return -count;
	} else { // Found delimiter
		return count;
	}
}

// Write the given message to the output stream, followed by the delimiter
// Return number of bytes written, or -1 on failure
int PutMsg(uint8_t *buf, size_t msg_size, FILE *out) {
	// Check for delimiter in message
	int i;
	for (i = 0; i < msg_size; ++i) {
		if (buf[i] == kDelimiter)
			return -1;
	}
	if (fwrite(buf, 1, msg_size, out) != msg_size)
		return -1;
	fputc(kDelimiter, out);
	fflush(out);
	return msg_size;
}