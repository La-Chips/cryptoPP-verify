#include "../include/Linux.hpp"


#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

namespace fs = std::filesystem;

Linux::Linux(){
    std::string path = "/sys/class/net";
    for (const auto &entry : fs::directory_iterator(path)){
                this->interfaces.push_back(entry.path());
    }
}




std::string Linux::getMACAddress(int index)
{
    std::string dir = this->interfaces[index -1];
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

std::string Linux::getType(){
    return "Linux";
}

inline void Linux::native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
        /* ecx is often an input as well as an output. */
        __asm__ __volatile__ ("cpuid": "=a" (*eax),"=b" (*ebx),"=c" (*ecx),"=d" (*edx): "0" (*eax), "2" (*ecx));
}

void Linux::getMotherBoardSerial(){
    std::string result = this->exec("dmidecode --type baseboard | grep -i serial");


    int index  = result.find(':');

    result = result.substr(index +2);


    std::cout << result << std::endl;
}

void Linux::getCPUInfo(){
    unsigned eax, ebx, ecx, edx;

  eax = 1; /* processor info and feature bits */
  native_cpuid(&eax, &ebx, &ecx, &edx);

  printf("stepping %d\n", eax & 0xF);
  printf("model %d\n", (eax >> 4) & 0xF);
  printf("family %d\n", (eax >> 8) & 0xF);
  printf("processor type %d\n", (eax >> 12) & 0x3);
  printf("extended model %d\n", (eax >> 16) & 0xF);
  printf("extended family %d\n", (eax >> 20) & 0xFF);

  /* EDIT */
  eax = 3; /* processor serial number */
  native_cpuid(&eax, &ebx, &ecx, &edx);

}