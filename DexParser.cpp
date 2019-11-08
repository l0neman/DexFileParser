#include <cstring>

#include "DexParser.h"
#include "type/DexFile.h"
#include "type/Leb128.h"

void DexParser::parse_map_list()
{
    const int offset = this->dex_header.map_off;
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

    delete[] map_list.list;
    map_list.list = nullptr;
}

void DexParser::parse_string_ids()
{
    int offset = this->dex_header.string_ids_off;

    const int string_ids_size = this->dex_header.string_ids_size;
    if (string_ids_size == 0)
    {
        printf("not found string ids.\n");
        return;
    }

    printf("string pool count: %d\n", string_ids_size);
    printf("string pool offset: %d\n", offset);

    this->string_ids = new string_id_item[string_ids_size];
    if (!fread(this->string_ids, sizeof(string_id_item), 
        string_ids_size, this->dex_file))
    {
        printf("read file error.");
        return;
    }

    const size_t string_id_item_size = sizeof(string_id_item);
    printf("string item offset array: \n");

    this->string_list = new char* [string_ids_size];
    this->string_list_size = string_ids_size;
    
    for(int i = 0; i < string_ids_size; i++)
    {
        u4 str_off = this->string_ids[i].string_data_off;
#ifdef _STRING_PRINT_
        printf("\nstring offset: %d\n", str_off);
#endif
        if (fseek(this->dex_file, offset,0))
        {
            printf("seek file error.");
            return;
        }

        // leb 为 1~5 byte，那么为了解析它，给予 5 byte 的缓冲。
        u1 leb128_buffer[5];
        if (!fread(&leb128_buffer, sizeof(u1), 5, this->dex_file))
        {
            printf("read file error.");
            return;
        }

        const u1 * p1 = leb128_buffer;
        const u1** data = &p1;

        u4 size = Leb128::decode_unsigned_leb128(data);
        u4 length = Leb128::unsigned_leb238_size(size);

        printf("leb128 length: %d\n", length);
        printf("string size: %d\n", size);

        // 最后一个留给 '\0' 用。
        const u4 str_size = size + 1;
        char* str_buf = new char[str_size];

        if (fseek(this->dex_file, str_off + length, 0))
        {
            printf("seek file error.");
            return;
        }

        if (!fread(str_buf, sizeof(char) * (str_size - 1), 
            1, this->dex_file))
        {
            printf("read file error.");
            return;
        }

        str_buf[str_size - 1] = '\0';

        this->string_list[i] = str_buf;

        printf("string: %s\n", str_buf);

        // delete[] str_buf;
        // str_buf = nullptr;
    }

    printf("\n");
}

// lazy
const char * DexParser::get_string_from_string_list(unsigned int index)
{
    return this->string_list[index];
}

void DexParser::parse_type_ids()
{
    const u4 type_ids_size = this->dex_header.type_ids_size;
    if (type_ids_size == 0)
    {
        printf("not found type ids.\n");
        return;
    }

    printf("type ids size: %d\n", type_ids_size);
    printf("type ids offset: %d\n", this->dex_header.type_ids_off);

    if (fseek(this->dex_file, this->dex_header.type_ids_off, 0))
    {
        printf("seek file error.\n");
        return;
    }

    type_id_item* type_ids = new type_id_item[type_ids_size];

    if (!fread(type_ids, sizeof(type_id_item), type_ids_size, 
        this->dex_file))
    {
        printf("read file error.\n");
        return;
    }

    for (int i = 0; i < type_ids_size; i++)
    {
        printf("type: %s\n", get_string_from_string_list(
            type_ids[i].descriptor_idx));
    }

    delete[] type_ids;
    type_ids = nullptr;
}

void DexParser::parse_proto_ids()
{
   
}

void DexParser::parse_field_ids()
{
    
}

void DexParser::parse_method_ids()
{
    
}

void DexParser::parse_class_defs()
{
    
}

void DexParser::call_site_ids()
{
    
}

void DexParser::method_handles()
{
    
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
    if (!fread(&this->dex_header, sizeof(header_item), 1, dex_file)) {
        printf("parse dex header error.\n");
        return;
    }

    // print_dex_header(&dex_header);
    // parse_map_list();
    parse_string_ids();
    parse_type_ids();
}

DexParser::~DexParser()
{
    fclose(dex_file);

    delete[] this->string_ids;
    this->string_ids = nullptr;

    for (int i = 0; i < this->string_list_size; i++)
    {
        delete[] this->string_list[i];
        this->string_list[i] = nullptr;
    }

    delete[] this->string_list;
}
