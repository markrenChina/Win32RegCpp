//
// Created by markrenChina on 2022/4/11.
//

#ifndef WIN32REGCPP_WIN32REGENUMERATION_H
#define WIN32REGCPP_WIN32REGENUMERATION_H

#include <string>
#include <windows.h>


/**
 * 具体某个注册表路径下的k，v对象集合的迭代器
 */
class Win32RegEnumeration {
public:
    Win32RegEnumeration(HKEY root,std::string path);
    ~Win32RegEnumeration() = default;
    std::string nextElement();
    bool hasMoreElements();

private:
    void startNameEnumeration();

    HKEY root;
    std::string path;
    int index = -1;
    HKEY hkey = nullptr;
    DWORD maxsize = 0;
    DWORD count = 0;
};


#endif //WIN32REGCPP_WIN32REGENUMERATION_H
