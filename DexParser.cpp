#include "DexParser.h"
#include <iostream>
#include "type/DexFile.h"

void parse_map_list(char const* FILE, int offset)
{

}

DexParser::DexParser(char const* dex_file_path)
{
    printf("parse: %s\n", dex_file_path);
    const errno_t s = fopen_s(&dex_file, dex_file_path, "rb");
    if (s != 0 || dex_file == nullptr) {
        printf("open dex file error.\n");
    }
}

void DexParser::parse() {
    header_item dex_header{};
    const size_t i = fread(&dex_header, sizeof(header_item), 1, dex_file);
    if (i != 1) {
        printf("parse dex header error.\n");
        return;
    }

    print_dex_header(&dex_header);
}

DexParser::~DexParser()
{
    fclose(dex_file);
}
