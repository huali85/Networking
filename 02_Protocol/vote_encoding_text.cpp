/* Routines for text encoding of vote messages
 * Write format:
 * Voting <v|i> [R] <canidiate ID> <count>
 */

#include "vote_encoding.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define BASE 10

static const char *kMagic = "Voting";
static const char *kVoteStr = "v";
static const char *kInqStr = "i";
static const char *kResponseStr = "R";
static const char *kDelimStr = " ";

// Encode voting message info as a text string
size_t Encode(VoteInfo *v, uint8_t *o_buf, size_t buf_size) {
	uint8_t *buf_ptr = o_buf;
	long size = (size_t)buf_size;
	int rv = snprintf((char *)buf_ptr, size, "%s %c %s %d", kMagic, (v->is_inquiry ? 'i' : 'v'), (v->is_response ? "R" : ""), v->candidate);
	buf_ptr += rv;
	size -= rv;
	if (v->is_response) {
		rv = snprintf((char *)buf_ptr, size, " %llu", v->count);
		buf_ptr += rv;
	}
	return (size_t)(buf_ptr - o_buf);
}

// Extract message information fromgiven buffer
bool Decode(uint8_t *i_buf, size_t size, VoteInfo *v) {
	char *token;
	token = strtok((char *)i_buf, kDelimStr);
	// Check for Magic
	if (token == nullptr || strcmp(token, kMagic) != 0)
		return false;
	// Get vote/inquiry indicator
	token = strtok(nullptr, kDelimStr);
	if (token == nullptr)
		return false;
	
	if (strcmp(token, kVoteStr) == 0)
		v->is_inquiry = false;
	else if (strcmp(token, kInqStr) == 0)
		v->is_inquiry = true;
	else
		return false;

	// Next token is either Response flag or candiate ID
	token = strtok(nullptr, kDelimStr);
	if (token == nullptr)
		return false;

	if (strcmp(token, kResponseStr) == 0) {
		v->is_response = true;
		token = strtok(nullptr, kDelimStr); // Get candidate ID
		if (token == nullptr)
			return false;
	} else { // No response flag, token is candidate ID
		v->is_response = false;
	}
	// Get candidate
	v->candidate = atoi(token);
	if (v->is_response) { // Response mesage should contain a count value
		token = strtok(nullptr, kDelimStr);
		if (token == nullptr)
			return false;
		v->count = strtoll(token, nullptr, BASE);
	} else {
		v->count = 0L;
	}
	return true;
}
