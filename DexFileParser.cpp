#include <iostream>
#include "DexParser.h"

int main()
{
    std::cout << "Hello World!\n";
    DexParser::parse(R"(..\..\..\dexfile\Hello.dex)");
    return 0;
}