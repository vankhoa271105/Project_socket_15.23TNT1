#include "func.h"


bool sendfile(SOCKET sock, const char* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		cout << "Failed to open file.\n";
	}

	// Get file size
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// Send file in chunks
	char buffer[BUFFER_SIZE];
	while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
		int bytesSent = send(sock, buffer, file.gcount(), 0);
		if (bytesSent == SOCKET_ERROR) {
			std::cerr << "Send failed: " << WSAGetLastError() << "\n";
			return false;
		}
	}

	// Close the file
	file.close();
	return true;
}
//send file
int connet_and_send_file(sockaddr_in srv, int lenh=0) {
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0) {
		cout << "not open";
	}
	else {
		cout << "the socket open success" << nSocket << endl;
	}
	//ini evnvironment
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&(srv.sin_zero), 0, 8);
	//bind
	int nRET = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRET < 0) {
		cout << "fail to bind to local port" << endl;
		exit(EXIT_FAILURE);
		return -1;
	}
	else cout << "success to bind to local port" << endl;
	nRET = listen(nSocket, 5);
	if (nRET < 0) {
		cout << "fail to listen to client" << endl;
		exit(EXIT_FAILURE);
		return -1;
	}
	else
		cout << "started listen" << endl;
	int nClient = 0;
	int addrlen = sizeof(srv);
	nClient = accept(nSocket, (struct sockaddr*)&srv, &addrlen);
	if (nClient < 0) {
		cout << "Cannot accept client at port" << errno;
		exit(EXIT_FAILURE);
		return -1;
	}
	else cout << "Accept client at port" << nClient << endl;
	char sBuff[1024] = { 0, };
	string fileSend = "";
		memset(sBuff, '\0', sizeof(sBuff));
		nRET = recv(nClient, sBuff, 1024, 0);
		if (nRET < 0) {
			cout << "No  new message";
			return 0;
		}
		cout << "\nRecv from client:\n" << sBuff << endl;
		int length = strlen(sBuff);
		//thong so lenh
		if (sBuff[length - 1] == '1') {
			ListApp();
			fileSend = "D:\\ListApp.txt";
		}
		else if (sBuff[length - 1] == '5') {
			Screenshot();
			fileSend = "D:\\chaymanhinh2.png";
		}
		else if (sBuff[length - 1] == '7') {
			if (sBuff[length - 2] == '7')
				Sleep();
			else 
				ShutDown();
			fileSend = "shutdown.txt";
		}
		else if (sBuff[length - 1] == '3') {
			ListService();
			fileSend = "D:\\ListService2.txt";
		}
		else if (sBuff[length - 1] == '2') {
			string app = "";
			for (int i = 0; i < length - 2; i++)
				app += sBuff[i];
			app += ".exe";
			StartApp((const string)app);
			Sleep(1000);
			Screenshot();
			fileSend = "D:\\chaymanhinh2.png";
		}
		int nSend = send(nClient, "ok", strlen(sBuff), 0);
		if (nSend < 0) {
			cout << "cant respone" << endl;
		}
		Sleep(1000);
		//dieu chinh file gui
		const char* filehandle = fileSend.c_str();
		sendfile(nClient, filehandle);
		closesocket(nClient);
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
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&(srv.sin_zero), 0, 8);
	while (1) {
		int t = connet_and_send_file(srv, 0);
		if (t < 0)
			break;
		WSACleanup();
		if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
			cout << "failed" << endl;
			break;
		}
		else
			cout << "ok" << endl;
	}
	return 0;
}
