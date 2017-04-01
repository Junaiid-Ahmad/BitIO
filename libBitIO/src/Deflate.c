#include <stdio.h>

#include "../include/Deflate.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* General Huffman shit */
    typedef struct HuffmanNode {
        int64_t    LeftHuffmanCode;
        int64_t    RightHuffmanCode;
        int64_t   *LeftNode;
        int64_t   *RightNode;
    } HuffmanNode;
    
    typedef struct HuffmanTree {
        uint64_t      NumNodes;
        HuffmanNode  *Node;
    } HuffmanTree;
    
    int64_t *MeasureSymbolProbability(uint64_t *Buffer, size_t BufferSize, uint8_t SymbolSize) {
        int64_t CurrentSymbol = 0;
        int64_t *Probabilities = calloc(1, BufferSize);
        
        for (uint64_t BufferSymbol = 0; BufferSymbol < BufferSize; BufferSymbol++) {
            // Extract the symbol contained in Buffer[BufferSymbol], then find that symbol in SymbolsProbabilities, then increment that element
            CurrentSymbol = Buffer[BufferSymbol];
            // Now use CurrentSymbol as the index into SymbolsProbabilities
            // and since by definition it was seen in the stream, increment the count unconditionally.
            Probabilities[CurrentSymbol] += 1;
            
            // So we're done with this, now all we need to do is sort the list.
        }
        return Probabilities;
    }
    
    int32_t CompareProbabilities(const void *A, const void *B) { // so what this probably wants, is for me to compare them, and tell the sorter which one is smaller?
        
        // The comparison function must return an integer less than, equal to, or
        // greater than zero if the first argument is considered to be respectively less
        // than, equal to, or greater than the second.
        
        // OK, so that means we're supposed to tell it the difference between A and B?
        // So, we need to find the difference between A and B.
        
        if (A > B) {
            return labs(A - B);
        } else {
            return -labs(B - A);
        }
    }
    
    /*!
     @remark "Uses the Andresson algorithm available at nada.kth.se/~snilsson/fast-sorting/"
     */
    void SortProbabilities(const int32_t *Probabilities, const size_t NumProbabilities) {
        for (uint64_t Probability = NumProbabilities; Probability > 0; Probability -= 2) {
            uint64_t Error = mergesort(Probabilities, NumProbabilities, sizeof(int64_t), CompareProbabilities(Probabilities[Probability], Probabilities[Probability - 1]));
            if (Error != 0) {
                Log(LOG_ERR, "libBitIO", "SortProbabilities", "Something happened with mergesort, but IDK what\n");
            }
        }
    }
    
    /*!
     @remark The SymbolsAndProbabilities need to be sorted before being submitted
     */
    HuffmanTree *BuildHuffmanTree(HuffmanTree *Tree2Build, int64_t **SymbolsAndProbabilities, size_t NumSymbols) {
        // Well start by taking the lowest probability pair (the bottom 2 regardless of value) symbols, and assigning them to nodes.
        Tree2Build->NumNodes = NumSymbols / 2;
        
        for (uint64_t Symbol = NumSymbols; Symbol > 0; Symbol -= 2) { // What do we do if the number of symbols is uneven?
            Tree2Build->Node[0].LeftNode  = SymbolsAndProbabilities[Symbol]; // FIXME: Not sure if "SymbolPair - 1" is gonna work?
            Tree2Build->Node[0].RightNode = SymbolsAndProbabilities[Symbol + 1];
        }
        
        return Tree2Build; // Is this seriously it?
                           // Make sure to remove any codes that do not occur (frequency = 0)
                           // When multiple values have the same frequency sort by intensity.
    }
    
    uint64_t GetHuffmanCode(HuffmanTree *Tree, int64_t **SymbolsAndProbabilities, int64_t Symbol, size_t NumSymbols) {
        uint64_t SymbolPlace = 0;
        for (uint64_t Index = 0; Index < NumSymbols; Index++) {
            // Find the probability of Symbol, then build the actual huffman code from where in the tree that symbol was assigned
           
            // No, we should simply do a search through all the nodes, and when we find the symbol, create the Huffman code.
            // Also, tree traversal for each fucking code will be slow should we just store the code alongside the symbol to speed up Decoding/Encoding?
        }
        return 0;
    }
    
    void ReadCanonicalHuffmanTable();
    
    
    
    
    /* Start LZ77 stuff */
    typedef struct LZ77 {
    } LZ77;
    
    /* LZ77 encoder */
    void EncodeLZ77(uint8_t *Buffer, uint8_t *EncodedBuffer, size_t BufferSize, uint64_t WindowSize, uint64_t AlphabetSize) {
         // The dictionary is simply the current buffer, at the current buffer position -(WindowSize / 2) +(WindowSize / 2)
        for (uint64_t BufferByte = 0; BufferByte < BufferSize; BufferByte++) {
            
        }
    }
    
    
    /* LZ77 decoder */
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /* ANCIENT CODE
    
	// The length code is read as 8 bits, and left shifted 2.
	// The distance code is 15 bits
	// or maybe the length code isn't shifted and is just read as 9 bits, which would byte align it
	// This is all the LZ77 stuff.
	
	// Huffman
	// the Huffman tree contains 288 nodes, one each for each length code
	// 286 and 287 arent used.
	// Basically a huffman tree stores it's bit, and the value it represents is represented by it's location in the tree?
	// After the distance code is the match length code
	
	void DecodeStoredHuffman(BitInput *BitI, DeflateBlock *Inflate) {
		
	}
	
	void DecodeStaticHuffman(BitInput *BitI, DeflateBlock *Inflate) {
		
	}
	
	void DecodeDynamicHuffman(BitInput *BitI, DeflateBlock *Inflate) {
		
	}
	
	void ParseDeflateBlock(BitInput *BitI, DeflateBlock *Inflate, uint16_t BlockSize) {
		Inflate->IsLastBlock    = ReadBits(BitI, 1, true); // no
		Inflate->EncodingMethod = ReadBits(BitI, 2, true); // 3
		
		switch (Inflate->EncodingMethod) {
			case 0:
				// Stored.
				DecodeStoredHuffman(BitI, Inflate);
				break;
			case 1:
				// Static Huffman + preagreed table
				DecodeStaticHuffman(BitI, Inflate);
				break;
			case 2:
				// Dynamic Huffman
				DecodeDynamicHuffman(BitI, Inflate);
				break;
			default:
				Log(LOG_EMERG, "BitIO", "ParseDeflateBlock", "Invalid Deflate encoding method: %d\n", Inflate->EncodingMethod);
				break;
		}
	}
	
	
     // *!
	 @param LengthDistanceSize    "Is the number of bits to read for the length and distance codes".
	 @param IsEAFormat            "EA format specifies the size in bytes of the code within the first 3 bits of the first byte".
	 /
	void ParseLZ77(BitInput *BitI, uint8_t LengthDistanceSize, bool IsEAFormat) {
		
		// Lookup the size of the sliding window.
        /*
		if (IsEAFormat == true && IsStreamByteAligned(BitI->BitsUnavailable, 1) == true) {
			// Read the first 3 bits of the byte for the size
			uint8_t EASize = PeekBits(BitI, 3, true);
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
     ///
	}
	
	void CreateHuffmanTree(uint16_t *SymbolOccurance) {
		
	}
	
	void DecodeHuffman(BitInput *BitI, size_t HuffmanSize) {
		// 3 alphabets, literal, "alphabet of bytes", or <length 8, distance 15> the first 2 are combined, 0-255 = literal, 256 = End of Block, 257-285 = length
		// FIXME: The Tilde ~ symbol is the negation symbol in C!!!!! XOR = ^
		
		uint8_t  DecodedData[32768] = {0};
		/ Parse Huffman block header /
		bool     IsLastHuffmanBlock     = ReadBits(BitI, 1, true);
		uint8_t  HuffmanCompressionType = ReadBits(BitI, 2, true); // 0 = none, 1 = fixed, 2 = dynamic, 3 = invalid.
		uint32_t DataLength             = 0;
		uint32_t OnesComplimentOfLength = 0; // Ones Compliment of DataLength
		
		if (OnesCompliment2TwosCompliment(OnesComplimentOfLength) != HuffmanSize) { // Make sure the numbers match up
			Log(LOG_WARNING, "BitIO", "DecodeHuffman", "One's Compliment of Length: %d != Length %d", OnesComplimentOfLength, DataLength);
		}
		
		if (IsLastHuffmanBlock == true) {
			
		}
		
		if (HuffmanCompressionType == 0) { // No compression.
			AlignInput(BitI, 1); // Skip the rest of the current byte
			DataLength             = ReadBits(BitI, 32, true);
			OnesComplimentOfLength = ReadBits(BitI, 32, true);
			if (OnesCompliment2TwosCompliment(OnesComplimentOfLength) != DataLength) {
				// Exit because there's an issue.
			}
			for (uint32_t Byte = 0; Byte < DataLength; Byte++) {
				DecodedData[Byte] = ReadBits(BitI, 8, true);
			}
		} else if (HuffmanCompressionType == 1) { // Static Huffman.
			uint8_t  Length   = (ReadBits(BitI, 8, true) - 254);
			uint16_t Distance = ReadBits(BitI, 5, true);
			
		} else if (HuffmanCompressionType == 2) { // Dynamic Huffman.
			/
			 Huff->Dynamic->Length     = ReadBits(BitI, 5) + 257;
			 Huff->Dynamic->Distance   = ReadBits(BitI, 5) + 1;
			 Huff->Dynamic->CodeLength = ReadBits(BitI, 4) + 4;
			 /
		} else { // Invalid.
			// Reject the stream.
		}
		/
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
		 /
	}
    
    struct LZ77Dictionary {
        uint8_t  *Value;
        uint64_t *Distance;
    };
    
    void DecodeLZ77(const uint8_t *EncodedBuffer, uint8_t *DecodedBuffer, const size_t BufferSize) {
        for (size_t Byte = 0; Byte < BufferSize; Byte++) {
        }
    }
    
    void EncodeLZ77(const uint8_t *RawData, uint8_t *EncodedBuffer, const size_t BufferSize, const size_t WindowSize) {
        for (size_t Byte = 0; Byte < BufferSize; Byte++) {
            // if BufferSize > WindowSize, we need to set up a dictionary structure that contains WindowSize bytes with their recent-ness.
        }
    }
    
    
    
    
    / Start Inflater /
    void ParseZLIBHeader(BitInput *BitI, DeflateBlock *Inflate) {
        // stored in big endian byte order, bits are stored LSB first
        uint8_t CompressionMethod  = ReadBits(BitI, 4, true); // 8 = DEFLATE
        uint8_t CompressionInfo    = ReadBits(BitI, 4, true); // 7 = LZ77 window size 32k
        uint8_t CheckCode          = ReadBits(BitI, 5, true); // 1, for the previous 2 fields, MUST be multiple of 31
        bool    DictionaryPresent  = ReadBits(BitI, 1, true); //
        uint8_t CompressionLevel   = ReadBits(BitI, 2, true); // 0
        if (DictionaryPresent == true) {
            uint32_t DictionaryID  = ReadBits(BitI, 32, true); // 0xEDC1
        }
        if (CompressionMethod == 8) {
            ParseDeflateBlock(BitI, Inflate, BlockSize[CompressionInfo]);
        } else {
            Log(LOG_ERR, "BitIO", "ParseDeflate", "Invalid DEFLATE compression method %d\n", CompressionMethod);
        }
    }
    
    void WriteZLIBHeader(BitOutput *BitO, DeflateBlock *Inflate) {
        WriteBits(BitO, Inflate->EncodingMethod, 4, false);
        
    }
    */
	
#ifdef __cplusplus
}
#endif
