#include "DexParser.h"
#include <iostream>
#include "type/DexFile.h"

void DexParser::parse(char const* dexFilePath) {
    printf("parse: %s\n", dexFilePath);

	FILE* dexFile = NULL;
	errno_t s = fopen_s(&dexFile, dexFilePath, "rb");
	if (s != 0 || dexFile == NULL) {
        printf("open dex file error.\n");
		return;
	}

	DexHeader dexHeader;
	size_t i = fread(&dexHeader, sizeof(DexHeader), 1, dexFile);
	if (i != 1) {
        printf("parse dex header error.\n");
		return;
	}

	printDexHeader(&dexHeader);
}