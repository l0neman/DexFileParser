#include <cstring>

#include "DexParser.h"
#include "type/DexFile.h"
#include "type/Leb128.h"

void DexParser::parse_map_list()
{
    const int offset = this->dex_header_.map_off;
    // seek to map list offset.
    if (0 != fseek(dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    // parse map list size.
    this->map_list_.size = -1;
    if (0 == fread(&this->map_list_.size, sizeof(int), 1, dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    if(this->map_list_.size == -1)
    {
        printf("map_list.size error.\n");
        return;
    }

    printf("map list size: %d\n", this->map_list_.size);

    // move to next map item offset.
    if (0 != fseek(dex_file_,  offset + sizeof(u4), 0))
    {
        printf("seek file error.\n");
        return;
    }

    this->map_list_.list = new map_item[this->map_list_.size];
    if (this->map_list_.size <= 0)
        return;

    if (0 == fread(this->map_list_.list, sizeof(map_item), 
        map_list_.size, dex_file_))
    {
        printf("read map_list.list error.\n");
        return;
    }

    for (u4 i = 0; i < this->map_list_.size; i++)
        print_map_item(&this->map_list_.list[i]);
}
/*
   - header_item
   - string_ids.
   - string list.
 */
void DexParser::parse_string_list(const u4 size, const u4 offset)
{
    // offset to string ids.
    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.");
        return;
    }

    const int string_ids_size = size;
    if (string_ids_size == 0)
    {
        printf("not found string ids.\n");
        return;
    }

    printf("string ids count: %d\n", string_ids_size);
    printf("string ids offset: %d\n", offset);

    // parse string id items.
    this->string_ids_ = new string_id_item[string_ids_size];
    if (0 == fread(this->string_ids_, sizeof(string_id_item),
        string_ids_size, this->dex_file_))
    {
        printf("read file error.");
        return;
    }

    printf("string item offset array: \n");

    this->string_list_size_ = string_ids_size;
    this->string_list_ = new string_data_item[string_ids_size];

    for(u4 i = 0; i < string_ids_size; i++)
    {
        const u4 str_off = this->string_ids_[i].string_data_off;
#ifdef _STRING_INFO_PRINT_
        printf("\nstring offset: %d\n", str_off);
#endif
        if (0 != fseek(this->dex_file_, str_off, 0))
        {
            printf("seek file error.");
            return;
        }

        // leb 为 1~5 byte，那么为了解析它，给予 5 byte 的缓冲。
        u1 leb128_buffer[5];

        if (0 == fread(&leb128_buffer, sizeof(u1), 5, this->dex_file_))
        {
            printf("read file error.");
            return;
        }

        const u1 * p1 = leb128_buffer;
        const u1** data = &p1;

        u4 size = Leb128::decode_unsigned_leb128(data);
        const u4 length = Leb128::unsigned_leb238_size(size);
#ifdef _STRING_INFO_PRINT_
        printf("leb128 length: %d\n", length);
        printf("string size: %d\n", size);
#endif
        if (size == 0)
        {
            continue;
        }

        // 最后一个留给 '\0' 用。
        const u4 str_size = size + 1;
        char* str_buf = new char[str_size];

        if (0 != fseek(this->dex_file_, str_off + length, 0))
        {
            printf("seek file error.");
            return;
        }

        if (0 == fread(str_buf, sizeof(char) * (str_size - 1), 
            1, this->dex_file_))
        {
            printf("read file error.");
            return;
        }

        str_buf[str_size - 1] = '\0';

        this->string_list_[i].utf16_size = leb128_buffer;
        this->string_list_[i].data = reinterpret_cast<u1*>(str_buf);

#ifdef _STRING_INFO_PRINT_
        printf("string: %s\n", str_buf);
#endif
    }
}

// lazy
const char * DexParser::get_string_from_string_list(const u4 index) const
{
    return reinterpret_cast<char*>(this->string_list_[index].data);
}

void DexParser::parse_type_ids(const u4 size, const u4 offset)
{
    const u4 type_ids_size = size;
    if (type_ids_size == 0)
    {
        printf("not found type ids.\n");
        return;
    }

    printf("type ids size: %d\n", type_ids_size);
    printf("type ids offset: %d\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    type_id_item* type_ids = new type_id_item[type_ids_size];

    if (0 == fread(type_ids, sizeof(type_id_item), type_ids_size, 
        this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    for (u4 i = 0; i < type_ids_size; i++)
    {
        printf("type: %s\n", get_string_from_string_list(
            type_ids[i].descriptor_idx));
    }

    delete[] type_ids;
    type_ids = nullptr;
}

void DexParser::parse_proto_ids(const u4 offset)
{
   
}

void DexParser::parse_field_ids(const u4 offset)
{
    
}

void DexParser::parse_method_ids(const u4 offset)
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
    this->dex_file_ = nullptr;
    this->string_ids_ = nullptr;
    this->string_list_size_ = 0;
    this->type_list_ = nullptr;

    printf("parse: %s\n\n", dex_file_path);
    const errno_t s = fopen_s(&this->dex_file_, dex_file_path, "rb");
    if (s != 0 || dex_file_ == nullptr) {
        printf("open dex file error.\n");
    }
}

void DexParser::parse_header_item()
{
    if (0 == fread(&this->dex_header_, sizeof(header_item), 1,
        this->dex_file_)) 
    {
        printf("parse dex header error.\n");
    }
}


void DexParser::parse()  
{
    printf(">>>>>>>>>>>> parse header_item:\n\n");
    parse_header_item();
    print_dex_header(&dex_header_);

    printf("\n>>>>>>>>>>>> parse map_list:\n\n");
    parse_map_list();

    for (u4 i = 0; i < this->map_list_.size; i++)
    {
        const map_item item = map_list_.list[i];
        printf("\n>>>>>>>>>>>> parse %s:\n\n", type_string(item.type));
        switch(item.type)
        {
        case TYPE_STRING_ID_ITEM:
            parse_string_list(item.size ,item.offset);
            break;
        case TYPE_TYPE_ID_ITEM:
            parse_type_ids(item.size, item.offset);
            break;
        default:
            printf("item: %s\n", type_string(item.type));
        }
    }
}

DexParser::~DexParser()
{
    // close file.
    fclose(dex_file_);

    // delete string ids.
    if (nullptr != this->string_ids_)
    {
        delete[] this->string_ids_;
        this->string_ids_ = nullptr;
    }

    // delete string list.
    /*if (nullptr != this->string_list_)
    {
        for (u4 i = 0; i < this->string_list_size_; i++)
        {
            delete[] this->string_list_[i];
            this->string_list_[i] = nullptr;
        }

        delete[] this->string_list_;
        this->string_list_ = nullptr;
    }*/

    // delete map list.
    if (nullptr != this->map_list_.list)
    {
        delete[] this->map_list_.list;
        this->map_list_.list = nullptr;
    }

    printf("end breakpoint.");
}
