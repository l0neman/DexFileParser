#include <iostream>
#include "DexParser.h"
#include "type/Leb128.h"

int main()
{
    std::cout << "Hello World!\n";
    DexParser dex_parser(R"(..\..\..\dexfile\Hello.dex)");
    dex_parser.parse();
    return 0;
}