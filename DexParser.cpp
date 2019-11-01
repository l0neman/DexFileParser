#include "DexParser.h"
#include <iostream>
#include "DexFile.h"

using namespace std;

void DexParser::parse(char const* dexFilePath) {
	cout << "parse: " << dexFilePath << endl;

	FILE* dexFile = NULL;
	errno_t s = fopen_s(&dexFile, dexFilePath, "rb");
	if (s != 0) {
		cout << "open dex file error." << endl;
		return;
	}

	DexHeader dexHeader;
	size_t i = fread(&dexHeader, sizeof(DexHeader), 1, dexFile);
	if (i != 1) {
		cout << "parse dex header error." << endl;
		return;
	}

	printDexHeader(&dexHeader);
}