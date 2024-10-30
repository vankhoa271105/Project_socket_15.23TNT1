#include "func.h"
bool readfile(SOCKET sock, const char* filename)
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
int connect_and_receive(sockaddr_in srv, const char* ipaddr, string ref, int lenh) {
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
	copy(ref.begin(), ref.end(), sBuff);
	send(nSocket, sBuff, strlen(sBuff), 0);
	memset(sBuff, '\0', sizeof(sBuff));
	nRET = recv(nSocket, sBuff, 1024, 0);
	if (nRET < 0) {
		cout << "cant see respone" << endl;
	}
	else
		cout << "Res from server: " << sBuff << endl;
	//dieu chinh file test
	string x;
	if (lenh == 1 || lenh == 3)
		x = "result.txt";
	if (lenh == 5 || lenh == 2)
		x = "result.png";
	if (lenh == 77)
		x = "shutdown.txt";
	const char* filename = x.c_str();
	readfile(nSocket, filename);
	closesocket(nSocket);
	return 0;
}