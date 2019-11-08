#ifndef DEX_PARSER_H
#define DEX_PARSER_H

#include <iostream>
#include "type/DexFile.h"

class DexParser
{
public:
    DexParser(char const* dex_file_path);
    void parse();
    ~DexParser();
private:
    void parse_map_list();
    void parse_string_ids();
    void parse_type_ids();
    void parse_proto_ids();
    void parse_field_ids();
    void parse_method_ids();
    void parse_class_defs();
    void call_site_ids();
    void method_handles();
    const char* get_string_from_string_list(unsigned int index);
    FILE* dex_file = nullptr;
    header_item dex_header{};
    string_id_item* string_ids;
    // string pool info.
    unsigned int string_list_size;
    char** string_list;

    type_id_item* type_list;
};

#endif // !DEX_PARSER_H