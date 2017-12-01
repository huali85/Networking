
#include "frame.h"
#include "util.h"
#include "vote_encoding.h"

int main(int argc, char *argv[]) {
	if (argc < 4 || argc > 5) {
		printf("Usage: %s <IP/name> <port/service> <candidate> [I]\n", argv[0]);
		exit(1);
	}

	STARTUP();

	const char *server = argv[1];
	const char *service = argv[2];
	int candidate = atoi(argv[3]);
	if (candidate < 0 || candidate > MAX_CANDIDATE)
		ErrorHandling("invalid candidate");

	bool inq = argc > 4 && strcmp(argv[4], "I") == 0;

	// Create a connected TCP socket
	sock_t sock = SetupTCPClientSocket(server, service);
	if (sock == INVALID_SOCKET)
		ErrorHandling("SetupTCPClientSocket() error");

	FILE *str = fdopen(sock, "r+");
	if (str == nullptr)
		ErrorHandling("fdopen() error");

	VoteInfo info = {};
	info.is_inquiry = inq;
	info.candidate = static_cast<uint16_t>(candidate);

	// Encode for transmission
	uint8_t out_buf[MAX_WIRE_SIZE];
	size_t req_size = Encode(&info, out_buf, MAX_WIRE_SIZE);

	// Print info
	printf("sending %d byte %s for candidate %d...\n", req_size, (inq ? "inquiry" : "vote"), candidate);

	// Frame and send
	if (PutMsg(out_buf, req_size, str) < 0)
		ErrorHandling("PutMsg() error");

	// Receive and print response
	uint8_t in_buf[MAX_WIRE_SIZE];
	size_t resp_size = GetNextMsg(str, in_buf, MAX_WIRE_SIZE);
	if (Decode(in_buf, resp_size, &info)) {
		puts("Received:\n");
		
		if (info.is_response)
			puts("Response to ");
		
		if (info.is_inquiry)
			puts("inquiry ");
		else
			puts("vote ");

		printf("for candidate %d\n", info.candidate);
		
		if (info.is_response)
			printf(" count = %llu\n", info.count);
	}

	fclose(str);
	CLOSESOCK(sock);
	CLEANUP();
	return 0;
}