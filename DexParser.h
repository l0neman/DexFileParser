#ifndef DEX_PARSER_H
#define DEX_PARSER_H

#include <iostream>

class DexParser
{
public:
    DexParser(char const* dex_file_path);
    ~DexParser();
    void parse();
private:
    FILE* dex_file = nullptr;
};

#endif // !DEX_PARSER_H