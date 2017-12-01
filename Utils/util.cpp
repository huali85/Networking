#include "util.h"

void ErrorHandling(const char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void ErrorHandling(const char *msg, const char *detail) {
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

void PrintSocketAddress(const sockaddr *address) {
	if (address == nullptr)
		return;

	void *numeric_address;
	port_t port;
	switch (address->sa_family) {
	case AF_INET:
		numeric_address = &((sockaddr_in *)address)->sin_addr;
		port = ntohs(((sockaddr_in *)address)->sin_port);
		break;

	case AF_INET6:
		numeric_address = &((sockaddr_in6 *)address)->sin6_addr;
		port = ntohs(((sockaddr_in6 *)address)->sin6_port);
		break;

	default:
		puts("unknown socket type");
		return;
	}

	// Convert binary to printable address
	char buf_addr[INET6_ADDRSTRLEN];
	if (inet_ntop(address->sa_family, numeric_address, buf_addr, sizeof(buf_addr)) == nullptr) {
		puts("invalid address");
	}
	else {
		printf("%s", buf_addr);
		if (port != 0)
			printf(":%u", port);
	}
}

sock_t SetupTCPClientSocket(const char *host, const char *service) {
	addrinfo hints = {};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *serv_addr;
	int result = getaddrinfo(host, service, &hints, &serv_addr);
	if (result != 0)
		ErrorHandling("getaddrinfo() error");

	sock_t sock = INVALID_SOCKET;
	for (addrinfo *addr = serv_addr; addr != nullptr; addr = addr->ai_next) {
		sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		if (sock == INVALID_SOCKET)
			continue;   // Socket creation failed, try next address

		if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
			break;  // Socket connection succeeded, break and return socket

		CLOSESOCK(sock);    // Socket connection failed, try next address
		sock = INVALID_SOCKET;
	}

	freeaddrinfo(serv_addr);
	return sock;
}

sock_t SetupTCPServerSocket(uint16_t port) {
	// Create a dual stack socket
	sock_t sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sock != INVALID_SOCKET) {
		
		int off = 0;
		setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&off, sizeof(off));
		
		sockaddr_in6 addr = {};
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		addr.sin6_addr = in6addr_any;

		if (bind(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
			ErrorHandling("SetupTCPServerSocket() error", "bind() error");

		if (listen(sock, ROOM_SIZE) == SOCKET_ERROR)
			ErrorHandling("SetupTCPServerSocket() error", "listen() error");
	}
	return sock;
}

sock_t AcceptTCPConnection(sock_t serv_sock) {
	sockaddr_storage client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	// Wait for a client to connect
	sock_t client_sock = accept(serv_sock, (sockaddr *)&client_addr, &client_addr_len);
	if (client_sock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	// A client is connected
	fputs("Handling client: ", stdout);
	PrintSocketAddress((sockaddr *)&client_addr);
	fputc('\n', stdout);

	return client_sock;
}