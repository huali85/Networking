
#include "frame.h"
#include "util.h"
#include "vote_encoding.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	STARTUP();

	sock_t serv_sock = SetupTCPServerSocket(atoi(argv[1]));
	if (serv_sock == INVALID_SOCKET)
		ErrorHandling("SetupTCPServerSocket() error");

	uint64_t counts[MAX_CANDIDATE + 1];

	while (1) {
		// Wait for a client to connect
		sock_t client_sock = AcceptTCPConnection(serv_sock);

		FILE *channel = fdopen(client_sock, "r+");
		if (channel == nullptr)
			ErrorHandling("fdopen() error");

		// Receive messages until connection closes
		int size;
		uint8_t in_buf[MAX_WIRE_SIZE];
		VoteInfo v;
		while ((size = GetNextMsg(channel, in_buf, MAX_WIRE_SIZE)) > 0) {
			memset(&v, 0, sizeof(v));
			printf("Received message (%d bytes)\n", size);
			if (Decode(in_buf, size, &v)) {	// Parse to get VoteInfo
				if (!v.is_response) {	// Ignore non-requests
					v.is_response = true;					
					if (v.candidate >= 0 && v.candidate <= MAX_CANDIDATE) {
						if (!v.is_inquiry)
							++counts[v.candidate];
						v.count = counts[v.candidate];
					}
				}
				uint8_t out_buf[MAX_WIRE_SIZE];
				size = Encode(&v, out_buf, MAX_WIRE_SIZE);
				if (PutMsg(out_buf, size, channel) < 0) {
					fputs("Error framing/outputting message\n", stderr);
					break;
				} else {
					printf("Processed %s for candidate %d; current count is %llu.\n", (v.is_inquiry ? "inquiry" : "vote"), v.candidate, v.count);
				}
				fflush(channel);
			} else {
				fputs("Parse error, closing connection.\n", stderr);
				break;
			}
		}
		puts("Client finished");
		fclose(channel);
	}

	CLOSESOCK(serv_sock);
	CLEANUP();
	return 0;
}
