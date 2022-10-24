//
//  main.cpp
//  snellium-licence
//
//  Created by Clement Hellot on 18/08/2022.
//
#include "./os/include/Linux.hpp"
#include "./tools/include/OpenSSL.hpp"

#include <iostream>



auto getCurrentOs()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    auto *os = new Linux();
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif

    return *os;
}

std::string getCPUSerialNumber()
{

    return "";
}

int main(int argc, const char *argv[])
{

    OpenSSL ssl;
    ssl.test();
    // if (ssl.verify())
    // {
    //     std::cout <<  "True" << std::endl;
    // }
    // else{
    //     std::cout << "False" << std::endl;
    // }
    


    auto os = getCurrentOs();

    if (os.getType() == "Linux" && false)
    {
        //os.getCPUInfo();
        os.getMotherBoardSerial();
        os.getGPUInfo();
        // std::cout << "Available network interfaces :" << std::endl;
        // os.listAllInterfaces();

        // int index;

        // int x;
        // std::cin >> x;
        // while (std::cin.fail() || x == 0 || x > os.getInterfacesCount())
        // {
        //     std::cout << "Invalid interface" << std::endl;
        //     std::cin.clear();
        //     std::cin.ignore(256, '\n');
        //     std::cin >> x;
        // }
        // std::cout << x << std::endl;

        // std::cout << os.getMACAddress(x) << std::endl;
    }

    return 0;
}
