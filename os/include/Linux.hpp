#pragma once


#include "Os.hpp"

class Linux : public Os
{
private:
    inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx);
public:
    Linux() ;
    std::string getMACAddress(int index);
    std::string getType();
    void getCPUInfo();
    void getMotherBoardSerial();


    
};

