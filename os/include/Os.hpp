#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <memory>
#include <stdexcept>
#include <array>

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
    std::string getType();
    void listAllInterfaces();
    int getInterfacesCount();

    int getGPUCount();
    void getGPUInfo();
    void getCPUInfo();
    void getMotherBoardSerial();

};

