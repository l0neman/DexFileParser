#include <cstring>

#include "DexParser.h"
#include "type/DexFile.h"
#include "type/Leb128.h"

// header_item
// string_id_item
// type_id_item
// proto_id_item
// field_id_item
// method_id_item
// class_def_item
// map_list
// type_list
// annotation_set_item
// class_data_item
// code_item
// string_data_item
// debug_info_item
// annotation_item
// encoded_array_item
// annotation_directory_item
void DexParser::parse_map_list()
{
    const u4 offset = this->dex_header_.map_off;
    // seek to map list offset.
    if (0 != fseek(dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    // parse map list size.
    this->map_list_.size = 0;
    if (0 == fread(&this->map_list_.size, sizeof(int), 1, dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    if(this->map_list_.size == 0)
    {
        printf("map_list.size error.\n");
        return;
    }

    printf("map list size: %u\n", this->map_list_.size);

    // move to next map item offset.
    if (0 != fseek(dex_file_, offset + sizeof(u4), 0))
    {
        printf("seek file error.\n");
        return;
    }

    if (this->map_list_.size <= 0)
        return;

    this->map_list_.list = new map_item[this->map_list_.size];
    this->map_list_.list_ = make_unique<map_item[]>(this->map_list_.size);

    if (0 == fread(this->map_list_.list, sizeof(map_item), 
        this->map_list_.size, dex_file_))
    {
        printf("read map_list.list error.\n");
        return;
    }

#ifdef _MAP_LIST_PRINT_
    for (u4 i = 0; i < this->map_list_.size; i++)
        print_map_item(&this->map_list_.list[i]);
#endif
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

    const u4 string_ids_size = size;
    if (string_ids_size == 0)
    {
        printf("not found string ids.\n");
        return;
    }

    printf("string ids count: %u\n", string_ids_size);
    printf("string ids offset: %u\n", offset);

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
        u1 *leb128_buffer = new u1[5]{0};

        if (0 == fread(leb128_buffer, sizeof(u1), 5, this->dex_file_))
        {
            printf("read file error.");
            return;
        }

        const u1 * p1 = leb128_buffer;
        const u1** data = &p1;

        u4 size = Leb128::decode_unsigned_leb128(data);
        const u4 length = Leb128::unsigned_leb128_size(size);

        // reset struct.
        this->string_list_[i].data = nullptr;
        this->string_list_[i].utf16_size = {0, nullptr, 0};

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

        this->string_list_[i].utf16_size = { 0, nullptr, 0 };
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

const char* DexParser::get_type_description(const u4 index) const
{
    return get_string_from_string_list(this->type_list_[index].descriptor_idx);
}

proto_id_item DexParser::get_proto_item(const u4 index) const
{
    return this->proto_list_[index];
}

void DexParser::parse_type_ids(const u4 size, const u4 offset)
{
    const u4 type_ids_size = size;
    if (type_ids_size == 0)
    {
        printf("not found type ids.\n");
        return;
    }

    printf("type ids size: %u\n", type_ids_size);
    printf("type ids offset: %u\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    this->type_list_ = new type_id_item[type_ids_size];

    if (0 == fread(this->type_list_, sizeof(type_id_item), 
        type_ids_size, this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

#ifdef _TYPE_LIST_INFO_
    for (u4 i = 0; i < type_ids_size; i++)
    {
        printf("type: %s\n", get_string_from_string_list(
            this->type_list_[i].descriptor_idx));
    }
#endif
}

void DexParser::parse_proto_ids(const u4 size, const u4 offset)
{
    printf("proto ids size: %d\n", size);
    printf("proto ids offset: %d\n\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
       printf("seek file error.\n");
       return;
    }

   this->proto_list_ = new proto_id_item[size];

    if (0 == fread(this->proto_list_, sizeof(proto_id_item), size,
        this->dex_file_))
    {
        printf("read file error.");
        return;
    }

#ifdef _PROTO_LIST_INFO_
    for (u4 i = 0; i < size; i++)
    {
        printf("shorty: %s\n", get_string_from_string_list(
            this->proto_list_[i].shorty_ids));
        printf("return type: %s\n", get_type_description(
            this->proto_list_[i].return_type_idx));
        printf("parameters offset: %d\n\n", this->proto_list_[i].parameters_off);
    }
#endif
}

void DexParser::parse_field_ids(const u4 size, const u4 offset) const
{
    printf("field list size: %d\n", size);
    printf("field list offset: %d\n\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    field_id_item* field_list = new field_id_item[size];

    if (0 == fread(field_list, sizeof(field_id_item), size, 
        this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

#ifdef _FIELD_LIST_INFO_
    for (u4 i = 0; i < size; i++)
    {
        printf("field name: %s\n", get_string_from_string_list(field_list[i].name_idx));
        printf("parent class name: %s\n", get_type_description(field_list[i].class_idx));
        printf("type name: %s\n\n", get_type_description(field_list[i].type_idx));
    }
#endif

    delete[] field_list;
    field_list = nullptr;
}

void DexParser::parse_method_ids(const u4 size, const u4 offset) const
{
    printf("method ids size: %d\n", size);
    printf("method ids offset: %d\n\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    method_id_item* method_list = new method_id_item[size];

    if (0 == fread(method_list, sizeof(method_id_item), size, 
        this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

#ifdef _METHOD_LIST_INFO_
    for (u4 i = 0; i < size; i++)
    {
        printf("method name: %s\n", get_string_from_string_list(
            method_list[i].name_idx));
        printf("method proto return type: %s\n", get_type_description(
            get_proto_item(method_list[i].proto_idx).return_type_idx));
        printf("parent class name: %s\n\n", get_type_description(method_list[i].class_idx));
    }
#endif

    delete[] method_list;
    method_list = nullptr;
}

void DexParser::parse_class_defs(const u4 size, const u4 offset) const
{
    printf("class defs size: %u\n", size);
    printf("class defs offset: %u\n\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    class_def_item* class_def_list = new class_def_item[size];

    if (0 == fread(class_def_list, sizeof(class_def_item), size, 
        this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

#ifdef _CLASS_DEF_LIST_INFO_
    for (u4 i = 0; i < size; i++)
    {
        printf("class name: %s\n", get_type_description(class_def_list[i].class_idx));
        printf("access flags: ");
        print_access_flags_description(class_def_list[i].access_flag);
        printf("\n\n");
    }
#endif


    delete[] class_def_list;
    class_def_list = nullptr;
}


void DexParser::parse_encoded_field(const int offset, encoded_field* p) const
{
    // parse field_idx_diff.
    u1* uleb128_buff = new u1[5];

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }


    p->field_idx_diff = {};
    parse_uleb128(uleb128_buff, &(p->field_idx_diff));

    if (0 != fseek(this->dex_file_, offset + p->field_idx_diff.length, 0))
    {
        printf("seek file error.\n");
        return;
    }

    // parse access_flags.
    uleb128_buff = new u1[5];
    p->access_flags = {};
    parse_uleb128(uleb128_buff, &(p->access_flags));
}

void DexParser::parse_encoded_method(const int offset, encoded_method* p) const
{
    // parse method_idx_diff.
    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    u1* uleb128_buff = new u1[5];

    if (0 == fread(uleb128_buff, sizeof(u1), 5, this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    p->method_idx_diff = {};
    parse_uleb128(uleb128_buff, &(p->method_idx_diff));

    // parse access_flags.
    if (0 != fseek(this->dex_file_, offset + p->method_idx_diff.length, 0))
    {
        printf("seek file error.\n");
        return;
    }

    uleb128_buff = new u1[5];

    if (0 == fread(uleb128_buff, sizeof(u1), 5, this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    p->access_flags = {};
    parse_uleb128(uleb128_buff, &(p->access_flags));

    // parse code_off.
    if (0 != fseek(this->dex_file_, offset + p->access_flags.length, 0))
    {
        printf("seek file error.\n");
        return;
    }

    uleb128_buff = new u1[5];

    if (0 == fread(uleb128_buff, sizeof(u1), 5, this->dex_file_))
    {
        printf("read file error.\n");
        return;
    }

    p->code_off = {};
    parse_uleb128(uleb128_buff, &(p->code_off));
}


void DexParser::parse_class_data_list(const u4 size, const u4 offset) const
{
    printf("class data list size: %u\n", size);
    printf("class data list offset: %u\n", offset);

    if (0 != fseek(this->dex_file_, offset, 0))
    {
        printf("seek file error.\n");
        return;
    }

    class_data_item* class_data_list = new class_data_item[size] {{}};

    u4 seek_add = 0;
    for (u4 i = 0; i < size; i++)
    {
        printf("seek add: %d\n", seek_add);
        class_data_item *item = &class_data_list[i];

        // parse static_fields_size.
        {
            u1* leb128_buffer = new u1[5];

            if (0 == fread(leb128_buffer, sizeof(u1), 5, this->dex_file_))
            {
                printf("read file error.\n");
                return;
            }

            item->static_fields_size = {};
            parse_uleb128(leb128_buffer, &item->static_fields_size);
            printf("static fields size: %d\n", item->static_fields_size.value);
            seek_add += item->static_fields_size.length;
        }

        // parse instance_field_size.
        {
            u1* leb128_buffer = new u1[5];

            if (0 != fseek(this->dex_file_, offset + seek_add, 0))
            {
                printf("seek file error.\n");
                return;
            }

            if (0 == fread(leb128_buffer, sizeof(u1), 5, this->dex_file_))
            {
                printf("read file error.\n");
                return;
            }

            item->instance_fields_size = {};
            parse_uleb128(leb128_buffer, &item->instance_fields_size);
            printf("instance fields size: %d\n", item->instance_fields_size.value);
            seek_add += item->instance_fields_size.length;
        }

        // parse direct_methods_size.
        {
            u1* leb128_buffer = new u1[5];

            if (0 != fseek(this->dex_file_, offset + seek_add, 0))
            {
                printf("seek file error.\n");
                return;
            }

            if (0 == fread(leb128_buffer, sizeof(u1), 5, this->dex_file_))
            {
                printf("read file error.\n");
                return;
            }

            item->direct_methods_size = {};
            parse_uleb128(leb128_buffer, &item->direct_methods_size);
            printf("direct methods size: %d\n", item->direct_methods_size.value);
            seek_add += item->direct_methods_size.length;
        }

        // parse virtual_methods_size.
        {
            u1* leb128_buffer = new u1[5];

            if (0 != fseek(this->dex_file_, offset + seek_add, 0))
            {
                printf("seek file error.\n");
                return;
            }

            if (0 == fread(leb128_buffer, sizeof(u1), 5, this->dex_file_))
            {
                printf("read file error.\n");
                return;
            }

            item->virtual_methods_size = {};
            parse_uleb128(leb128_buffer, &item->virtual_methods_size);
            printf("virtual methods size: %d\n", item->virtual_methods_size.value);
            seek_add += item->virtual_methods_size.length;
        }

        printf("parse static_fields.\n");

        // parse static_fields.
        item->static_fields = new encoded_field[item->static_fields_size.value];
        for (u4 i = 0; i < item->static_fields_size.value; i++)
        {
            parse_encoded_field(offset + seek_add, &item->static_fields[i]);
            seek_add += item->static_fields[i].field_idx_diff.length + 
                item->static_fields[i].access_flags.length;
        }

        // parse instance_fields.
        item->instance_fields = new encoded_field[item->instance_fields_size.value];
        for (u4 i = 0; i < item->instance_fields_size.value; i++)
        {
            parse_encoded_field(offset + seek_add, &item->instance_fields[i]);
            seek_add += item->instance_fields[i].field_idx_diff.length +
                item->instance_fields[i].access_flags.length;
        }

        // parse direct_methods.
        item->direct_methods = new encoded_method[item->direct_methods_size.value];
        for (u4 i = 0; i < item->direct_methods_size.value; i++)
        {
            parse_encoded_method(offset + seek_add, &item->direct_methods[i]);
            seek_add += item->direct_methods[i].method_idx_diff.length +
                item->direct_methods[i].access_flags.length + 
                item->direct_methods[i].code_off.length;
        }

        // parse virtual_methods.
        item->virtual_methods = new encoded_method[item->virtual_methods_size.value];
        for (u4 i = 0; i < item->virtual_methods_size.value; i++)
        {
            parse_encoded_method(offset + seek_add, &item->virtual_methods[i]);
            seek_add += item->virtual_methods[i].method_idx_diff.length +
                item->virtual_methods[i].access_flags.length +
                item->virtual_methods[i].code_off.length;
        }
    }

    // delete class_data_list items.
    for (u4 i = 0; i < size; i++)
    {
       delete[] class_data_list[i].static_fields_size.data;
       class_data_list[i].static_fields_size.data = nullptr;

       delete[] class_data_list[i].instance_fields_size.data;
       class_data_list[i].instance_fields_size.data = nullptr;

       delete[] class_data_list[i].direct_methods_size.data;
       class_data_list[i].direct_methods_size.data = nullptr;

       delete[] class_data_list[i].virtual_methods_size.data;
       class_data_list[i].virtual_methods_size.data = nullptr;

       for(u4 j = 0; j < class_data_list[i].static_fields_size.value; j++)
       {
           delete[] class_data_list[i].static_fields[j].field_idx_diff.data;
           class_data_list[i].static_fields[j].field_idx_diff.data = nullptr;

           delete[] class_data_list[i].static_fields[j].access_flags.data;
           class_data_list[i].static_fields[j].access_flags.data = nullptr;
       }

       for (u4 j = 0; j < class_data_list[i].instance_fields_size.value; j++)
       {
           delete[] class_data_list[i].instance_fields[j].field_idx_diff.data;
           class_data_list[i].instance_fields[j].field_idx_diff.data = nullptr;

           delete[] class_data_list[i].instance_fields[j].access_flags.data;
           class_data_list[i].instance_fields[j].access_flags.data = nullptr;
       }

       for (u4 j = 0; j < class_data_list[i].direct_methods_size.value; j++)
       {
           delete[] class_data_list[i].direct_methods[j].method_idx_diff.data;
           class_data_list[i].direct_methods[j].method_idx_diff.data = nullptr;

           delete[] class_data_list[i].direct_methods[j].access_flags.data;
           class_data_list[i].direct_methods[j].access_flags.data = nullptr;

           delete[] class_data_list[i].direct_methods[j].code_off.data;
           class_data_list[i].direct_methods[j].code_off.data = nullptr;
       }

       for (u4 j = 0; j < class_data_list[i].virtual_methods_size.value; j++)
       {
           delete[] class_data_list[i].virtual_methods[j].method_idx_diff.data;
           class_data_list[i].virtual_methods[j].method_idx_diff.data = nullptr;

           delete[] class_data_list[i].virtual_methods[j].access_flags.data;
           class_data_list[i].virtual_methods[j].access_flags.data = nullptr;

           delete[] class_data_list[i].virtual_methods[j].code_off.data;
           class_data_list[i].virtual_methods[j].code_off.data = nullptr;
       }
    }

    delete[] class_data_list;
    class_data_list = nullptr;
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

        if (item.size == 0)
        {
            printf("item is empty.\n");
            continue;
        }

        switch(item.type)
        {
        case TYPE_STRING_ID_ITEM:
            parse_string_list(item.size ,item.offset);
            break;
        case TYPE_TYPE_ID_ITEM:
            parse_type_ids(item.size, item.offset);
            break;
        case TYPE_PROTO_ID_ITEM:
            parse_proto_ids(item.size, item.offset);
            break;
        case TYPE_FIELD_ID_ITEM:
            parse_field_ids(item.size, item.offset);
            break;
        case TYPE_METHOD_ID_ITEM:
            parse_method_ids(item.size, item.offset);
            break;
        case TYPE_CLASS_DEF_ITEM:
            parse_class_defs(item.size, item.offset);
            break;
        case TYPE_MAP_LIST:
            printf("ignore.\n");
            break;
        case TYPE_TYPE_LIST:
            printf("ignore.\n");
            break;
        case TYPE_ANNOTATION_SET_ITEM:
            printf("ignore.\n");
            break;
        case TYPE_CLASS_DATA_ITEM:
            parse_class_data_list(item.size, item.offset);
            break;
        case TYPE_CODE_ITEM:
            break;
        case TYPE_STRING_DATA_ITEM:
            printf("ignore.\n");
            break;
        case TYPE_DEBUG_INFO_ITEM:
            break;
        case TYPE_ANNOTATION_ITEM:
            break;
        case TYPE_ENCODED_ARRAY_ITEM:
            break;
        case TYPE_ANNOTATIONS_DIRECTORY_ITEM:
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

    // delete map list.
    if (nullptr != this->map_list_.list)
    {
        delete[] this->map_list_.list;
        this->map_list_.list = nullptr;
    }

    // delete string list.
    if (nullptr != this->string_list_)
    {
        for (u4 i = 0; i < this->string_list_size_; i++)
        {
            delete[] this->string_list_[i].data;
            this->string_list_[i].data = nullptr;

            delete[] this->string_list_[i].utf16_size.data;
            this->string_list_[i].utf16_size.data = nullptr;
        }

        delete[] this->string_list_;
        this->string_list_ = nullptr;
    }

    // delete type list.
    if (nullptr != this->type_list_)
    {
        delete[] this->type_list_;
        this->type_list_ = nullptr;
    }

    // delete proto list.
    if (nullptr != this->proto_list_)
    {
        delete[] this->proto_list_;
        this->proto_list_ = nullptr;
    }

}
