//
//  main.cpp
//  snellium-licence
//
//  Created by Clement Hellot on 18/08/2022.
//
#include "./os/include/Linux.hpp"
#include "./tools/include/Sign.hpp"

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


int main(int argc, const char *argv[])
{
    auto os = getCurrentOs();


    os.addConfigToSign("sign.dat");

    Sign sign;
    sign.test();

    return 0;
}
