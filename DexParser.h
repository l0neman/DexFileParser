#ifndef DEX_PARSER_H
#define DEX_PARSER_H

#include <iostream>

class DexParser
{
public:
    DexParser(char const* dex_file_path);
    void parse();
    ~DexParser();
private:
    void parse_map_list();
    FILE* dex_file = nullptr;
    unsigned int offset;
};

#endif // !DEX_PARSER_H