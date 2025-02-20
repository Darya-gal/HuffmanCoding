#include "huffman.h"

int main() {
    const char* inputFilename = "Rowling J.. Harry Potter and the Deathly Hallows.txt";
    const char* compressedFilename = "compressed.bin";
    const char* decompressedFilename = "decompressed.txt";
    
//creating sorted list of nodes
    NODE* head = count_frequency(inputFilename);
    if (!head) return 1;
    
//build a Huffman tree from sorted list of nodes
    NODE* root = MakeTreeFromList(head);

    //initialize array to keep the Huffman codes for each symbol
    char codes[BYTE_RANGE][CODE_SIZE] = { 0 };

    //generate the Huffman codes based on Huffman tree
    generateHuffmanCodes(root, "", codes);

    //declaring peremennay to store the size of encoded data
    size_t encodedSize = 0;

    //code the input file using the generated Huffman codes
    unsigned char* encodedBuffer = encodeFile(inputFilename, codes, &encodedSize);

    //writing the compressed data to file
    writeCompressedFile(compressedFilename, encodedBuffer, encodedSize);
    free(encodedBuffer);
    
   //decode compressed file and write the decompressed data to the file
    decodeFile(compressedFilename, decompressedFilename, root);

    printf("End!\n");
    return 0;
}
