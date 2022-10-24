#pragma once


#include "Os.hpp"

class Linux : public Os
{
private:

    protected:
    std::string getMACAddress(int index);

public:
    Linux() ;
    json serializeMACAddress();

    std::string getType();
    
    std::string getMotherboardSerial();
    json serializeMotherboardInfo();


    std::string serialize();
    void addConfigToSign(std::string filename);


    
};

