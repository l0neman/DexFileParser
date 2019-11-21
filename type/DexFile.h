#ifndef DEX_FILE_H
#define DEX_FILE_H

#include <cstdint>
#include <iostream>
#include <string>
#include "../util/Printer.h"
#include "Leb128.h"


using namespace std;

typedef uint8_t   u1;
typedef uint16_t  u2;
typedef uint32_t  u4;
typedef uint64_t  u8;
typedef int8_t    s1;
typedef int16_t   s2;
typedef int32_t   s4;
typedef int64_t   s8;

//typedef u1* uleb128;
//typedef u1* uleb128p1;

struct uleb128
{
    u4 value;
    u1* data;
    u4 length;
};

struct uleb128p1
{
    u4 value;
    u1* data;
    u4 length;
};

inline void parse_uleb128(/* u1[5] */u1* leb128_buffer, uleb128 *p)
{
    const u1* p1 = leb128_buffer;
    const u1** data = &p1;

    u4 size = Leb128::decode_unsigned_leb128(data);
    u4 length = Leb128::unsigned_leb128_size(size);

    p->value = size;
    p->data = leb128_buffer;
    p->length = length;
}

// 字节序标记常量。
enum
{
    ENDIAN_CONSTANT         = 0x12345678,
    REVERSE_ENDIAN_CONSTANT = 0x78563412,
};

enum
{
    TYPE_HEADER_ITEM                = 0x0000, // header_item	            size: 0x70
    TYPE_STRING_ID_ITEM             = 0x0001, // string_id_item	            size: 0x04
    TYPE_TYPE_ID_ITEM               = 0x0002, // type_id_item	            size: 0x04
    TYPE_PROTO_ID_ITEM              = 0x0003, // proto_id_item	            size: 0x0c
    TYPE_FIELD_ID_ITEM              = 0x0004, // field_id_item	            size: 0x08
    TYPE_METHOD_ID_ITEM             = 0x0005, // method_id_item	            size: 0x08
    TYPE_CLASS_DEF_ITEM             = 0x0006, // class_def_item	            size: 0x20
    TYPE_CALL_SITE_ID_ITEM          = 0x0007, // call_site_id_item	        size: 0x04
    TYPE_METHOD_HANDLE_ITEM         = 0x0008, // method_handle_item	        size: 0x08
    TYPE_MAP_LIST                   = 0x1000, // map_list	                size: 4 + (item.size * 12)
    TYPE_TYPE_LIST                  = 0x1001, // type_list	                size: 4 + (item.size * 2)
    TYPE_ANNOTATION_SET_REF_LIST    = 0x1002, // annotation_set_ref_list    size: 4 + (item.size * 4)
    TYPE_ANNOTATION_SET_ITEM        = 0x1003, // annotation_set_item	    size: 4 + (item.size * 4)
    TYPE_CLASS_DATA_ITEM            = 0x2000, // class_data_item	        size: 隐式；必须解析
    TYPE_CODE_ITEM                  = 0x2001, // code_item	                size: 隐式；必须解析
    TYPE_STRING_DATA_ITEM           = 0x2002, // string_data_item	        size: 隐式；必须解析
    TYPE_DEBUG_INFO_ITEM            = 0x2003, // debug_info_item	        size: 隐式；必须解析
    TYPE_ANNOTATION_ITEM            = 0x2004, // annotation_item	        size: 隐式；必须解析
    TYPE_ENCODED_ARRAY_ITEM         = 0x2005, // encoded_array_item	        size: 隐式；必须解析
    TYPE_ANNOTATIONS_DIRECTORY_ITEM = 0x2006, // annotations_directory_item size: 隐式；必须解析
};

inline const char* type_string(const u2 type)
{
    switch (type)
    {
    case TYPE_HEADER_ITEM:return "header_item";
    case TYPE_STRING_ID_ITEM:return "string_id_item";
    case TYPE_TYPE_ID_ITEM:return "type_id_item";
    case TYPE_PROTO_ID_ITEM:return "proto_id_item";
    case TYPE_FIELD_ID_ITEM:return "field_id_item";
    case TYPE_METHOD_ID_ITEM:return "method_id_item";
    case TYPE_CLASS_DEF_ITEM:return "class_def_item";
    case TYPE_CALL_SITE_ID_ITEM:return "call_site_id_item";
    case TYPE_METHOD_HANDLE_ITEM:return "method_handle_item";
    case TYPE_MAP_LIST:return "map_list";
    case TYPE_TYPE_LIST:return "type_list";
    case TYPE_ANNOTATION_SET_REF_LIST:return "annotation_set_ref_list";
    case TYPE_ANNOTATION_SET_ITEM:return "annotation_set_item";
    case TYPE_CLASS_DATA_ITEM:return "class_data_item";
    case TYPE_CODE_ITEM:return "code_item";
    case TYPE_STRING_DATA_ITEM:return "string_data_item";
    case TYPE_DEBUG_INFO_ITEM:return "debug_info_item";
    case TYPE_ANNOTATION_ITEM:return "annotation_item";
    case TYPE_ENCODED_ARRAY_ITEM:return "encoded_array_item";
    case TYPE_ANNOTATIONS_DIRECTORY_ITEM:return "annotations_directory_item";
    default:return"KNOWN";
    }
}

enum {
    ACC_PUBLIC      = 0x00000001,       // class, field, method, ic
    ACC_PRIVATE     = 0x00000002,       // field, method, ic
    ACC_PROTECTED   = 0x00000004,       // field, method, ic
    ACC_STATIC      = 0x00000008,       // field, method, ic
    ACC_FINAL       = 0x00000010,       // class, field, method, ic
    ACC_SYNCHRONIZED= 0x00000020,       // method (only allowed on natives)
    ACC_SUPER       = 0x00000020,       // class (not used in Dalvik)
    ACC_VOLATILE    = 0x00000040,       // field
    ACC_BRIDGE      = 0x00000040,       // method (1.5)
    ACC_TRANSIENT   = 0x00000080,       // field
    ACC_VARARGS     = 0x00000080,       // method (1.5)
    ACC_NATIVE      = 0x00000100,       // method
    ACC_INTERFACE   = 0x00000200,       // class, ic
    ACC_ABSTRACT    = 0x00000400,       // class, method, ic
    ACC_STRICT      = 0x00000800,       // method
    ACC_SYNTHETIC   = 0x00001000,       // field, method, ic
    ACC_ANNOTATION  = 0x00002000,       // class, ic (1.5)
    ACC_ENUM        = 0x00004000,       // class, field, ic (1.5)
    ACC_CONSTRUCTOR = 0x00010000,       // method (Dalvik only)
    ACC_DECLARED_SYNCHRONIZED = 0x00020000, // method (Dalvik only)
    ACC_CLASS_MASK =
    (ACC_PUBLIC | ACC_FINAL | ACC_INTERFACE | ACC_ABSTRACT
        | ACC_SYNTHETIC | ACC_ANNOTATION | ACC_ENUM),
    ACC_INNER_CLASS_MASK =
    (ACC_CLASS_MASK | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC),
    ACC_FIELD_MASK =
    (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
        | ACC_VOLATILE | ACC_TRANSIENT | ACC_SYNTHETIC | ACC_ENUM),
    ACC_METHOD_MASK =
    (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
        | ACC_SYNCHRONIZED | ACC_BRIDGE | ACC_VARARGS | ACC_NATIVE
        | ACC_ABSTRACT | ACC_STRICT | ACC_SYNTHETIC | ACC_CONSTRUCTOR
        | ACC_DECLARED_SYNCHRONIZED),
};

inline void print_access_flags_description(u4 flags)
{
    string desc;

    if ((flags & ACC_PUBLIC) != 0)
        desc += "public ";

    if ((flags & ACC_PRIVATE) != 0)
        desc += "private ";

    if ((flags & ACC_PROTECTED) != 0)
        desc += "protected ";

    if ((flags & ACC_STATIC) != 0)
        desc += "static ";

    if ((flags & ACC_FINAL) != 0)
        desc += "final ";

    if ((flags & ACC_SYNCHRONIZED) != 0)
        desc += "synchronized ";

    if ((flags & ACC_SUPER) != 0)
        desc += "super ";

    if ((flags & ACC_VOLATILE) != 0)
        desc += "volatile ";

    if ((flags & ACC_BRIDGE) != 0)
        desc += "bridge ";

    if ((flags & ACC_TRANSIENT) != 0)
        desc += "transient ";

    if ((flags & ACC_VARARGS) != 0)
        desc += "varargs ";

    if ((flags & ACC_NATIVE) != 0)
        desc += "native ";

    if ((flags & ACC_INTERFACE) != 0)
        desc += "interface ";

    if ((flags & ACC_ABSTRACT) != 0)
        desc += "abstract ";

    if ((flags & ACC_STRICT) != 0)
        desc += "strict ";

    if ((flags & ACC_SYNTHETIC) != 0)
        desc += "synthetic ";

    if ((flags & ACC_ANNOTATION) != 0)
        desc += "annotation ";

    if ((flags & ACC_ENUM) != 0)
        desc += "enum ";

    if ((flags & ACC_CONSTRUCTOR) != 0)
        desc += "constructor ";

    if ((flags & ACC_DECLARED_SYNCHRONIZED) != 0)
        desc += "declared synchronized ";

    printf("%s", desc.c_str());
}

struct header_item
{
    /* 魔数 */
    u1 magic[8];
    /*
      文件剩余内容（除 magic 和此字段之外的所有内容)的 adler32 校验和；
      用于检测文件损坏情况。
     */
    u1 checksum;
    /*
      文件剩余内容（除 magic、checksum 和此字段之外的所有内容)的 SHA-1 签名（哈希)；
      用于对文件进行唯一标识。
     */
    u1 signature[20];
    /* 整个文件（包括标头)的大小，以字节为单位 */
    u4 file_size;
    /*
      标头（整个区段)的大小，以字节为单位。这一项允许至少一定程度的向后/向前兼容性，
      而不必让格式失效。
     */
    u4 header_size;
    /* 字节序标记 */
    u4 endian_tag;
    /* 链接区段的大小；如果此文件未进行静态链接，则该值为 0 */
    u4 link_size;
    /*
      从文件开头到链接区段的偏移量；如果 link_size == 0，则该值为 0。
      该偏移量（如果为非零值)应该是到 link_data 区段的偏移量。
     */
    u4 link_off;
    /* 从文件开头到映射项的偏移量。该偏移量（必须为非零)应该是到 data 区段的偏移量 */
    u4 map_off;
    /* 字符串标识符列表中的字符串数量 */
    u4 string_ids_size;
    /*
      从文件开头到字符串标识符列表的偏移量；如果 string_ids_size == 0（极端情况)，
      则该值为 0。该偏移量（如果为非零值)应该是到 string_ids 区段开头的偏移量。
     */
    u4 string_ids_off;
    /* 类型标识符列表中的元素数量，最多为 65535 */
    u4 type_ids_size;
    /*
      从文件开头到类型标识符列表的偏移量；如果 type_ids_size == 0（极端情况)，
      则该值为 0。该偏移量（如果为非零值)应该是到 type_ids 区段开头的偏移量。
     */
    u4 type_ids_off;
    /* 原型标识符列表中的元素数量，最多为 65535 */
    u4 proto_ids_size;
    /*
      从文件开头到原型标识符列表的偏移量；如果 proto_ids_size == 0（极端情况)，
      则该值为 0。该偏移量（如果为非零值)应该是到 proto_ids 区段开头的偏移量。
     */
    u4 proto_ids_off;
    /* 字段标识符列表中的元素数量 */
    u4 field_ids_size;
    /*
      从文件开头到字段标识符列表的偏移量；如果 field_ids_size == 0，则该值为 0。
      该偏移量（如果为非零值)应该是到 field_ids 区段开头的偏移量。
     */
    u4 field_ids_off;
    /* 方法标识符列表中的元素数量 */
    u4 method_ids_size;
    /*
      从文件开头到方法标识符列表的偏移量；如果 method_ids_size == 0，则该值为 0。
      该偏移量（如果为非零值)应该是到 method_ids 区段开头的偏移量
     */
    u4 method_ids_off;
    /* 类定义列表中的元素数量 */
    u4 class_defs_size;
    /*
      从文件开头到类定义列表的偏移量；如果 class_defs_size == 0（极端情况)，
      则该值为 0。该偏移量（如果为非零值)应该是到 class_defs 区段开头的偏移量。
     */
    u4 class_defs_off;
    /* data 区段的大小（以字节为单位)。该数值必须是 sizeof(uint) 的偶数倍 */
    u4 data_size;
    /* 从文件开头到 data 区段开头的偏移量 */
    u4 data_off;
};

struct map_item
{
    u2 type;   // 项的类型。
    u2 unused; //（未使用。
    u4 size;   // 在指定偏移量处找到的项数量。
    u4 offset; // 从文件开头到相关项的偏移量。
};

inline void print_map_item(map_item const* map_item)
{
    printf("\nmap_item:\n");
    printf("type: 0x%x\n", map_item->type);
    printf("type desc: %s\n", type_string(map_item->type));
    printf("size: %d\n", map_item->size);
    printf("offset: %u\n", map_item->offset);
}

struct map_list
{
    u4 size;          // 列表的大小（以条目数表示）。
    map_item* list;   // 列表的元素。
    unique_ptr<map_item[]> list_;
};

struct string_id_item
{
    /*
      从文件开头到此项的字符串数据的偏移量。该偏移量应该是到 data 区段中某个位置的
      偏移量。
     */
    u4 string_data_off;
};

struct string_data_item
{
    /*
      此字符串的大小；以 UTF-16 代码单元（在许多系统中为“字符串长度”）为单位。
      也就是说，这是该字符串的解码长度（编码长度隐含在 0 字节的位置）。
     */
    uleb128 utf16_size;
    /*
      一系列 MUTF-8 代码单元（又称八位字节），后跟一个值为 0 的字节。
     */
    u1* data;
};

struct type_id_item
{
    /*
      此类描述符字符串的 string_ids 列表中的索引。该字符串必须符合上文定义的
      TypeDescriptor 的语法。
     */
    u4 descriptor_idx;
};

struct proto_id_item
{
    /*
      此原型的简短式描述符字符串的 string_ids 列表中的索引。该字符串必须符合上文定义
      的 ShortyDescriptor 的语法，而且必须与该项的返回类型和参数相对应。
     */
    u4 shorty_ids;
    /* 此原型的返回类型的 type_ids 列表中的索引 */
    u4 return_type_idx;
    /*
      从文件开头到此原型的参数类型列表的偏移量；如果此原型没有参数，则该值为 0。
      该偏移量（如果为非零值）应该位于 data 区段中，且其中的数据应采用下文中
      “"type_list"”指定的格式。此外，不得对列表中的类型 void 进行任何引用。
     */
    u4 parameters_off;
};

struct field_id_item
{
    /*
      此字段的定义符的 type_ids 列表中的索引。此项必须是“类”类型，而不能是“数组”或
      “基元”类型。
     */
    u2 class_idx;
    /* 此字段的类型的 type_ids 列表中的索引 */
    u2 type_idx;
    /*
      此字段的名称的 string_ids 列表中的索引。该字符串必须符合上文定义的 MemberName
      的语法。
     */
    u4 name_idx;
};

struct method_id_item
{
    /*
      此方法的定义符的 type_ids 列表中的索引。此项必须是“类”或“数组”类型，而不能是
      “基元”类型。
     */
    u2 class_idx;
    /* 此方法的原型的 proto_ids 列表中的索引 */
    u2 proto_idx;
    /*
      此方法名称的 string_ids 列表中的索引。该字符串必须符合上文定义的 MemberName
      的语法。
     */
    u4 name_idx;
};

struct class_def_item
{
    /*
      此类的 type_ids 列表中的索引。此项必须是“类”类型，而不能是“数组”或“基元”类型。
     */
    u4 class_idx;
    /*
      类的访问标记（public、final 等）。有关详情，请参阅“access_flags 定义”。
     */
    u4 access_flag;
    /*
     超类的 type_ids 列表中的索引。如果此类没有超类（即它是根类，例如 Object），
     则该值为常量值 NO_INDEX。如果此类存在超类，则此项必须是“类”类型，而不能是
     “数组”或“基元”类型。
     */
    u4 superclass_idx;
    /*
      从文件开头到接口列表的偏移量；如果没有接口，则该值为 0。该偏移量应该位于 data
      区段，且其中的数据应采用下文中“type_list”指定的格式。该列表的每个元素都必须是
      “类”类型（而不能是“数组”或“基元”类型），并且不得包含任何重复项。
     */
    u4 interfaces_off;
    /*
     文件（包含这个类（至少大部分）的原始来源）名称的 string_ids 列表中的索引；
     或者该值为特殊值 NO_INDEX，以表示缺少这种信息。任何指定方法的 debug_info_item
     都可以覆盖此源文件，但预期情况是大多数类只能来自一个源文件。
     */
    u4 source_file_idx;
    /*
      从文件开头到此类的注释结构的偏移量；如果此类没有注释，则该值为 0。该偏移量
      （如果为非零值）应该位于 data 区段，且其中的数据应采用下文中
      “annotations_directory_item”指定的格式，同时所有项将此类作为定义符进行引用。
     */
    u4 annotations_off;
    /*
      从文件开头到此项的关联类数据的偏移量；如果此类没有类数据，则该值为 0（这种情况
      有可能出现，例如，如果此类是标记接口）。该偏移量（如果为非零值）应该位于 data
      区段，且其中的数据应采用下文中“class_data_item”指定的格式，同时所有项将此类作
      为定义符进行引用。
     */
    u4 class_data_off;
    /*
      从文件开头到 static 字段的初始值列表的偏移量；如果没有该列表（并且所有 static
      字段都将使用 0 或 null 进行初始化），则该值为 0。该偏移量应该位于 data 区段，
      且其中的数据应采用下文中“encoded_array_item”指定的格式。该数组的大小不得超出
      此类所声明的 static 字段的数量，且 static 字段所对应的元素应采用相对应的
      field_list 中所声明的顺序每个数组元素的类型均必须与其相应字段的声明类型相匹配。
      如果该数组中的元素比 static 字段中的少，则剩余字段将使用适当类型 0 或 null
      进行初始化。
     */
    u4 static_values_off;
};

struct call_site_id_item
{
    /*
      从文件开头到调用点定义的偏移量。该偏移量应位于数据区段中，且其中的数据应采用下
      文中“call_site_item”指定的格式。
     */
    u4 call_site_off;
};

struct encoded_field
{
    /*
      此字段标识（包括名称和描述符）的 field_ids 列表中的索引；
      它会表示为与列表中前一个元素的索引之间的差值。列表中第一个元素的索引则直接表示出来。
     */
    uleb128 field_idx_diff;
    /*
      字段的访问标记（public、final 等）。
     */
    uleb128 access_flags;
};

struct encoded_method
{
    /*
      此方法标识（包括名称和描述符）的 method_ids 列表中的索引；
      它会表示为与列表中前一个元素的索引之间的差值。列表中第一个元素的索引则直接表示出来。
     */
    uleb128 method_idx_diff;
    /*
      方法的访问标记（public、final 等）。
     */
    uleb128 access_flags;
    /*
      从文件开头到此方法代码结构的偏移量；如果此方法是 abstract 或 native，则该值为 0。
      该偏移量应该是到 data 区段中某个位置的偏移量。数据格式由下文的“code_item”指定。
     */
    uleb128 code_off;
};

struct class_data_item
{
    /* 此项中定义的静态字段的数量 */
    uleb128 static_fields_size;
    /* 此项中定义的实例字段的数量 */
    uleb128 instance_fields_size;
    /* 此项中定义的直接方法的数量 */
    uleb128 direct_methods_size;
    /* 此项中定义的虚拟方法的数量 */
    uleb128 virtual_methods_size;
    /*
      定义的静态字段；以一系列编码元素的形式表示。这些字段必须按 field_idx 以升序进行排序。
     */
    encoded_field* static_fields;
    /*
      定义的实例字段；以一系列编码元素的形式表示。这些字段必须按 field_idx 以升序进行排序。
     */
    encoded_field* instance_fields;
    /*
      定义的直接（static、private 或构造函数的任何一个）方法；以一系列编码元素的形式
      表示。这些方法必须按 method_idx 以升序进行排序。
     */
    encoded_method* direct_methods;
    /*
      定义的虚拟（非 static、private 或构造函数）方法；以一系列编码元素的形式表示。
      此列表不得包括继承方法，除非被此项所表示的类覆盖。这些方法必须按 method_idx 以
      升序进行排序。虚拟方法的 method_idx 不得与任何直接方法相同。
     */
    encoded_method* virtual_methods;
};

struct type_item
{
    u2 type_idx; // type_ids 列表中的索引。
};

struct type_list
{
    u4 size;          // 列表的大小（以条目数表示）
    type_item* list;; // 列表的元素。
};

struct code_item
{
    u2 registers_size;
    u2 ins_size;
    u2 outs_size;
    u2 tries_size;
    u4 debug_info_off;
    u4 insns_size;
    u2* insns;
    // optional: u2 padding;
    // optional: try_item[tries_size]
    // optional: encode_catch_handler_list;
};

struct debug_info_item
{
    /*
      状态机的 line 寄存器的初始值。不表示实际的位置条目。
     */
    uleb128 line_start;
    /*
      已编码的参数名称的数量。每个方法参数都应该有一个名称，但不包括实例方法的 this
     （如果有）。
     */
    uleb128 parameters_size;
    /*
      方法参数名称的字符串索引。NO_INDEX 的编码值表示该关联参数没有可用的名称。
      该类型描述符和签名隐含在方法描述符和签名中。
     */
    uleb128p1* parameter_names;
};

struct field_annotation
{
    /* 字段（带注释）标识的 field_ids 列表中的索引 */
    u4 field_idx;
    /*
      从文件开头到该字段的注释列表的偏移量。该偏移量应该是到 data 区段中某个位置的偏
      移量。数据格式由下文的“annotation_set_item”指定。
     */
    u4 annotations_off;
};

struct method_annotation
{
    /* 方法（带注释）标识的 method_ids 列表中的索引 */
    u4 method_idx;
    /*
      从文件开头到该方法注释列表的偏移量。该偏移量应该是到 data 区段中某个位置的偏移量。
      数据格式由下文的“annotation_set_item”指定。
     */
    u4 annotations_off;
};

struct parameter_annotation
{
    /* 方法（其参数带注释）标识的 method_ids 列表中的索引 */
    u4 method_idx;
    /*
      从文件开头到该方法参数的注释列表的偏移量。该偏移量应该是到 data 区段中某个位置的
      偏移量。数据格式由下文的“annotation_set_ref_list”指定。
     */
    u4 annotations_off;
};

struct annotations_directory_item
{
    /*
      从文件开头到直接在该类上所做的注释的偏移量；如果该类没有任何直接注释，则该值为 0。
      该偏移量（如果为非零值）应该是到 data 区段中某个位置的偏移量。数据格式由下文的
      “annotation_set_item”指定。
     */
    u4 class_annotations_off;
    /* 此项所注释的字段数量 */
    u4 fields_size;
    /* 此项所注释的方法数量 */
    u4 annotated_methods_size;
    /* 此项所注释的方法参数列表的数量 */
    u4 annotated_parameters_size;
    /*
      Optional，所关联字段的注释列表。该列表中的元素必须按 field_idx 以升序进行排序。
     */
    field_annotation* field_annotations;
    /*
      Optional，所关联方法的注释列表。该列表中的元素必须按 method_idx 以升序进行排序。
     */
    method_annotation* method_annotations;
    /*
      Optional，所关联方法参数的注释列表。该列表中的元素必须按 method_idx 以升序进行排序。
     */
    parameter_annotation* parameter_annotation;
};

struct DexFile
{
    header_item header;

};

inline void print_dex_header(header_item* dex_header) {
    printf("magic: ");
    Printer::print_hex_array(dex_header->magic, 8);
    printf("checksum: %d\n", dex_header->checksum);

    printf("signature: ");
    Printer::print_hex_array(dex_header->signature, 20);

    printf("file_size: %d\n", dex_header->file_size);
    printf("header_size: %d\n", dex_header->header_size);
    printf("endian_tag: %d\n", dex_header->endian_tag);
    printf("link_size: %d\n", dex_header->link_size);
    printf("link_off: %d\n", dex_header->link_off);
    printf("map_off: %d\n", dex_header->map_off);
    printf("string_ids_size: %d\n", dex_header->string_ids_size);
    printf("string_ids_off: %d\n", dex_header->string_ids_off);
    printf("type_ids_size: %d\n", dex_header->type_ids_size);
    printf("type_ids_off: %d\n", dex_header->type_ids_off);
    printf("proto_ids_size: %d\n", dex_header->proto_ids_size);
    printf("proto_ids_off: %d\n", dex_header->proto_ids_off);
    printf("field_ids_size: %d\n", dex_header->field_ids_size);
    printf("field_ids_off: %d\n", dex_header->field_ids_off);
    printf("method_ids_size: %d\n", dex_header->method_ids_size);
    printf("method_ids_off: %d\n", dex_header->method_ids_off);
    printf("class_defs_size: %d\n", dex_header->class_defs_size);
    printf("class_defs_off: %d\n", dex_header->class_defs_off);
    printf("data_size: %d\n", dex_header->data_size);
    printf("data_off: %d\n", dex_header->data_off);
}

#endif // !DEX_FILE_H
