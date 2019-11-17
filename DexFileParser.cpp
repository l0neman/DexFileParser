#include <iostream>
#include "DexParser.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    DexParser dex_parser(R"(..\..\..\dexfile\Hello.dex)");
    dex_parser.parse();
    return 0;
}