#ifndef PRINTER_H
#define PRINTER_H
#include <cstdint>

class Printer
{
public:
    static void print_int_array(int const* array, const unsigned int size);
    static void print_hex_array(uint8_t const* array, const unsigned int size);
};

#endif // !PRINTER_H