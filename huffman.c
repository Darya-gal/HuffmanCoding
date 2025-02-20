#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"

// Function for counring symbols 
NODE* count_frequency(const char* filename) {
    FILE* input = fopen(filename, "rb");
    if (!input) {
        perror("Error opening file");
        exit(1);
    }

    unsigned int freq[BYTE_RANGE] = { 0 };
    unsigned char ch;
    while (fread(&ch, 1, 1, input)) {
        freq[ch]++;
    }
    fclose(input);

    NODE* head = NULL;

    for (int i = 0; i < BYTE_RANGE; i++) {
        if (freq[i] > 0) {
            NODE* newNode = (NODE*)malloc(sizeof(NODE));
            if (!newNode) {
                perror("Error memory allocation");
                exit(1);
            }
            newNode->symb = i;
            newNode->freq = freq[i];
            newNode->isSymb = 1;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->next = NULL;

            if (!head || head->freq >= newNode->freq) {
                newNode->next = head;
                head = newNode;
            }
            else {
                NODE* temp = head;
                while (temp->next && temp->next->freq < newNode->freq) {
                    temp = temp->next;
                }
                newNode->next = temp->next;
                temp->next = newNode;
            }
        }
    }
    return head;
}

// Function of building a tree Huffman
NODE* MakeTreeFromList(NODE* head) {
    if (!head) return NULL;

    while (head->next) {
        NODE* first = head;
        NODE* second = head->next;

        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if (!newNode) {
            perror("Error memory allocation");
            exit(1);
        }

        newNode->symb = 0;
        newNode->freq = first->freq + second->freq;
        newNode->isSymb = 0;
        newNode->left = first;
        newNode->right = second;
        newNode->next = NULL;

        head = second->next;

        NODE* temp = head;
        NODE* prev = NULL;

        while (temp && temp->freq < newNode->freq) {
            prev = temp;
            temp = temp->next;
        }

        if (prev == NULL) {
            newNode->next = head;
            head = newNode;
        }
        else {
            newNode->next = temp;
            prev->next = newNode;
        }

        first->next = NULL;
        second->next = NULL;
    }
    return head;
}

// Function of generation code Huffman
void generateHuffmanCodes(NODE* root, char* code, char codes[BYTE_RANGE][CODE_SIZE]) {
    if (!root) return;

    if (root->isSymb) {
        strcpy(codes[root->symb], code);
        return;
    }

    char leftCode[CODE_SIZE];
    char rightCode[CODE_SIZE];

    strcpy(leftCode, code);
    strcpy(rightCode, code);

    strcat(leftCode, "0");
    strcat(rightCode, "1");

    generateHuffmanCodes(root->left, leftCode, codes);
    generateHuffmanCodes(root->right, rightCode, codes);
}

// Coding file
unsigned char* encodeFile(const char* filename, char codes[BYTE_RANGE][CODE_SIZE], size_t* encodedSize) {
    FILE* input = fopen(filename, "rb");
    if (!input) {
        perror("Error opening file");
        exit(1);
    }

    unsigned char* encodedBuffer = (unsigned char*)malloc(1000000);
    if (!encodedBuffer) {
        perror("Error memory allocation");
        exit(1);
    }

    size_t byteIndex = 0;
    unsigned char byte = 0;
    size_t bitCount = 0;

    unsigned char ch;
    while (fread(&ch, 1, 1, input) == 1) {
        const char* code = codes[ch];
        for (int i = 0; code[i] != '\0'; ++i) {
            unsigned char bit = code[i] - '0';
            byte = (byte << 1) | bit;
            ++bitCount;

            if (bitCount == 8) {
                encodedBuffer[byteIndex++] = byte;
                bitCount = 0;
                byte = 0;
            }
        }
    }

    if (bitCount > 0) {
        byte <<= (8 - bitCount);
        encodedBuffer[byteIndex++] = byte;
    }

    fclose(input);
    *encodedSize = byteIndex;
    return encodedBuffer;
}

// Decoding file
void decodeFile(const char* compressedFilename, const char* decompressedFilename, NODE* root) {
    FILE* input = fopen(compressedFilename, "rb");
    FILE* output = fopen(decompressedFilename, "wb");
    if (!input || !output) {
        perror("Error opening file");
        exit(1);
    }

    NODE* current = root;
    unsigned char byte;
    while (fread(&byte, 1, 1, input)) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            current = (bit == 0) ? current->left : current->right;

            if (current->isSymb) {
                fputc(current->symb, output);
                current = root;
            }
        }
    }

    fclose(input);
    fclose(output);
}

// Recording bit data
void writeCompressedFile(const char* outputFilename, unsigned char* encodedBuffer, size_t encodedSize) {
    FILE* output = fopen(outputFilename, "wb");
    if (!output) {
        perror("Error opening file");
        exit(1);
    }

    fwrite(encodedBuffer, 1, encodedSize, output);
    fclose(output);
}
