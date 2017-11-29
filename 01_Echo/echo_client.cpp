
#include <cstdlib>
#include <cstring>
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <IP/name> <port/service>\n", argv[0]);
        exit(1);
    }

	STARTUP();

    const char *server = argv[1];
    const char *service = (argc == 3) ? argv[2] : "echo";
    sock_t sock = setup_tcp_client(server, service);
    if (sock == INVALID_SOCKET) {
        error_handling("setup_tcp_client() error");
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