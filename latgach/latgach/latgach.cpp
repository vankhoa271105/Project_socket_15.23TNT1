// latgach.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <string>
#define BUFFER_SIZE 1024
#define PORT 9909
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
//bool readdata(SOCKET sock, void* buf, int buflen)
//{
//    char* pbuf = (char*)buf;
//
//    while (buflen > 0)
//    {
//        int num = recv(sock, pbuf, buflen, 0);
//        if (num == SOCKET_ERROR)
//        {
//            if (WSAGetLastError() == WSAEWOULDBLOCK)
//            {
//                // optional: use select() to check for timeout to fail the read
//                continue;
//            }
//            return false;
//        }
//        else if (num == 0)
//            return false;
//
//        pbuf += num;
//        buflen -= num;
//    }
//
//    return true;
//}
//
//bool readlong(SOCKET sock, long* value)
//{
//    if (!readdata(sock, value, sizeof(value)))
//        return false;
//    *value = ntohl(*value);
//    return true;
//}

bool readfile(SOCKET sock, const char *filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        cout << "Failed to open file.\n";
    }

    // Receive file in chunks
    char buffer[BUFFER_SIZE];
	int bytesReceived;
	while ((bytesReceived = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
		file.write(buffer, bytesReceived);
	}

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Receive failed: " << WSAGetLastError() << "\n";
		return false;
    }

    // Close file
    file.close();
	return true;
}
int connect_and_receive(sockaddr_in srv, const char*ipaddr, int lenh = 0) {
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0) {
		cout << "not open";
		return -1;
	}
	else {
		cout << "the socket open success" << nSocket << endl;
	}
	//u_long iMode = 1;  // 1 to enable non-blocking mode
	//ioctlsocket(nSocket, FIONBIO, &iMode);
	//ini evnvironment
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = inet_addr(ipaddr);
	srv.sin_port = htons(PORT);
	memset(&(srv.sin_zero), 0, 8);
	int nRET = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRET < 0) {
		cout << "cannot connet to server" << errno;
		exit(EXIT_FAILURE);
		return -1;
	}
	else cout << "ok" << endl;
	char sBuff[1024] = { 0, };
		Sleep(2000);
		cout << "what mess u want to send: ";
		cin.getline(sBuff, 1023);
		send(nSocket, sBuff, strlen(sBuff), 0);
		memset(sBuff, '\0', sizeof(sBuff));
		nRET = recv(nSocket, sBuff, 1024, 0);
		if (nRET < 0) {
			cout << "cant see respone" << endl;
		}
		else
			cout << "Res from server: " << sBuff << endl;
		string x = "Anhtest" + to_string(lenh) + ".png";
		const char* filename = x.c_str();
		readfile(nSocket, filename);
		closesocket(nSocket);
		return 0;
}
int main() {
	struct sockaddr_in srv;
	WSADATA ws;
	int nRET = 0;

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		cout << "failed" << endl;
	}
	else
		cout << "ok" << endl;
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = inet_addr(" 192.168.229.198");
	srv.sin_port = htons(PORT);
	memset(&(srv.sin_zero), 0, 8);
	int lenh = 0;
	while (1) {
		
		int t = connect_and_receive(srv, " 192.168.229.198", lenh);
		if (t == -1)
			break;
		lenh++;
	}
	return 0;
}

