#ifndef WIN32REGCPP_WIN32REGKEY_H
#define WIN32REGCPP_WIN32REGKEY_H

#include <string>
#include <memory>
#include <Windows.h>

#include "Win32RegEnumeration.h"

/**
 * 具体某个注册表路径下的k，v对象集合
 */
class Win32RegKey {
public:
    using Ptr = std::shared_ptr<Win32RegKey>;
    Win32RegKey(HKEY root, std::string path);
    virtual ~Win32RegKey() { enumeration.reset(nullptr); };

    std::string getStringValue(const std::string& name) const;
    std::int32_t getInt32Value(const std::string& name) const;
    std::unique_ptr<BYTE[]> getValue(_In_ const std::string& name,_Out_ LPDWORD type,_Out_ LPDWORD size) const;
    std::unique_ptr<BYTE[]> getValue(_In_ const std::string& name,_Out_ DWORD type,DWORD size) const;

    void setValue(const std::string& name,const std::string& value) const;
    void setValue(const std::string& name,DWORD value) const;
    void setValue(_In_ const std::string& name,_In_ BYTE * value,_In_ DWORD size) const;
    void setValue(const std::string& name,DWORD type,const BYTE *value,DWORD size) const;

    Win32RegEnumeration::Ptr& names();
private:
    Win32RegEnumeration::Ptr enumeration = nullptr;
    HKEY root;
    std::string path;
};

#endif //WIN32REGCPP_WIN32REGKEY_H
