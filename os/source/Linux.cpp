#include "../include/Linux.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

namespace fs = std::filesystem;

Linux::Linux()
{
    std::string path = "/sys/class/net";
    for (const auto &entry : fs::directory_iterator(path))
    {
        std::string tmp = entry.path();
        if (tmp.find("/lo") == std::string::npos)
        {
            this->interfaces.push_back(entry.path());
        }
    }
}

std::string Linux::serialize()
{
    json values = {};

    values.push_back(this->serializeMACAddress());
    values.push_back(this->serializeMotherboardInfo());
    values.push_back(this->getGPUInfo());

    return values.dump();
}

std::string Linux::getMACAddress(int index)
{
    std::string dir = this->interfaces[index];
    if (fs::is_directory(fs::status(dir)))
    {
        std::string path = dir + "/address";

        if (fs::is_regular_file(fs::status(path)))
        {
            std::string addr;

            std::ifstream MyReadFile(path);

            getline(MyReadFile, addr);

            // Close the file
            MyReadFile.close();
            return addr;
        }
    }

    return "";
};

json Linux::serializeMACAddress()
{
    json value = {};

    for (size_t i = 0; i < this->interfaces.size(); i++)
    {
        value.push_back({this->interfaces[i], this->getMACAddress(i)});
    }

    value = {
        {"Interface", value}};
    return value;
}

std::string Linux::getType()
{
    return "Linux";
}


std::string Linux::getMotherboardSerial()
{
    std::string result = this->exec("dmidecode --type baseboard | grep -i serial");

    int index = result.find(':');

    result = result.substr(index + 2);

    return result;
}

json Linux::serializeMotherboardInfo()
{
    std::string serial = this->getMotherboardSerial();

    json value = {
        {"Motherboard", {{"Serial", serial}}}};
    return value;
}



void Linux::addConfigToSign(std::string filename){


    std::ifstream ifs(filename.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );


    auto end = content.find("json");
  if(end != std::string::npos){
    content = content.substr(0,end);
  }

    content += "json";
    content += this->serialize(); 

std::ofstream MyFile(filename);

  MyFile << content;

  MyFile.close();
}