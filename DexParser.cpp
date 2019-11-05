#include "DexParser.h"
#include <iostream>
#include "type/DexFile.h"

void DexParser::parse(char const* dex_file_path) {
    printf("parse: %s\n", dex_file_path);

    FILE* dex_file = nullptr;
    const errno_t s = fopen_s(&dex_file, dex_file_path, "rb");
    if (s != 0 || dex_file == nullptr) {
        printf("open dex file error.\n");
        return;
    }

    header_item dex_header{};
    const size_t i = fread(&dex_header, sizeof(header_item), 1, dex_file);
    if (i != 1) {
        printf("parse dex header error.\n");
        return;
    }

    print_dex_header(&dex_header);
}