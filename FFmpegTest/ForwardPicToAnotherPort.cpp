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

	
	// initializing socket
	void init_Socket(void) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//
		memset(&servAddr, 0, sizeof(servAddr));

		servAddr.sin_family = PF_INET;
		inet_pton(PF_INET, "127.0.0.1", &(servAddr.sin_addr));
		servAddr.sin_port = htons(8888);

		addrLen = sizeof(fromAddr);
		connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));//
	}
	// send frame to another port
	void sendPicData(AVFrame *pFrame, long int bufSize,
		int width, int height, int iFrame) {

		char *buffer;
		//Bytes number every line of picture
		int lineBytes = pFrame->linesize[0];
		buffer = new char[lineBytes];

		// send every line
		for (int y = 0; y < height; y++) {
			memcpy(buffer, 
				pFrame->data[0]+y*lineBytes, lineBytes);

			sendto(sock, buffer, lineBytes, 0, 
				(struct sockaddr*)&servAddr, sizeof(servAddr));
			cout << y << " nd line" << endl;
		}
		
	}

	void end_Socket() {
		closesocket(sock);
		WSACleanup();
	}

};