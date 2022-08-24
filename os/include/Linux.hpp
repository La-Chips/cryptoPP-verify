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
    json serializeMACAddress();

    std::string getType();
    void getCPUInfo();
    
    std::string getMotherboardSerial();
    json serializeMotherboardInfo();


    std::string serialize();
    void addConfigToSign(std::string filename);


    
};

