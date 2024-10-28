#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace std;
pair<string, string> tokenize(string s, string del)
{
    vector<string> alo;
    int start, end = -1 * del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        alo.push_back(s.substr(start, end - start));
    } while (end != -1);
    return make_pair(alo[0], alo[1]);
}
void ListApp() {
    char gg[1024] = "Minhcute`1";
    int length = strlen(gg);
    cout << gg[length - 1] << endl;
    if (gg[length - 1] == '1') {
        cout << "i am listing app" << endl;
    }

    const char* command = "powershell -Command \"Get-ItemProperty HKLM:\\Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\* | Select-Object DisplayName, DisplayVersion, Publisher, InstallDate | Format-Table –AutoSize > D:\\ListApp.txt\"";
    int result = system(command);
}
int main() {
    ListApp();
}