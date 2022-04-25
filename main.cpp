//
// Created by Ushop on 2022/4/11.
//
#include "Win32RegKey.h"
#include <iostream>
#include <memory>


int main(){

    Win32RegKey::Ptr key = std::make_shared<Win32RegKey>(HKEY_LOCAL_MACHINE,"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
    /*key->setValue("custom","markren china");
    std::string value = key->getStringValue("custom");
    std::cout << value << std::endl;

    key->setValue("custom2",1111222);
    int32_t value2 = key->getInt32Value("custom2");
    std::cout << value2 << std::endl;*/

    Win32RegEnumeration::Ptr e = key->names();
    while (e->hasMoreElements()){
        std::string name;
        try{
            name = e->nextElement();
        }catch (std::runtime_error error){
            std::cerr << error.what();
            break;
        }
        DWORD type;
        DWORD size;
        std::unique_ptr<BYTE[]> value3 = key->getValue(name,&type,&size);
        std::cout << name << " : " ;
        switch (type) {
            case REG_SZ: std::cout << value3; break;
            case REG_DWORD: std::cout << *(int*)(&value3[0]); break;
            case REG_BINARY:
                for (int i = 0; i < size; i++) {
                    std::cout << (value3[i] & 0xFFFFFFFF) << " ";
                } ;break;
            default:;
        }
        std::cout << std::endl;
        //free(value3);
    }

    return 0;
}