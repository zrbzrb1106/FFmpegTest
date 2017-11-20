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
		// every frame stored in a buffer
		for (int y = 0; y < height; y++) {
			memcpy(buffer + y*lineBytes, 
				pFrame->data[0]+y*lineBytes, lineBytes);
		}

		for (int i = 0; i < bufSize; i++) {
			cout << *(buffer + i);
		}


		sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
		//release the buffer
		buffer = {};
		//cout << "Message form server: " << buffer << endl;
		
		
	}

	void end_Socket() {
		closesocket(sock);
		WSACleanup();
	}

};