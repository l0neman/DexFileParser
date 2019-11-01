#include <iostream>
#include "DexParser.h"

int main()
{
    std::cout << "Hello World!\n";
	DexParser dexParser;
	dexParser.parse("./dexfile/Hello.dex");
}