#ifdef _WIN64

// Prevent accidentally selecting integrated GPU
// ---------------------------------------------
extern "C" 
{
    __declspec(dllexport) unsigned __int64 AmdPowerXpressRequestHighPerformance = 0x1;
    __declspec(dllexport) unsigned __int64 NvOptimusEnablement = 0x1;
}

#include "engine.h"

int main()
{
    Engine LDTA;
    return LDTA.engineRun();
}
#endif

