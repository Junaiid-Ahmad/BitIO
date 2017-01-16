#include "../include/Deflate.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	// The length code is read as 8 bits, and left shifted 2.
	// The distance code is 15 bits
	// or maybe the length code isn't shifted and is just read as 9 bits, which would byte align it
	// This is all the LZ77 stuff.
	
	// Huffman
	// the Huffman tree contains 288 nodes, one each for each length code
	// 286 and 287 arent used.
	// Basically a huffman tree stores it's bit, and the value it represents is represented by it's location in the tree?
	// After the distance code is the match length code
	
	
	void DecodeStaticHuffman(BitInput *BitI, DeflateBlock *Deflate) {
		
	}
	
	void ParseDeflateBlock(BitInput *BitI, DeflateBlock *Deflate) {
		char ErrorString[BitIOStringSize];
		Deflate->IsLastBlock    = ReadBits(BitI, 1);
		Deflate->EncodingMethod = ReadBits(BitI, 2);
		
		switch (Deflate->EncodingMethod) {
			case 0:
				// Raw data
				break;
			case 1:
				// Static Huffman + preagreed table
				DecodeStaticHuffman(BitI, Deflate);
				break;
			case 2:
				// Dynamic Huffman
				break;
				
			default:
				snprintf(ErrorString, BitIOStringSize, "Invalid Deflate encoding method: %d\n", Deflate->EncodingMethod);
				Log(SYSEmergency, "BitIO", "ParseDeflateBlock", ErrorString);
				break;
		}
	}
	
	
	/*!
	 @param LengthDistanceSize    "Is the number of bits to read for the length and distance codes".
	 @param IsEAFormat            "EA format specifies the size in bytes of the code within the first 3 bits of the first byte".
	 */
	void ParseLZ77(BitInput *BitI, uint8_t LengthDistanceSize, bool IsEAFormat) {
		
		// Lookup the size of the sliding window.
		if (IsEAFormat == true && IsStreamByteAligned(BitI->BitsUnavailable, 1) == true) {
			// Read the first 3 bits of the byte for the size
			uint8_t EASize = PeekBits(BitI, 3);
			switch (EASize) {
				case 0:
					LengthDistanceSize = 1;
					break;
				case 1:
					LengthDistanceSize = 2;
					break;
				case 3:
					LengthDistanceSize = 3;
					break;
				case 7:
					LengthDistanceSize = 4;
					break;
			}
		}
	}
	
	void CreateHuffmanTree(uint16_t *SymbolOccurance) {
		
	}
	
	void ParseDeflate(BitInput *BitI) {
		uint8_t CompressionInfo    = ReadBits(BitI, 4); // 7 = LZ77 window size 32k
		uint8_t CompressionMethod  = ReadBits(BitI, 4); // 8 = DEFLATE
		uint8_t CheckCode          = ReadBits(BitI, 5); // for the previous 2 fields, MUST be multiple of 31
		bool    DictionaryPresent  = ReadBits(BitI, 1); // false
		uint8_t CompressionLevel   = ReadBits(BitI, 2); // Fixed Huffman
		if (DictionaryPresent == true) {
			uint16_t Dictionary    = ReadBits(BitI, 16);
		}
	}
	
	void DecodeHuffman(BitInput *BitI, size_t HuffmanSize) {
		// 3 alphabets, literal, "alphabet of bytes", or <length 8, distance 15> the first 2 are combined, 0-255 = literal, 256 = End of Block, 257-285 = length
		// FIXME: The Tilde ~ symbol is the negation symbol in C!!!!! XOR = ^
		
		uint8_t  DecodedData[32768] = {0};
		/* Parse Huffman block header */
		bool     IsLastHuffmanBlock     = ReadBits(BitI, 1);
		uint8_t  HuffmanCompressionType = ReadBits(BitI, 2); // 0 = none, 1 = fixed, 2 = dynamic, 3 = invalid.
		int32_t  DataLength             = 0;
		int32_t  OnesComplimentOfLength = 0; // Ones Compliment of DataLength
		
		if (OnesCompliment2TwosCompliment(OnesComplimentOfLength) != HuffmanSize) { // Make sure the numbers match up
			BitI->ErrorStatus->DecodeHuffman = InvalidData;
			char String2Print[BitIOStringSize];
			snprintf(String2Print, BitIOStringSize, "One's Compliment of Length: %d != Length %d", OnesComplimentOfLength, DataLength);
			Log(SYSWarning, "BitIO", "DecodeHuffman", String2Print);
		}
		
		if (IsLastHuffmanBlock == true) {
			
		}
		
		if (HuffmanCompressionType == 0) { // No compression.
			AlignInput(BitI, 1); // Skip the rest of the current byte
			DataLength             = ReadBits(BitI, 32);
			OnesComplimentOfLength = ReadBits(BitI, 32);
			if (OnesCompliment2TwosCompliment(OnesComplimentOfLength) != DataLength) {
				// Exit because there's an issue.
			}
			for (size_t Byte = 0; Byte < DataLength; Byte++) {
				DecodedData[Byte] = ReadBits(BitI, 8);
			}
		} else if (HuffmanCompressionType == 1) { // Static Huffman.
			uint8_t  Length   = (ReadBits(BitI, 8) - 254);
			uint16_t Distance = ReadBits(BitI, 5);
			
		} else if (HuffmanCompressionType == 2) { // Dynamic Huffman.
			/*
			 Huff->Dynamic->Length     = ReadBits(BitI, 5) + 257;
			 Huff->Dynamic->Distance   = ReadBits(BitI, 5) + 1;
			 Huff->Dynamic->CodeLength = ReadBits(BitI, 4) + 4;
			 */
		} else { // Invalid.
			// Reject the stream.
		}
		/*
		 if compressed with dynamic Huffman codes
		 read representation of code trees (see
		 subsection below)
		 loop (until end of block code recognized)
		 decode literal/length value from input stream
		 if value < 256
		 copy value (literal byte) to output stream
		 otherwise
		 if value = end of block (256)
		 break from loop
		 otherwise (value = 257..285)
		 decode distance from input stream
		 
		 move backwards distance bytes in the output
		 stream, and copy length bytes from this
		 position to the output stream.
		 end loop
		 while not last block
		 */
	}
	
#ifdef __cplusplus
}
#endif
