#pragma once
#ifndef _func_H_
#define _func_H_
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <comutil.h>
#include <vector>
#define BUFFER_SIZE 1024
#define PORT 9909
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
DWORD  _getCurrentPath(LPTSTR lpPath, DWORD nSize);
const char* TCHARToChar(const TCHAR* tcharStr);
bool RetrieveWithXOAUTH2(const char* lpszEmail, const char* lpszAccessToken, const char* password, string &revmail, pair<string, string>& request);
_bstr_t ParseParameter(const char* uri, const char* key);
_bstr_t RequestAccessToken(const char* code, const char* redirectUri);
void DoOauthAndRetrieveEmail(const char* password, string& revmail, bool first, pair<string, string>& request);
void SendMailWithXOAUTH2(const char* lpszEmail, const char* lpszAccessToken, const char* endmail, int lenh = 0);
_bstr_t RequestAccessToken_send(const char* code, const char* redirectUri);
void DoOauthAndSendEmail(const char* endmail, int lenh = 0);
int connect_and_receive(sockaddr_in srv, const char* ipaddr, string ref, int lenh = 0);
pair<string, string> tokenize(string s, string del = " ");
#endif