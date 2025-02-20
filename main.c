#include "huffman.h"

int main() {
    const char* inputFilename = "Rowling J.. Harry Potter and the Deathly Hallows.txt";
    const char* compressedFilename = "compressed.bin";
    const char* decompressedFilename = "decompressed.txt";

    NODE* head = count_frequency(inputFilename);
    if (!head) return 1;

    NODE* root = MakeTreeFromList(head);

    char codes[BYTE_RANGE][CODE_SIZE] = { 0 };
    generateHuffmanCodes(root, "", codes);

    size_t encodedSize = 0;
    unsigned char* encodedBuffer = encodeFile(inputFilename, codes, &encodedSize);

    writeCompressedFile(compressedFilename, encodedBuffer, encodedSize);
    free(encodedBuffer);

    decodeFile(compressedFilename, decompressedFilename, root);

    printf("End!\n");
    return 0;
}