# CMakeList.txt : CMake project for DexFileParserNew, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# enable map list info.
add_definitions(-D_MAP_LIST_PRINT_)

# enable string list info.
# add_definitions(-D_STRING_INFO_PRINT_)

# enable type list info.
# add_definitions(_D_TYPE_LIST_INFO_)

# enable proto list info.
# add_definitions(-D_PROTO_LIST_INFO_)

# enable field list info.
# add_definitions(-D_FIELD_LIST_INFO_)

# enable method list info.
# add_definitions(-D_METHOD_LIST_INFO_)

# enable class def list info.
add_definitions(-D_CLASS_DEF_LIST_INFO_)

# Add source to this project's executable.
add_executable (
    DexFileParser
    DexFileParser.cpp
    type/DexFile.h
    type/Leb128.cpp
    type/Leb128.h
    DexParser.cpp
    DexParser.h
    util/Formatter.cpp
    util/Formatter.h
    util/Printer.cpp
    util/Printer.h
)

# TODO: Add tests and install targets if needed.
