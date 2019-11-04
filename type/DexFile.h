#ifndef DEX_FILE_H
#define DEX_FILE_H

#include <cstdint>
#include <iostream>

#include "util/Formatter.h"
#include "util/Printer.h"

using namespace std;

typedef uint8_t   u1;
typedef uint16_t  u2;
typedef uint32_t  u4;
typedef uint64_t  u8;
typedef int8_t    s1;
typedef int16_t   s2;
typedef int32_t   s4;
typedef int64_t   s8;

/* header_item */
struct DexHeader {
    u1 magic[8];        // 魔数。
    u1 checksum;        // 文件剩余内容（除 magic 和此字段之外的所有内容)的 adler32 校验和；用于检测文件损坏情况。
    u1 signature[20];   // 文件剩余内容（除 magic、checksum 和此字段之外的所有内容)的 SHA-1 签名（哈希)；用于对文件进行唯一标识。
    u4 file_size;       // 整个文件（包括标头)的大小，以字节为单位。
    u4 header_size;     // 标头（整个区段)的大小，以字节为单位。这一项允许至少一定程度的向后/向前兼容性，而不必让格式失效。
    u4 endian_tag;      // 字节序标记。
    u4 link_size;       // 链接区段的大小；如果此文件未进行静态链接，则该值为 0。
    u4 link_off;        // 从文件开头到链接区段的偏移量；如果 link_size == 0，则该值为 0。该偏移量（如果为非零值)应该是到 link_data 区段的偏移量。
    u4 map_off;         // 从文件开头到映射项的偏移量。该偏移量（必须为非零)应该是到 data 区段的偏移量。
    u4 string_ids_size; // 字符串标识符列表中的字符串数量。
    u4 string_ids_off;  // 从文件开头到字符串标识符列表的偏移量；如果 string_ids_size == 0（极端情况)，则该值为 0。该偏移量（如果为非零值)应该是到 string_ids 区段开头的偏移量。
    u4 type_ids_size;   // 类型标识符列表中的元素数量，最多为 65535。
    u4 type_ids_off;    // 从文件开头到类型标识符列表的偏移量；如果 type_ids_size == 0（极端情况)，则该值为 0。该偏移量（如果为非零值)应该是到 type_ids 区段开头的偏移量。
    u4 proto_ids_size;  // 原型标识符列表中的元素数量，最多为 65535。
    u4 proto_ids_off;   // 从文件开头到原型标识符列表的偏移量；如果 proto_ids_size == 0（极端情况)，则该值为 0。该偏移量（如果为非零值)应该是到 proto_ids 区段开头的偏移量。
    u4 field_ids_size;  // 字段标识符列表中的元素数量。
    u4 field_ids_off;   // 从文件开头到字段标识符列表的偏移量；如果 field_ids_size == 0，则该值为 0。该偏移量（如果为非零值)应该是到 field_ids 区段开头的偏移量。
    u4 method_ids_size; // 方法标识符列表中的元素数量。
    u4 method_ids_off;  // 从文件开头到方法标识符列表的偏移量；如果 method_ids_size == 0，则该值为 0。该偏移量（如果为非零值)应该是到 method_ids 区段开头的偏移量。
    u4 class_defs_size; // 类定义列表中的元素数量。
    u4 class_defs_off;  // 从文件开头到类定义列表的偏移量；如果 class_defs_size == 0（极端情况)，则该值为 0。该偏移量（如果为非零值)应该是到 class_defs 区段开头的偏移量。
    u4 data_size;       // data 区段的大小（以字节为单位)。该数值必须是 sizeof(uint) 的偶数倍。
    u4 data_off;        // 从文件开头到 data 区段开头的偏移量。
};

/* map_item */
struct DexMapItem
{
    u2 type;   // 项的类型。
    u2 unused; //（未使用。
    u4 size;   // 在指定偏移量处找到的项数量。
    u4 offset; // 从文件开头到相关项的偏移量。
};

/* map_list */
struct DexMapList
{
    u4 size;            // 列表的大小（以条目数表示）。
    DexMapItem list[1]; // 列表的元素。
};


/* string_id_item */
struct DexStringId
{
    u4 string_data_off;
};

/* type_id_item */
struct DexTypeId
{
    u4 descriptionIdx;
};



struct DexFile
{
    
};

inline void print_dex_header(DexHeader* dexHeader) {
    printf("header_item:\n");

    printf("magic: ");
    Printer::print_hex_array(dexHeader->magic, 8);
    printf("checksum: %d\n", dexHeader->checksum);

    printf("signature: ");
    Printer::print_hex_array(dexHeader->signature, 20);

    printf("file_size: %d\n", dexHeader->file_size);
    printf("header_size: %d\n", dexHeader->header_size);
    printf("endian_tag: %d\n", dexHeader->endian_tag);
    printf("link_size: %d\n", dexHeader->link_size);
    printf("link_off: %d\n", dexHeader->link_off);
    printf("map_off: %d\n", dexHeader->map_off);
    printf("string_ids_size: %d\n", dexHeader->string_ids_size);
    printf("string_ids_off: %d\n", dexHeader->string_ids_off);
    printf("type_ids_size: %d\n", dexHeader->type_ids_size);
    printf("type_ids_off: %d\n", dexHeader->type_ids_off);
    printf("proto_ids_size: %d\n", dexHeader->proto_ids_size);
    printf("proto_ids_off: %d\n", dexHeader->proto_ids_off);
    printf("field_ids_size: %d\n", dexHeader->field_ids_size);
    printf("field_ids_off: %d\n", dexHeader->field_ids_off);
    printf("method_ids_size: %d\n", dexHeader->method_ids_size);
    printf("method_ids_off: %d\n", dexHeader->method_ids_off);
    printf("class_defs_size: %d\n", dexHeader->class_defs_size);
    printf("class_defs_off: %d\n", dexHeader->class_defs_off);
    printf("data_size: %d\n", dexHeader->data_size);
    printf("data_off: %d\n", dexHeader->data_off);
}

#endif // !DEX_FILE_H
