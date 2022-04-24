#include "Win32RegKey.h"

#include <stdexcept>
#include <utility>
#include <memory>

Win32RegKey::Win32RegKey(const HKEY root, std::string path) : root(root), path(std::move(path)) {}

//字符串类型
void Win32RegKey::setValue(const std::string& name,const std::string& value) const {
    setValue(name,REG_SZ,reinterpret_cast<const BYTE *>(value.c_str()),(value.size()+1));
}

void Win32RegKey::setValue(const std::string& name, BYTE *value,DWORD size) const {
    setValue(name,REG_BINARY,value,size);
}

//整型，4位内存
void Win32RegKey::setValue(const std::string& name, DWORD value) const {
    setValue(name,REG_DWORD,reinterpret_cast<const BYTE *>(&value),4);
}

void Win32RegKey::setValue(const std::string& name,DWORD type, const BYTE *value,DWORD size) const {
    HKEY hkey;
    /* open or create the registry key */
    if (RegCreateKeyExA(root, path.c_str(), 0,nullptr, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, nullptr,&hkey,nullptr) != ERROR_SUCCESS){
        throw std::runtime_error("Open key failed");
    }

    /* set the value */
    if (RegSetValueExA(hkey, name.c_str(), 0, type, value, size) != ERROR_SUCCESS){
        RegCloseKey(hkey);
        throw std::runtime_error("Set value failed");
    }
    RegCloseKey(hkey);
}

std::unique_ptr<BYTE[]> Win32RegKey::getValue(const std::string& name, LPDWORD type,LPDWORD size) const {
    HKEY hkey;
    /* open the registry key
     * LONG RegOpenKeyEx(
     * HKEY hKey, // 需要打开的主键的名称
     * LPCTSTR lpSubKey, //需要打开的子键的名称
     * DWORD ulOptions, // 保留，设为0
     * REGSAM samDesired, // 安全访问标记，也就是权限
     * PHKEY phkResult // 得到的将要打开键的句柄
     * )
     */
    if (RegOpenKeyExA(root, path.c_str(), 0, KEY_READ, &hkey) != ERROR_SUCCESS){
        throw std::runtime_error("open key failed");
    }
    /* find the type and size of the value
     * LONG WINAPI RegQueryValueEx(
     * HKEY hKey,            // handle to key
     * LPCTSTR lpValueName,  // value name
     * LPDWORD lpReserved,   // reserved
     * LPDWORD lpType,       // type buffer
     * LPBYTE lpData,        // data buffer
     * LPDWORD lpcbData      // size of data buffer
     * )
     */
    if (RegQueryValueExA(hkey, name.c_str(), nullptr, type, nullptr, size) != ERROR_SUCCESS){
        RegCloseKey(hkey);
        throw std::runtime_error("Query value key failed");
    }
    /* get memory to hold the value */
    std::unique_ptr<BYTE[]> cret(new BYTE[*size]);
    //BYTE* cret = (BYTE*)malloc(*size);

    /* read the value */
    if (RegQueryValueExA(hkey, name.c_str(), nullptr, type, &cret[0], size) != ERROR_SUCCESS){
        //free(cret);
        RegCloseKey(hkey);
        throw std::runtime_error("Query value key failed");
    }
    return cret;
}

std::unique_ptr<BYTE[]> Win32RegKey::getValue(const std::string &name, DWORD type, DWORD size) const {
    HKEY hkey;
    /* open the registry key */
    if (RegOpenKeyExA(root, path.c_str(), 0, KEY_READ, &hkey) != ERROR_SUCCESS){
        throw std::runtime_error("open key failed");
    }
    /* find the type and size of the value */
//    if (RegQueryValueExA(hkey, name.c_str(), nullptr, type, nullptr, &size) != ERROR_SUCCESS){
//        RegCloseKey(hkey);
//        throw std::runtime_error("Query value key failed");
//    }
    /* get memory to hold the value */
    std::unique_ptr<BYTE[]> cret(new BYTE[size]);
    //BYTE* cret = (BYTE*)malloc(size);

    /* read the value */
    if (RegQueryValueExA(hkey, name.c_str(), nullptr, &type, &cret[0], &size) != ERROR_SUCCESS){
        //free(cret);
        RegCloseKey(hkey);
        throw std::runtime_error("Query value key failed");
    }
    return cret;
}


Win32RegEnumeration* Win32RegKey::names() {
    if (enumeration == nullptr){
        enumeration = new Win32RegEnumeration(root,path);
    }
    return enumeration;
}

Win32RegKey::~Win32RegKey() {
    delete enumeration;
}

std::string Win32RegKey::getStringValue(const std::string &name) const {
    DWORD type = REG_SZ;
    DWORD size;
    std::unique_ptr<BYTE[]> value = getValue(name,&type, &size);
    std::string res = reinterpret_cast<const char *>(&value[0]);
    return res;
}

std::int32_t Win32RegKey::getInt32Value(const std::string &name) const {
    DWORD type = REG_DWORD;
    std::unique_ptr<BYTE[]> value = getValue(name,type, 4);
    int32_t res = *(int32_t *) (&value[0]);
    //free(value);
    return res;
}



