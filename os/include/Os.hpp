#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <memory>
#include <stdexcept>
#include <array>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Os
{
private:

    
protected:
    std::vector<std::string> interfaces ;
    std::string getMACAddress(int index);
    std::string exec(const char* cmd);



public:
    Os(){};

    json serializeMACAddress();

    std::string getType();
    void listAllInterfaces();
    int getInterfacesCount();

    int getGPUCount();
    json getGPUInfo();


    std::string getMotherboardSerial();
    json serializeMotherboardInfo();

    std::string serialize();
    void addConfigToSign(std::string filename);

};

