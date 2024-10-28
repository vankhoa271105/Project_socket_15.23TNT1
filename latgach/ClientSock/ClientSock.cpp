#include "func.h"
int main()
{
    ::CoInitialize(NULL);
    printf(
        "+------------------------------------------------------------------+\n"
        "  Sign in with Google                                             \n"
        "   If you got \"This app isn't verified\" information in Web Browser, \n"
        "   click \"Advanced\" -> Go to ... to continue test.\n"
        "+------------------------------------------------------------------+\n\n");

    printf("Press ENTER key to sign in...\n");
    getchar();
	string maildich;
	bool temp = true;
	pair <string, string> request;
	while (1) {
		DoOauthAndRetrieveEmail("12345", maildich, temp, request);
		if (temp)
			temp = !temp;
		struct sockaddr_in srv;
		WSADATA ws;
		int nRET = 0;

		if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
			cout << "failed" << endl;
		}
		else
			cout << "ok" << endl;
		srv.sin_family = AF_INET;
		srv.sin_addr.s_addr = inet_addr(request.second.c_str());
		srv.sin_port = htons(PORT);
		memset(&(srv.sin_zero), 0, 8);
		int lenh = 0;
		pair <string, string> command = tokenize(request.first, ":");
		if (command.first == "Welcome")
			lenh = 0;
		else if (command.first == "ListApp")
			lenh = 1;
		else if (command.first == "StartApp")
			lenh = 2;
		else if (command.first == "StopApp")
			lenh = 22;
		else if (command.first == "ListService")
			lenh = 3;
		else if (command.first == "StartService")
			lenh = 4;
		else if (command.first == "StopService")
			lenh = 44;
		else if (command.first == "ScreenShot")
			lenh = 5;
		else if (command.first == "StartCam")
			lenh = 6;
		else if (command.first == "StopCam")
			lenh = 66;
		else if (command.first == "Shutdown")
			lenh = 7;
		else if (command.first == "Sleep")
			lenh = 77;
		else if (command.first == "TakeFile")
			lenh = 8;
		else if (command.first == "CopyFile")
			lenh = 9;
		else if (command.first == "MoveFile")
			lenh = 99;
		cout << "Request:" << command.first << "; Code:" << lenh << "; IP:" << request.second.c_str() << endl;
		command.second += ":" + to_string(lenh);
		int t = connect_and_receive(srv, request.second.c_str(), command.second, lenh);
		cout << maildich << ": mail nhan" << endl;
		DoOauthAndSendEmail(maildich.c_str(), lenh);
	}
}


