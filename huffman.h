
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_RANGE 256  // Количество возможных байтов (0-255)
#define CODE_SIZE 256   // Максимальная длина кода Хаффмана

// Структура узла дерева Хаффмана
typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned int freq;
    struct node* left, * right, * next;
} NODE;

// Прототипы функций
NODE* count_frequency(const char* filename);
NODE* MakeTreeFromList(NODE* head);
void generateHuffmanCodes(NODE* root, char* code, char codes[BYTE_RANGE][CODE_SIZE]);
unsigned char* encodeFile(const char* filename, char codes[BYTE_RANGE][CODE_SIZE], size_t* encodedSize);
void decodeFile(const char* compressedFilename, const char* decompressedFilename, NODE* root);
void writeCompressedFile(const char* outputFilename, unsigned char* encodedBuffer, size_t encodedSize);

#endif // HUFFMAN_H