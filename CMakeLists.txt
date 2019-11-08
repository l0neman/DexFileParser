# CMakeList.txt : CMake project for DexFileParserNew, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# add_definitions(-D_STRING_PRINT_)

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
