#include <iostream>
#include "DexParser.h"
#include "type/Leb128.h"

int main()
{
    std::cout << "Hello World!\n";
    DexParser::parse(R"(..\..\..\dexfile\Hello.dex)");
    return 0;
}