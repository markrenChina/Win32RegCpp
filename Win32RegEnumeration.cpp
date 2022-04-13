//
// Created by Ushop on 2022/4/11.
//

#include "Win32RegEnumeration.h"

#include <stdexcept>

Win32RegEnumeration::Win32RegEnumeration(HKEY root,const std::string path)
:root(root),path(path) {

}

std::string Win32RegEnumeration::nextElement() {
    DWORD _maxsize = maxsize;
    /* first time */
    if (index == -1){
        startNameEnumeration();
        index = 0;
    }

    /* already at end */
    if (index >= count){
        throw std::range_error("past end of enumeration");
    }

    char *cret= (char*)malloc(_maxsize);
    /* find the next name
     * hKey Long，一个已打开项的句柄，或者指定一个标准项名
     * dwIndex Long，欲获取值的索引。注意第一个值的索引编号为零
     * lpValueName String，用于装载位于指定索引处值名的一个缓冲区
     * lpcbValueName Long，用于装载lpValueName缓冲区长度的一个变量。一旦返回，它会设为实际载入缓冲区的字符数量
     * lpReserved Long，未用；设为零
     * lpType Long，用于装载值的类型代码的变量
     * lpData Byte，用于装载值数据的一个缓冲区
     * lpcbData Long，用于装载lpData缓冲区长度的一个变量。一旦返回，它会设为实际载入缓冲区的字符数量
     */
    if (RegEnumValue(hkey, index, cret, &_maxsize, nullptr,
                     nullptr, nullptr, nullptr) != ERROR_SUCCESS){
        free(cret);
        RegCloseKey(hkey);
        throw std::runtime_error("Enum value failed");
    }
    std::string ret(cret);
    free(cret);
    /* increment index */
    index++;
    if (index == count) /* at end */
    {
        RegCloseKey(hkey);
    }
    return ret;
}

void Win32RegEnumeration::startNameEnumeration() {
    DWORD _maxsize = 0;
    /* open the registry key */
    if (RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &hkey) != ERROR_SUCCESS){
        throw std::runtime_error("Open key failed");
    }
    /* query count and max length of names */
    if (RegQueryInfoKey(hkey, nullptr, nullptr, nullptr, nullptr, nullptr,
                        nullptr, &count, &_maxsize,
                        nullptr, nullptr, nullptr) != ERROR_SUCCESS){
        RegCloseKey(hkey);
        throw std::runtime_error("Query info key failed");
    }

    maxsize = ++_maxsize;
}

bool Win32RegEnumeration::hasMoreElements() {
    /* first time */
    if (index == -1){
        startNameEnumeration();
        index = 0;
    }
    return index < count;
}

