#include <stdio.h>
//#include <winsock2.h>
#include "tlib.h"

//#pragma comment(lib, "ws2_32.lib") // Winsock Library

int main(int argc, char *argv[])
{
	request_token(argv[1], argv[2]);

	return 0;
}