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
     * hKey Long��һ���Ѵ���ľ��������ָ��һ����׼����
     * dwIndex Long������ȡֵ��������ע���һ��ֵ���������Ϊ��
     * lpValueName String������װ��λ��ָ��������ֵ����һ��������
     * lpcbValueName Long������װ��lpValueName���������ȵ�һ��������һ�����أ�������Ϊʵ�����뻺�������ַ�����
     * lpReserved Long��δ�ã���Ϊ��
     * lpType Long������װ��ֵ�����ʹ���ı���
     * lpData Byte������װ��ֵ���ݵ�һ��������
     * lpcbData Long������װ��lpData���������ȵ�һ��������һ�����أ�������Ϊʵ�����뻺�������ַ�����
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

