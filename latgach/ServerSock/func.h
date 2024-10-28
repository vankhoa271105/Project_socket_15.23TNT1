#pragma once
#ifndef _func_H_
#define _func_H_
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace std;
#pragma comment (lib,"Gdiplus.lib")
#define PORT 9909
#define BUFFER_SIZE 1024
#pragma comment(lib, "Ws2_32.lib")
void ShutDown();
void ListApp();
void InitGDIPlus();
bool SaveBitmapAsPNG(HBITMAP hBitmap, const WCHAR* filePath);
HBITMAP CaptureScreen();
void Screenshot();
#endif
