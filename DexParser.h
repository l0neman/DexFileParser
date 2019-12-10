#ifndef DEX_PARSER_H
#define DEX_PARSER_H

#include <iostream>
#include "type/DexFile.h"

class DexParser
{
public:
    DexParser(char const* dex_file_path);
    ~DexParser();
    void parse();
private:
    void parse_header_item();
    void parse_map_list();
    void parse_string_list(const u4 size, const u4 offset);
    void parse_type_ids(const u4 size, const u4 offset);
    void parse_proto_ids(const u4 size, const u4 offset);
    void parse_field_ids(const u4 size, const u4 offset) const;
    void parse_method_ids(const u4 size, const u4 offset) const;
    void parse_class_defs(const u4 size, const u4 offset) const;
    void parse_encoded_field(const u4 offset, encoded_field* p) const;
    void parse_encoded_method(const u4 offset, encoded_method* p) const;
    void parse_class_data_list(const u4 size, const u4 offset) const;
    void parse_code_list(const u4 size, const u4 offset) const;
    void call_site_ids();
    void method_handles();
    const char* get_string_from_string_list(const u4 index) const;
    const char* get_type_description(const u4 index) const;
    proto_id_item get_proto_item(const u4 index) const;

    FILE* dex_file_;
    // 文件头。
    header_item dex_header_;
    // 类型映射表。
    map_list map_list_;
    // 字符串偏移信息。
    string_id_item* string_ids_;
    // 字符串数量。
    u4 string_list_size_;
    // 字符串列表。
    string_data_item* string_list_;
    // 类型列表。
    type_id_item* type_list_;
    // 原型列表。
    proto_id_item* proto_list_;
};

#endif // !DEX_PARSER_H