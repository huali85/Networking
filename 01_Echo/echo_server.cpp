
#include <cstdlib>
#include "util.h"

void handle_client(sock_t client_sock);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    STARTUP();

    sock_t serv_sock = setup_tcp_server(argv[1]);
    if (serv_sock == INVALID_SOCKET) {
        error_handling("socket() error");
    }

    while(1) {
        sock_t client_sock = accept_tcp_connection(serv_sock);

        handle_client(client_sock);
    }

    CLOSESOCK(serv_sock);
    CLEANUP();
    return 0;
}

void handle_client(sock_t client_sock) {
    char buffer[BUF_SIZE];
    // Receive message from client
    int recv_len = recv(client_sock, buffer, BUF_SIZE, 0);
    if (recv_len < 0) {
        error_handling("recv() error");
    }
    // Send received message
    while (recv_len > 0) {
        int send_len = send(client_sock, buffer, recv_len, 0);
        if (send_len < 0) {
            error_handling("send() error");
        }
        else if (send_len != recv_len) {
            error_handling("send unexpected number of bytes");
        }

        recv_len = recv(client_sock, buffer, BUF_SIZE, 0);
        if (recv_len < 0) {
            error_handling("recv() error");
        }
    }

    CLOSESOCK(client_sock);
}
