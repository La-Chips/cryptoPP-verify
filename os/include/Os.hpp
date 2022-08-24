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
    /* data */
protected:
    std::vector<std::string> interfaces ;

public:
    Os(){};
    std::string exec(const char* cmd);

    std::string getMACAddress(int index);
    json serializeMACAddress();

    std::string getType();
    void listAllInterfaces();
    int getInterfacesCount();

    int getGPUCount();
    json getGPUInfo();
    void getCPUInfo();


    std::string getMotherboardSerial();
    json serializeMotherboardInfo();

    std::string serialize();
    void addConfigToSign(std::string filename);

};

