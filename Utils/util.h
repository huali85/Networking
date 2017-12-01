
#ifndef NETWORK_UTIL_H
#define NETWORK_UTIL_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
	ErrorHandling("WSAStartup() error")
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

#define BUF_SIZE	512
#define ROOM_SIZE	5

void ErrorHandling(const char *msg);
void ErrorHandling(const char *msg, const char *detail);

void PrintSocketAddress(const sockaddr *address);

sock_t SetupTCPClientSocket(const char *host, const char *service);
sock_t SetupTCPServerSocket(uint16_t port);

sock_t AcceptTCPConnection(sock_t serv_sock);

#endif // NETWORK_UTIL_H
