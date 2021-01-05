#pragma once

#include "snap7.h"

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

// helper function for returning "normal value", including an error message on failure
template<typename T>
std::tuple<T,std::string> returnWithCliError(T ret)
{
    return std::tuple<T,std::string>(ret, CliErrorText(ret));
};

struct S7Address
{
    int area    =  0;
    int db      =  0;
    int start   =  0;
    int amount  =  0;
    int wordLen =  0;
};

enum S7FormatHint
{
    Unsigned = 0,
    Signed,
    Float
};

template <typename T>
void SwapEndian(T& val)
{
    union U
    {
        T val;
        std::array<std::uint8_t, sizeof(T)> raw;
    }
    src, dst;

    src.val = val;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    val = dst.val;
}
