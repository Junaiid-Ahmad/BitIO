#include "../include/BitIO.h"

#ifndef BITIO_DEFLATE_H
#define BITIO_DEFLATE_H

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
    static const uint16_t BlockSize[8] = {
        0, 0, 0, 0, 0, 0, 0, 32768
    };
    
    typedef struct HuffmanNode {
        struct HuffmanNode *LeftNode;
        struct HuffmanNode *RightNode;
    } HuffmanNode;
    
    typedef struct HuffmanTree {
        uint64_t  NumNodes;
        HuffmanNode *Node[];
    } HuffmanTree;
    
    typedef struct LZ77Dictionary LZ77Dictionary;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    typedef struct DeflateBlock {
        bool     IsLastBlock;
        uint8_t  EncodingMethod:2;
    } DeflateBlock;
    
    typedef struct LZ77 {
        size_t WindowSize;
    } LZ77;
    
    extern enum HuffmanConstants {
        Huffman_MaxLengthCodes      = 286,
        Huffman_MaxDistanceSymbols  = 30,
        Huffman_MaxSymbols          = Huffman_MaxLengthCodes + Huffman_MaxDistanceSymbols,
        Huffman_FixedLengthCodes    = 288,
    } HuffmanConstants;
    
    /*!
     @abstract                     "Decodes Huffman encoded data".
     @remark                       "It's not even CLOSE to API/ABI compatible with zlib, because zlib is shit".
     */
    //void DecodeHuffman(BitInput *BitI, Huffman *Huff);
    
    /*!
     @abstract                     "Parses DEFLATE encoded block, and sends it off to the Huffman/LZ77 decoder".
     
     @param    BitI                "Pointer to BitInput".
     */
    void           ParseDeflate(BitInput *BitI);
    
    void           DecodeHuffman(BitInput *BitI, size_t HuffmanSize);
    
    void           ParseZLIBHeader(BitInput *BitI, DeflateBlock *Inflate);
    
    void           CreateHuffmanTree(uint16_t *SymbolOccurance);
    
    void           ParseLZ77(BitInput *BitI, uint8_t LengthDistanceSize, bool IsEAFormat);
    
    void           ParseDeflateBlock(BitInput *BitI, DeflateBlock *Inflate, uint16_t BlockSize);
    
#ifdef __cplusplus
}
#endif

#endif /* BITIO_DEFLATE_H */
