#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define TOKEN_LEN 32

#pragma comment(lib, "ws2_32.lib") // Winsock Library

const char* gen_token()
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	static char tmp_s[TOKEN_LEN] = "";

	srand(time(0));

	tmp_s[0] = alphanum[rand() % (sizeof(alphanum) - 1)];

	for (int i = 0; i < TOKEN_LEN; ++i)
	{
		strncat(tmp_s, &alphanum[rand() % (sizeof(alphanum) - 1)], 1);
	}

	tmp_s[TOKEN_LEN] = '\0';
	
	//printf("token: %s\r\n", tmp_s);

	return tmp_s;
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char *token = NULL;

	char *ip = strtok(argv[2], ":");
	printf("server ip: %s\n", ip);

	char *port = strtok(NULL, "");
	printf("server port: %s\n", port);

	//gen_token(TOKEN_LEN);

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	// Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip); // INADDR_ANY;
	server.sin_port = htons(atoi(port));	// htons( 8080 );

	// Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	// Listen to incoming connections
	listen(s, 3);

	// Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);

	do
	{
		new_socket = accept(s, (struct sockaddr *)&client, &c);
		if (new_socket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d", WSAGetLastError());
		}
		else
		{
			puts("Connection accepted");

			token = gen_token();
			printf("%s\n", token);

			send(new_socket, token, strlen(token) , 0);
		}

	} while (new_socket != INVALID_SOCKET);

	closesocket(s);
	WSACleanup();

	return 0;
}
