#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")  //load ws2_32.dll

using namespace std;

class ForwardPicToAnotherPort {

public:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in servAddr;
	sockaddr fromAddr;
	int addrLen;
	char *buffer = nullptr;

	// initializing socket, using TCP
	void init_Socket(void) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(&servAddr, 0, sizeof(servAddr));

		servAddr.sin_family = PF_INET;
		inet_pton(PF_INET, "127.0.0.1", &(servAddr.sin_addr));
		servAddr.sin_port = htons(8888);

		addrLen = sizeof(fromAddr);
		connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));//
		buffer = new char[2764800];
		
	}
	// send frame to another port
	void sendPicData(AVFrame *pFrame, long int bufSize,
		int width, int height, int iFrame) {
		
		memcpy(buffer, pFrame->data[0], bufSize);
		sendto(sock, buffer, bufSize, 0,
			(struct sockaddr*)&servAddr, sizeof(servAddr));
		
	}
	// close socket
	void end_Socket() {
		closesocket(sock);
		WSACleanup();
	}

};