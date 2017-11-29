
#ifndef HELLO_TCPIP_UTIL_H
#define HELLO_TCPIP_UTIL_H

#include <cstdio>

#ifdef WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#ifdef WIN32
typedef SOCKET sock_t;
typedef USHORT port_t;

#define STARTUP() \
WSADATA wsa_data; \
if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) \
	error_handling("WSAStartup() error")
#define CLOSESOCK(sock) closesocket(sock)
#define CLEANUP() WSACleanup()

#else
typedef int sock_t;
typedef in_port_t port_t;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define STARTUP()
#define CLOSESOCK(sock) close(sock)
#define CLEANUP()

#endif

const int BUF_SIZE = 1024;
const int ROOM_SIZE = 5;

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void print_socket_address(const sockaddr *address) {
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

sock_t setup_tcp_client(const char *host, const char *service) {
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *serv_addr;
    int result = getaddrinfo(host, service, &hints, &serv_addr);
    if (result != 0) {
        error_handling("getaddrinfo() error");
    }

    sock_t sock = INVALID_SOCKET;
    for (addrinfo *addr = serv_addr; addr != nullptr; addr = addr->ai_next) {
        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            continue;   // Socket creation failed, try next address
        }

        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
            break;  // Socket connection succeeded, break and return socket
        }

        CLOSESOCK(sock);    // Socket connection failed, try next address
        sock = INVALID_SOCKET;
    }

    freeaddrinfo(serv_addr);
    return sock;
}

sock_t setup_tcp_server(const char *service) {
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *serv_addr;
    int result = getaddrinfo(nullptr, service, &hints, &serv_addr);
    if (result != 0) {
        error_handling("getaddrinfo() error");
    }

    sock_t sock = INVALID_SOCKET;
    for (addrinfo *addr = serv_addr; addr != nullptr; addr = addr->ai_next) {
        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            continue;   // Socket creation failed, try next address
        }

        if ((bind(sock, addr->ai_addr, addr->ai_addrlen) == 0) &&
            (listen(sock, ROOM_SIZE) == 0)) {
            break;  // Bind and listen successful
        }

        CLOSESOCK(sock);    // Try next address
        sock = INVALID_SOCKET;
    }

    freeaddrinfo(serv_addr);
    return sock;
}

sock_t accept_tcp_connection(sock_t serv_sock) {
    sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Wait for a client to connect
    sock_t client_sock = accept(serv_sock, (sockaddr *)&client_addr, &client_addr_len);
    if (client_sock == INVALID_SOCKET) {
        error_handling("accept() error");
    }

    // A client is connected
    fputs("Handling client: ", stdout);
    print_socket_address((sockaddr *)&client_addr);
    fputc('\n', stdout);

    return client_sock;
}

#endif //HELLO_TCPIP_UTIL_H
