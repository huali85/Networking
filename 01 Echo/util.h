//
// Created by huali on 17-11-28.
//

#ifndef HELLO_TCPIP_UTIL_H
#define HELLO_TCPIP_UTIL_H

#include <cstdio>

#ifdef WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#ifdef WIN32
typedef SOCKET sock_t;

#define STARTUP() \
WSADATA wsa_data; \
if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) \
	error_handling("WSAStartup() error")
#define CLOSESOCK(sock) closesocket(sock)
#define CLEANUP() WSACleanup()

#else
typedef int sock_t;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define STARTUP()
#define CLOSESOCK(sock) close(sock)
#define CLEANUP()

#endif

const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

#endif //HELLO_TCPIP_UTIL_H
