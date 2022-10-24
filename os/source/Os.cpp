#include "../include/Os.hpp"
#include <nvml.h>

using namespace std;

string Os::getType()
{
    return "";
}

string Os::getMACAddress(int index)
{
    return "";
}

void Os::listAllInterfaces()
{
    if (this->getInterfacesCount() == 0)
    {
        cout << "No interfaces available" << endl;
    }
    else
    {
        int count = 1;
        for (const auto &entry : this->interfaces)
        {
            cout << count << " : " << entry << endl;
            count++;
        }
    }
}

int Os::getInterfacesCount()
{
    return this->interfaces.size();
}

int Os::getGPUCount()
{
    unsigned int device_count;

    nvmlDeviceGetCount(&device_count);
    return device_count;
}

// TODO
void Os::getGPUInfo()
{
    nvmlReturn_t result;
    unsigned int temp;

    // First initialize NVML library
    result = nvmlInit();
    if (NVML_SUCCESS != result)
    {
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));

        printf("Press ENTER to continue...\n");
        getchar();
    }

    nvmlDevice_t device;

    for (size_t i = 0; i < this->getGPUCount(); i++)
    {
        result = nvmlDeviceGetHandleByIndex(i, &device);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get handle for device %i: %s\n", 0, nvmlErrorString(result));
        }

        char name[64]; 
        unsigned int length;
        nvmlDeviceGetName(device,name,length);



        char uuid[128];
        unsigned length_3;
         nvmlDeviceGetUUID(device,uuid,length_3);

          cout << name << endl;
          cout << "UUID - " << uuid << endl << endl;



    }

    result = nvmlShutdown();
    if (NVML_SUCCESS != result)
        printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));
}

void Os::getCPUInfo()
{
}

void Os::getMotherBoardSerial()
{
}


std::string Os::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
