#ifndef DEXFILE_H_
#define DEXFILE_H_

#include <cstdint>
#include <iostream>

#include "util/Format.h"

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
	u1 magic[8];        /* 魔数 */
	u1 checksum;        /* 文件剩余内容（除 magic 和此字段之外的所有内容）的 adler32 校验和；用于检测文件损坏情况 */
	u1 signature[20];   /* 文件剩余内容（除 magic、checksum 和此字段之外的所有内容）的 SHA-1 签名（哈希）；用于对文件进行唯一标识 */
	u4 file_size;       /* 整个文件（包括标头）的大小，以字节为单位 */
	u4 header_size;     /* 标头（整个区段）的大小，以字节为单位。这一项允许至少一定程度的向后/向前兼容性，而不必让格式失效 */
	u4 endian_tag;      /* 字节序标记 */
	u4 link_size;       /* 链接区段的大小；如果此文件未进行静态链接，则该值为 0 */
	u4 link_off;        /* 从文件开头到链接区段的偏移量；如果 link_size == 0，则该值为 0。该偏移量（如果为非零值）应该是到 link_data 区段的偏移量 */
	u4 map_off;         /* 从文件开头到映射项的偏移量。该偏移量（必须为非零）应该是到 data 区段的偏移量 */
	u4 string_ids_size; /* 字符串标识符列表中的字符串数量 */
	u4 string_ids_off;  /* 从文件开头到字符串标识符列表的偏移量；如果 string_ids_size == 0（极端情况），则该值为 0。该偏移量（如果为非零值）应该是到 string_ids 区段开头的偏移量 */
	u4 type_ids_size;   /* 类型标识符列表中的元素数量，最多为 65535 */
	u4 type_ids_off;    /* 从文件开头到类型标识符列表的偏移量；如果 type_ids_size == 0（极端情况），则该值为 0。该偏移量（如果为非零值）应该是到 type_ids 区段开头的偏移量 */
	u4 proto_ids_size;  /* 原型标识符列表中的元素数量，最多为 65535 */
	u4 proto_ids_off;   /* 从文件开头到原型标识符列表的偏移量；如果 proto_ids_size == 0（极端情况），则该值为 0。该偏移量（如果为非零值）应该是到 proto_ids 区段开头的偏移量 */
	u4 field_ids_size;  /* 字段标识符列表中的元素数量 */
	u4 field_ids_off;   /* 从文件开头到字段标识符列表的偏移量；如果 field_ids_size == 0，则该值为 0。该偏移量（如果为非零值）应该是到 field_ids 区段开头的偏移量 */
	u4 method_ids_size; /* 方法标识符列表中的元素数量 */
	u4 method_ids_off;  /* 从文件开头到方法标识符列表的偏移量；如果 method_ids_size == 0，则该值为 0。该偏移量（如果为非零值）应该是到 method_ids 区段开头的偏移量 */
	u4 class_defs_size; /* 类定义列表中的元素数量 */
	u4 class_defs_off;  /* 从文件开头到类定义列表的偏移量；如果 class_defs_size == 0（极端情况），则该值为 0。该偏移量（如果为非零值）应该是到 class_defs 区段开头的偏移量 */
	u4 data_size;       /* data 区段的大小（以字节为单位）。该数值必须是 sizeof(uint) 的偶数倍 */
	u4 data_off;        /* 从文件开头到 data 区段开头的偏移量 */
};

void printDexHeader(DexHeader* dexHeader) {
    printf("header_item:\n");

	cout << "magic: " << dexHeader->magic << endl;

	cout << "checksum: " << dexHeader->checksum << endl;
	cout << "signature: " << dexHeader->signature << endl;
	cout << "file_size: " << dexHeader->file_size << endl;
	cout << "header_size: " << dexHeader->header_size << endl;
	cout << "endian_tag: " << dexHeader->endian_tag << endl;
	cout << "link_size: " << dexHeader->link_size << endl;
	cout << "link_off: " << dexHeader->link_off << endl;
	cout << "map_off: " << dexHeader->map_off << endl;
	cout << "string_ids_size: " << dexHeader->string_ids_size << endl;
	cout << "string_ids_off: " << dexHeader->string_ids_off << endl;
	cout << "type_ids_size: " << dexHeader->type_ids_size << endl;
	cout << "type_ids_off: " << dexHeader->type_ids_off << endl;
	cout << "proto_ids_size: " << dexHeader->proto_ids_size << endl;
	cout << "proto_ids_off: " << dexHeader->proto_ids_off << endl;
	cout << "field_ids_size: " << dexHeader->field_ids_size << endl;
	cout << "field_ids_off: " << dexHeader->field_ids_off << endl;
	cout << "method_ids_size: " << dexHeader->method_ids_size << endl;
	cout << "method_ids_off: " << dexHeader->method_ids_off << endl;
	cout << "class_defs_size: " << dexHeader->class_defs_size << endl;
	cout << "class_defs_off: " << dexHeader->class_defs_off << endl;
	cout << "data_size: " << dexHeader->data_size << endl;
	cout << "data_off: " << dexHeader->data_off << endl;
}

#endif // !DEXFILE_H_
