#include "DexParser.h"
#include <iostream>
#include "type/DexFile.h"

void DexParser::parse_map_list()
{
    if (fseek(dex_file, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    map_list map_list{};
    map_list.size = -1;
    if (!fread(&map_list.size, sizeof(int), 1, dex_file))
    {
        printf("read file error.\n");
        return;
    }

    if(map_list.size == -1)
    {
        printf("map_list.size error.\n");
        return;
    }

    printf("map list size: %d\n", map_list.size);

    // Move forward map list size.
    if (fseek(dex_file,  offset + sizeof(u4), 0))
    {
        printf("seek file error.\n");
        return;
    }

    map_list.list = new map_item[map_list.size];
    if (map_list.size <= 0)
        return;

    if (!fread(map_list.list, sizeof(map_item), map_list.size, dex_file))
    {
        printf("read map_list.list error.\n");
        return;
    }

    for (int i = 0; i < map_list.size; i++)
        print_map_item(&map_list.list[i]);

    delete map_list.list;
    map_list.list = nullptr;
}

DexParser::DexParser(char const* dex_file_path)
{
    printf("parse: %s\n", dex_file_path);
    const errno_t s = fopen_s(&this->dex_file, dex_file_path, "rb");
    if (s != 0 || dex_file == nullptr) {
        printf("open dex file error.\n");
    }
}

void DexParser::parse()  
{
    header_item dex_header{};
    if (!fread(&dex_header, sizeof(header_item), 1, dex_file)) {
        printf("parse dex header error.\n");
        return;
    }

    // print_dex_header(&dex_header);
    offset = dex_header.map_off;
    parse_map_list();
}

DexParser::~DexParser()
{
    fclose(dex_file);
}
