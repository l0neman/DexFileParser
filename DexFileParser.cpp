#include <iostream>
#include "DexParser.h"

long uint_to_long(unsigned int a)
{
    return 0L;
}

int main()
{
    std::cout << "Hello World!" << std::endl;
    DexParser dex_parser(R"(..\..\..\dexfile\test3.dex)");
    dex_parser.parse();
    return 0;
}