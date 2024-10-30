#include "func.h"
void ShutDown() {
	const char* command = "powershell -Command \"Stop-Computer -Force\"";
	int result = system(command);
}
void ListApp() {
	const char* command = "powershell -Command \"Get-ItemProperty HKLM:\\Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\* | Select-Object DisplayName, DisplayVersion, Publisher, InstallDate | Format-Table –AutoSize > D:\\ListApp.txt\"";
	int result = system(command);
}
void InitGDIPlus() {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
bool SaveBitmapAsPNG(HBITMAP hBitmap, const WCHAR* filePath) {
	Gdiplus::Bitmap bmp(hBitmap, NULL);
	CLSID pngClsid;
	Gdiplus::Status status = Gdiplus::Ok;
	CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
	status = bmp.Save(filePath, &pngClsid, NULL);
	return status == Gdiplus::Ok;
}
HBITMAP CaptureScreen() {
	HDC hdcScreen = GetDC(NULL);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hdcMemory = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
	SelectObject(hdcMemory, hBitmap);
	BitBlt(hdcMemory, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);
	DeleteDC(hdcMemory);
	ReleaseDC(NULL, hdcScreen);
	return hBitmap;
}
void Screenshot() {
	InitGDIPlus();
	HBITMAP hBitmap = CaptureScreen();
	if (SaveBitmapAsPNG(hBitmap, L"D:\\chaymanhinh2.png")) {
		cout << "Screenshot saved successfully!" << endl;
	}
	else {
		cout << "Failed to save screenshot." << endl;
	}
	DeleteObject(hBitmap);
}
void ListService() {
	string command = "powershell -Command \"Get-Service | Out-File -FilePath D:/ListService2.txt\"";
	int result = system(command.c_str());
}
void Sleep() {
	string command = "powershell -command \"Start-Sleep 1; Add-Type -AssemblyName System.Windows.Forms; [System.Windows.Forms.Application]::SetSuspendState('Suspend', $false, $false)\"";
	int result = system(command.c_str());
}
void StartApp(const string& app) {
	string command = "powershell -command \"Start-Process '" + app + "'\"";
	int result = system(command.c_str());
}