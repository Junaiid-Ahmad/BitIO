#include "../include/BitIO.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
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
	
	typedef struct HuffmanTree {
		uint16_t   Symbol[255];        // input symbol to be coded
		uint16_t   HuffmanCode[255];   // Encoded value for that symbol
	} HuffmanTree;
	
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

#ifdef __cplusplus
}
#endif
