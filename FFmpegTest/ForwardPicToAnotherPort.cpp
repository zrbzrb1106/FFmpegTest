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

	

	void init_Socket(void) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		
		sock = socket(PF_INET, SOCK_DGRAM, 0);
		memset(&servAddr, 0, sizeof(servAddr));

		servAddr.sin_family = PF_INET;
		inet_pton(PF_INET, "127.0.0.1", &(servAddr.sin_addr));
		servAddr.sin_port = htons(1234);

		addrLen = sizeof(fromAddr);
	}
	
	void sendPicData(AVFrame *pFrame, long int bufSize,
		int width, int height, int iFrame) {

		char *buffer;
		buffer = new char[bufSize];
		//Bytes number every line of picture
		int lineBytes = pFrame->linesize[0];

		memcpy(buffer, pFrame->data[0]+300* lineBytes, lineBytes);

		cout << "modify" << endl;
		// buffer!!!!!!!!!!!!!!!!!!!!!!!!!!!
		/*
		for (int y = 0; y < height; y++) {
			memcpy(&buffer[0 + y*pFrame->linesize[0]], 
				pFrame->data[0+y*pFrame->linesize[0]], pFrame->linesize[0]);
		}
		*/

		cout << pFrame->data[0] << endl;

		//cout << pFrame->data[0] << endl;
		sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
		buffer = {0};
		//cout << "Message form server: " << buffer << endl;
		
		
	}

	void end_Socket() {
		closesocket(sock);
		WSACleanup();
	}

};