#include <iostream>
#include "DexParser.h"

int main()
{
    std::cout << "Hello World!\n";
    DexParser dex_parser(R"(..\..\..\dexfile\test.dex)");
    dex_parser.parse();
    return 0;
}