#include "func.h"
void ShutDown() {
    const char* command = "powershell -Command \"Stop-Computer -Force\"";
    int result = system(command);
}