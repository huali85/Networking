//
// Created by huali on 17-11-28.
//

#include <cstdlib>
#include "util.h"

const int ROOM_SIZE = 5;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    STARTUP();

    sock_t serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET) {
        error_handling("socket() error");
    }

    sockaddr_in serv_addr = {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, ROOM_SIZE) == -1) {
        error_handling("listen() error");
    }

    char buffer[BUF_SIZE];
    sockaddr_in client_addr = {};
    socklen_t client_addr_size = sizeof(client_addr);

    for (int i = 0; i < ROOM_SIZE; ++i) {
        sock_t client_sock = accept(serv_sock, (sockaddr*)&client_addr, &client_addr_size);
        if (client_sock == INVALID_SOCKET) {
            error_handling("accept() error");
        }
        else {
            printf("Connected client %d\n", i + 1);
        }

        int str_len;
        while ((str_len = recv(client_sock, buffer, BUF_SIZE, 0)) != 0) {
            printf("received message from client %d: %s", i + 1, buffer);
            send(client_sock, buffer, str_len, 0);
        }

        CLOSESOCK(client_sock);
    }

    CLOSESOCK(serv_sock);
    CLEANUP();
    return 0;
}
