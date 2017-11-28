//
// Created by huali on 17-11-28.
//

#include <cstdlib>
#include <cstring>
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
    }

	STARTUP();

    sock_t sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        error_handling("socket() error");
    }

    sockaddr_in serv_addr = {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        error_handling("connect() error");
    }
    else {
        puts("Connected......");
    }

    char message[BUF_SIZE];
    while (1) {
        fputs("Message to send(q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n")) {
            break;
        }

        int str_len = send(sock, message, strlen(message), 0);
        int recv_len = 0;
        while (recv_len < str_len) {
            int recv_count = recv(sock, &message[recv_len], BUF_SIZE - 1, 0);
            if (recv_count == SOCKET_ERROR) {
                error_handling("recv() error");
            }
            recv_len += recv_count;
        }
        message[recv_len] = 0;
        printf("Message from server: %s", message);
    }

    CLOSESOCK(sock);
	CLEANUP();
    return 0;
}