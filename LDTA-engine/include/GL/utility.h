#ifndef UTILITY_H
#define UTILITY_H

#include <windows.h>
#include <intrin.h> 
#include <array>

//    COLOR CODE	       COLOR
//      0	                BLACK
//      1	                BLUE
//      2	                GREEN
//      3	                AQUA
//      4	                RED
//      5	                PURPLE
//      6	                YELLOW
//      7	                WHITE
//      8	                GRAY
//      9	                LIGHT BLUE
//      10(A)               LIGHT GREEN
//      11(B)               LIGHT AQUA
//      12(C)               LIGHT RED
//      13(D)               LIGHT PURPLE
//      14(E)               LIGHT YELLOW
//      15(F)               BRIGHT WHITE

void SET_COLOR(int color)
{
    if (color < 0 || color > 15) return;
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

std::string GetCpuInfo()
{
    // 4 is essentially hardcoded due to the __cpuid function requirements.
    // NOTE: Results are limited to whatever the sizeof(int) * 4 is...
    std::array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

    std::array<char, 64> charBuffer = {};

    // The information you wanna query __cpuid for.
    // https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019constexpr 
    std::array<int, 3> functionIds = 
    {
        // Manufacturer
        //  EX: "Intel(R) Core(TM"
        0x8000'0002,
        // Model
        //  EX: ") i7-8700K CPU @"
        0x8000'0003,
        // Clockspeed
        //  EX: " 3.70GHz"
        0x8000'0004
    };

    std::string cpu;

    for (int id : functionIds)
    {
        // Get the data for the current ID.
        __cpuid(integerBuffer.data(), id);

        // Copy the raw data from the integer buffer into the character buffer
        std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);

        // Copy that data into a std::string
        cpu += std::string(charBuffer.data());
    }
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    return cpu + " (" + std::to_string(sysInfo.dwNumberOfProcessors) + " CPUs)";
}

int SystemMemoryInfo() {

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return (statex.ullTotalPhys / 1024) / 1024;// MB
}

#endif