#ifdef _WIN64

// Prevent accidentally selecting integrated GPU
// ---------------------------------------------
extern "C" 
{
    __declspec(dllexport) unsigned __int64 AmdPowerXpressRequestHighPerformance = 0x01;
    __declspec(dllexport) unsigned __int64 NvOptimusEnablement = 0x01;
}

#include "engine.h"

int main(int argc, char** argv)
{
    Engine LDTA;
    return LDTA.engineRun();
}

#endif

