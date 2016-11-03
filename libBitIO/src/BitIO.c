#include "../include/BitIO.h"

#ifdef __cplusplus
extern "C" {
#endif

	// To handle a format specifier, there are 2 parts. The first is in argv which is %0XY, the second is in -s which is just a raw int

	uint64_t BitIOCurrentArgument = 1;
	char     BitInputFormatSpecifier[];
	uint64_t BitInputCurrentSpecifier  = 0;
	uint64_t BitOutputCurrentSpecifier = 0;

	uint16_t SwapEndian16(uint16_t Data2Swap) { // In UnitTest
		return ((Data2Swap & 0xFF00) >> 8) | ((Data2Swap & 0x00FF) << 8);
	}

	uint32_t SwapEndian32(uint32_t Data2Swap) { // In UnitTest
        uint32_t Swapped = 0;
		Swapped = ((Data2Swap & 0xFF000000) >> 24) | ((Data2Swap & 0x00FF0000) >> 8) | ((Data2Swap & 0x0000FF00) << 8) | ((Data2Swap & 0x000000FF) << 24);
        return Swapped;
	}

	uint64_t SwapEndian64(uint64_t Data2Swap) { // In UnitTest
		return (((Data2Swap & 0xFF00000000000000) >> 56) | ((Data2Swap & 0x00FF000000000000) >> 40) | \
				((Data2Swap & 0x0000FF0000000000) >> 24) | ((Data2Swap & 0x000000FF00000000) >>  8) | \
				((Data2Swap & 0x00000000FF000000) <<  8) | ((Data2Swap & 0x0000000000FF0000) << 24) | \
				((Data2Swap & 0x000000000000FF00) << 40) | ((Data2Swap & 0x00000000000000FF) << 56));
	}

	bool IsOdd(int64_t Input) { // UnitTest Unnecessary
		return (Input % 2);
	}

	uint64_t Bits2Bytes(uint64_t Bits) { // UnitTest Unnecessary
		return (Bits / 8);
	}

	uint64_t Bytes2Bits(uint64_t Bytes) { // UnitTest Unnecessary
		return (Bytes * 8);
	}

    uint8_t BitsRemaining(uint64_t BitsAvailable) { // UnitTest Unnecessary // 2
		//uint8_t Bits = 0;
		// The problem with this function, is that it doesn't use as many bits as possible, so if there are 12 bits left, it'd say to read 4 from byte 1, when you can read all 8.
		// Let's solve that.

		//Bits = BitsAvailable > 8 ? 8 : BitsAvailable; // New result: 2 ; Old Result: 2
		return BitsAvailable > 8 ? 8 : BitsAvailable;
	}

	uint64_t Signed2Unsigned(int64_t Signed) { // In UnitTest
		return (uint64_t)Signed;
	}

	int64_t Unsigned2Signed(uint64_t Unsigned) {  // In UnitTest
		return (int64_t)Unsigned;
	}

	uint64_t Power2Mask(uint8_t Exponent) { // UnitTest Unnecessary
		//uint64_t Mask1 = 0, Mask2 = 0;

		if ((Exponent > 64) || (Exponent <= 0)) {
			return EXIT_FAILURE;
		}

		/*
		Mask1          = 0x80 >> Exponent; // Mask1          = 1 << Exponent;
        Mask2          = Mask1 - 1;
        //return Mask1 + Mask2;
        return Mask2;
		// Shift 1 (Exponent - 1) times, then take the result, subtract one, and add that to the result.
		 */
        return ((1 << Exponent) - 1); // 7
	}

	uint64_t OnesCompliment2TwosCompliment(int64_t Input) { // All unset bits ABOVE the set bit are set, including those originally set
															// If 1 was already set, it's set as well.
		return ~Input + 1;
	}

	uint64_t TwosCompliment2OnesCompliment(int64_t Input) { // All unset bits become set, except those originally set
		return Input ^ 0xFFFFFFFFFFFFFFFF;
	}

	/*
	 uint8_t FindHighestBitSet(uint64_t InputData) {
		// Just return 0... that should never appear, so you're good...
		uint8_t HighestBitSet = 0;
		for (uint8_t Bit = 64; Bit > 0; Bit--) { // stop at the first 1 bit, and return that.
	 if ((InputData & (1 << (Bit - 1))) == 1) {
	 HighestBitSet = Bit;
	 }
		}
		return HighestBitSet;
	 }
	 */
	/*
	 uint8_t CountBits(uint64_t Input) {
		if (Input == 0) {
	 return 0;
		}
		uint8_t Count = 0;
		for (size_t Bit = 64; Bit > 0; Bit--) {
	 if ((Input & (1 << (Bit - 1))) == 1) {
	 Count++;
	 }
		}
		return Count;
	 }
	 */

    uint64_t RoundUp2NearestMultiple(uint64_t Number2Round, uint8_t Multiple) { // 15, 8
        uint64_t Rounded = 0;
        if ((Number2Round % 8) == 0) {
            Rounded = Number2Round;
        } else {
            Rounded = Number2Round + (Multiple - (Number2Round % Multiple)); // 15 + (8 - (15 % 8))
        }
        return Rounded;
    }

    uint8_t DetectSystemEndian(void) { // this function always returns lil no matter what?
        uint8_t SystemEndian = 0;
        uint16_t Endian = 0xFFFE;
        if (Endian == 0xFFFE) {
            SystemEndian = LittleEndian;
        } else if (Endian == 0xFEFF) {
            SystemEndian = BigEndian;
        } else {
            SystemEndian = UnknownEndian;
        }
        return SystemEndian;
    }

	bool IsStreamByteAligned(uint64_t BitsUsed, uint8_t BytesOfAlignment) {
		bool Truth = 0;

		if (BytesOfAlignment == 1) {
			if ((BitsUsed % 8) == 0) {
				Truth = true;
			}
			Truth = false;
		} else {
			if ((BitsUsed % Bytes2Bits(BytesOfAlignment)) == true) {
				Truth = true;
			} else {
				Truth = false;
			}
		}
		return Truth;
	}

	void AlignInput(BitInput *BitI, uint8_t BytesOfAlignment) {
		uint64_t Bits2Align    = (BitsRemaining(BitI->BitsAvailable) + (BitsRemaining(BytesOfAlignment - 1)));
		BitI->BitsAvailable   -= Bits2Align;
		BitI->BitsUnavailable += Bits2Align;

		/*
		 uint8_t Bits2Add       = BitsRemaining(BitI->BitsAvailable);
		 BitI->BitsAvailable   -= Bits2Add;
		 BitI->BitsUnavailable += Bits2Add;
		 if (Bytes2Align > 1) {
			uint64_t Wat = Bytes2Bits(Bytes2Align -1);
			BitI->BitsAvailable   -= Wat;
			BitI->BitsUnavailable += Wat;
		 }
		 */
	}

	void AlignOutput(BitOutput *BitO, uint8_t BytesOfAlignment) {
		uint64_t Bits2Align     = (BitsRemaining(BitO->BitsAvailable) + (BitsRemaining(BytesOfAlignment - 1)));
		BitO->BitsAvailable    -= Bits2Align;
		BitO->BitsUnavailable  += Bits2Align;

		/*
		 uint8_t Bits2Add       = BitsRemaining(BitO->BitsAvailable);
		 BitO->BitsAvailable   += Bits2Add;
		 BitO->BitsUnavailable -= Bits2Add;
		 if (Bytes2Align > 1) {
			uint64_t Wat           = Bytes2Bits(Bytes2Align -1);
			BitO->BitsAvailable   -= Wat;
			BitO->BitsUnavailable += Wat;
		 }
		 */
	}

	static void PrintHelp(void) {
		fprintf(stderr, "Usage: -i <input> -o <output> (-s is to specify the start of a file sequence, and should be before the argument it's intended for)\n");
	}

    void ParseInputOptions(BitInput *BitI, int argc, const char *argv[]) {
        // I need to add some variables to BitInput to split the path into 3, one for the base path, the second for the format specifier number, and the third for the extension.
        while (BitI->File == NULL) {
            for (int Argument = BitIOCurrentArgument; Argument < argc; Argument++) {
                char Path[BitIOPathSize];
                snprintf(Path, BitIOPathSize, "%s", argv[Argument]);

                if ((strcasecmp(Path, "-s") == 0) && (BitInputCurrentSpecifier == 0)) {
                    Argument += 1;
                    sscanf(Path, "%lld", &BitInputCurrentSpecifier);
                }

                if (strcasecmp(Path, "-i") == 0) {
                    Argument += 1;
                    if (BitInputCurrentSpecifier != 0) {
                        // We need to replace the character sequence with the specifier
                        // So use sprintf to substitute the format string with the SpecifierOffset, then increment it by one for the next time
                        snprintf(Path, BitIOPathSize, "%s", argv[Argument]);
                        // Extract the format specifier from the argument and replace it with SpecifierOffset and put that into Path
                    } else {
                        snprintf(Path, BitIOPathSize, "%s", argv[Argument]);
                    }

                    if (strcasecmp(Path, "-") == 0) {
                        BitI->File = freopen(Path, "rb", stdin);
                        setvbuf(BitI->File, BitI->Buffer, _IONBF, BitInputBufferSize);
                    } else {
                        BitI->File = fopen(Path, "rb");
                        setvbuf(BitI->File, BitI->Buffer, _IONBF, BitInputBufferSize);
                        if (BitI->File == NULL) {
                            Log(SYSCritical, &BitI->ErrorStatus->ParseInputOptions, FopenFailed, "BitIO", "ParseInputOptions", strerror(errno));
                        }
                    }
                    BitIOCurrentArgument = Argument + 1;
                }
                /*
                BitIOCurrentArgument += Argument - 1;
                if ((Argument >= argc) && (BitI->File == NULL)) { // end of the argument list with no file, so start back at the top.
                    BitIOCurrentArgument = 1;
                }
                 */
            }
        }
    }

    void ParseOutputOptions(BitOutput *BitO, int argc, const char *argv[]) {
        while (BitO->File == NULL) {
            for (int Argument = BitIOCurrentArgument; Argument < argc; Argument++) { // The problem is that InputBuffer read past where it should.
                int64_t SpecifierOffset = 0;
                char Path[BitIOPathSize];
                snprintf(Path, BitIOPathSize, "%s", argv[Argument]);

                if (strcasecmp(Path, "-s")    == 0) { // Specifier Offset
                    Argument += 1;
                    snprintf((char*)SpecifierOffset, 1, "%s", argv[Argument]);
                }

                if (strcasecmp(Path, "-o")    == 0) {
                    Argument += 1;
                    snprintf(Path, BitIOPathSize, "%s", argv[Argument]);
                    if (strcasecmp(Path, "-") == 0) {
                        BitO->File = freopen(Path, "wb", stdout);
                        setvbuf(BitO->File, BitO->Buffer, _IONBF, BitOutputBufferSize);
                    } else {
                        BitO->File = fopen(Path, "wb");
                        setvbuf(BitO->File, BitO->Buffer, _IONBF, BitOutputBufferSize);
                        if (BitO->File == NULL) {
                            Log(SYSCritical, &BitO->ErrorStatus->ParseOutputOptions, FopenFailed, "BitIO", "ParseOutputOptions", strerror(errno));
                        }
                    }
                    BitIOCurrentArgument = Argument + 1;
                }
            }
        }
	}

	void InitBitInput(BitInput *BitI, ErrorStatus *ES, int argc, const char *argv[]) {
		// FIXME: Remove any quotes on the path, or the issue could be that the -i and location are specified together...
		if (argc < 3) {
			PrintHelp();
		} else {
			if (BitI->ErrorStatus == NULL) {
				BitI->ErrorStatus  = ES;
			}
			BitI->SystemEndian = DetectSystemEndian();
			ParseInputOptions(BitI, argc, argv);

			fseek(BitI->File, 0, SEEK_END);
			BitI->FileSize         = (uint64_t)ftell(BitI->File);
			fseek(BitI->File, 0, SEEK_SET);
			//uint64_t Bytes2Read    = BitI->FileSize > BitInputBufferSize ? BitInputBufferSize : BitI->FileSize;
			uint64_t BytesRead     = fread(BitI->Buffer, 1, BitInputBufferSize, BitI->File);
			if (BytesRead < BitInputBufferSize) { // Bytes2Read
				Log(SYSCritical, BitI->ErrorStatus->InitBitInput, FreadReturnedTooLittleData, "BitIO", "InitBitInput", strerror(errno));
			}
			BitI->BitsAvailable    = Bytes2Bits(BytesRead);
			BitI->BitsUnavailable  = 0;
		}
	}

	void InitBitOutput(BitOutput *BitO, ErrorStatus *ES, int argc, const char *argv[]) {
		if (argc < 3) {
			PrintHelp();
		} else {
			if (BitO->ErrorStatus == NULL) {
				BitO->ErrorStatus = ES;
			}
			BitO->SystemEndian     = DetectSystemEndian();
			ParseOutputOptions(BitO, argc, argv);
			BitO->BitsAvailable    = BitOutputBufferSizeInBits;
			BitO->BitsUnavailable  = 0;
		}
	}

	void CloseBitInput(BitInput *BitI) {
		fclose(BitI->File);
		memset(BitI->Buffer, 0, Bits2Bytes(BitI->BitsUnavailable + BitI->BitsAvailable));
		free(BitI);
	}

	void CloseBitOutput(BitOutput *BitO) {
		fclose(BitO->File);
		memset(BitO->Buffer, 0, Bits2Bytes(BitO->BitsUnavailable + BitO->BitsAvailable));
		free(BitO);
	}

	void CloseBitBuffer(BitBuffer *Bits) {
		Bits->BitsAvailable   = 0;
		Bits->BitsUnavailable = 0;
		Bits->Buffer          = NULL;
		Bits->ErrorStatus     = NULL;
	}

	void UpdateInputBuffer(BitInput *BitI, int64_t RelativeOffsetInBytes) {
        uint64_t BytesRead = 0;
        fseek(BitI->File, RelativeOffsetInBytes, SEEK_CUR);
        BitI->FilePosition = ftell(BitI->File);
        //uint64_t Bytes2Read = BitI->FileSize - BitI->FilePosition > Bits2Bytes(BitI->BitsUnavailable + BitI->BitsAvailable) ? Bits2Bytes(BitI->BitsUnavailable + BitI->BitsAvailable) : BitI->FileSize - BitI->FilePosition;
        memset(BitI->Buffer, 0, BitInputBufferSize); // Bytes2Read
        BytesRead = fread(BitI->Buffer, 1, BitInputBufferSize, BitI->File); // Bytes2Read
        if (BytesRead != BitInputBufferSize) { // Bytes2Read
			Log(SYSWarning, BitI->ErrorStatus->UpdateInputBuffer, FreadReturnedTooLittleData, "BitIO", "UpdateInputBuffer", NULL);
		}
		uint64_t NEWBitsUnavailable = BitI->BitsUnavailable % 8;

        BitI->BitsUnavailable = NEWBitsUnavailable;
        BitI->BitsAvailable   = Bytes2Bits(BytesRead);
	}

	uint64_t ReadBits(BitInput *BitI, uint8_t Bits2Read) {
		uint64_t OutputData = 0;

		if ((Bits2Read <= 0) || (Bits2Read > 64)) {
			char Description[BitIOPathSize];
			snprintf(Description, BitIOPathSize, "ReadBits only supports reading 1-64 bits at a time, you tried reading: %d bits\n", Bits2Read);
			Log(SYSCritical, &BitI->ErrorStatus->ReadBits, NumberNotInRange, "BitIO", "ReadBits", Description);
		} else {
			OutputData             = NewPeekBits3(BitI, Bits2Read); // , InputEndian
			if (BitI->ErrorStatus->PeekBits != Success) {
				BitI->ErrorStatus->ReadBits  = BitI->ErrorStatus->PeekBits;
			}
			BitI->BitsUnavailable += Bits2Read;
			BitI->BitsAvailable   -= Bits2Read;
		}
		return OutputData;
	}

	void ReadRLEData(BitInput *BitI, size_t BufferSize, uint64_t *DecodedBuffer) {

	}

	void WriteRLEData(BitOutput *BitO, size_t BufferSize, uint64_t *Buffer2Encode) {

	}

	uint64_t ReadExpGolomb(BitInput *BitI, bool IsSigned, bool IsTruncated) {
		uint64_t Zeros = 0;
		uint64_t  Data = 0;
		Data = ReadBits(BitI, 1);
		if ((Zeros == 0) && (Data == 1)) { // If this is the first loop?
			while (ReadBits(BitI, 1) == 0) {
				Zeros += 1;
			}
			Data  = (1 << Zeros);
			Data += ReadBits(BitI, Zeros);
		}
		if (Zeros == 0) { // Zero case
			Data = 0;
		}
		if (IsSigned == true) {
			Data  = Unsigned2Signed(Data);
		}
		if (IsTruncated == true) {
			Data  = 2 * Zeros - 1; // Left bit first
			Data += ReadBits(BitI, Zeros);
		}
		return Data;
	}

	uint64_t ReadRICE(BitInput *BitI, bool StopBit) {
		uint64_t BitCount = 0;
		if (StopBit != (0|1)) {
			BitI->ErrorStatus->ReadRICE = NumberNotInRange;
		} else {
			while (NewPeekBits3(BitI, 1) != StopBit) { // The StopBit needs to be included in the count.
				SkipBits(BitI, 1);
				BitCount += 1;
			}
		}
		return BitCount + 1;
	}

	void WriteRICE(BitOutput *BitO, bool StopBit, uint64_t Data2Write) {
		if ((StopBit < 0) || (StopBit > 1)) {
			BitO->ErrorStatus->WriteRICE = NumberNotInRange;
		} else {
			for (uint64_t Bit = 0; Bit < Data2Write; Bit++) {
				WriteBits(BitO, (1 ^ StopBit), 1);
			}
			WriteBits(BitO, StopBit, 1);
		}
	}

	uint64_t NewPeekBits3(BitInput *BitI, uint8_t Bits2Peek) {
		int8_t Bits = Bits2Peek, UserBits = 0, SystemBits = 0, LeftShift = 0, RightShift = 0, Mask = 0, Data = 0, OutputData = 0;

		if (BitI->BitsAvailable < Bits) {
			UpdateInputBuffer(BitI, 0);
		}

		while (Bits > 0) {
			UserBits               = BitsRemaining(Bits);
			SystemBits             = BitsRemaining(BitI->BitsAvailable);
			if (UserBits < SystemBits) {
				Bits               = UserBits;
				LeftShift          = SystemBits - UserBits;
				Mask               = Power2Mask(Bits) << LeftShift;
			} else { // System bits are more constrained
				Bits               = SystemBits;
				RightShift         = UserBits - SystemBits;
				Mask               = Power2Mask(Bits);
			}

			OutputData           <<= LeftShift;
			Data                  += BitI->Buffer[BitI->BitsUnavailable / 8] & Mask;
			if (SystemBits > UserBits) {
				Data             >>= RightShift;
			}
			OutputData            += Data;
			BitI->BitsAvailable   -= Bits;
			BitI->BitsUnavailable += Bits;
			Bits                   = Bits2Peek - Bits;
		}

		BitI->BitsAvailable       += Bits2Peek;
		BitI->BitsUnavailable     -= Bits2Peek;

		return OutputData;
	}

	uint64_t NewPeekBits2(BitInput *BitI, uint8_t Bits2Peek) {
		uint64_t OutputData = 0, UserBits2Read = 0, SystemBitsAvailable = 0, UserBitsRequested = 0, SystemBits2Read = 0, CurrentByte = 0, Bits = Bits2Peek, Bits2Read = 0, Shift = 0, Mask = 0, ShiftedMask = 0, RawData = 0, LeftShift = 0, RightShift = 0;

		// The ONLY time the user request matters, is when it's less than the system has available.
		// So, if the System has fewer bits than the user wants, you go with the system, if the user wants fewer bits than the system has available, go with user + shift the mask.

		if (BitI->BitsAvailable < Bits2Peek) {
			UpdateInputBuffer(BitI, 0);
		}

		UserBits2Read       = BitsRemaining(Bits);
		SystemBitsAvailable = BitsRemaining(BitI->BitsAvailable);
		if (UserBits2Read < SystemBitsAvailable) {
			Bits2Read       = UserBits2Read;
			LeftShift       = SystemBitsAvailable - UserBits2Read;
		} else { // System bits are more constrained
			Bits2Read       = SystemBitsAvailable;
			RightShift      = UserBits2Read - SystemBitsAvailable;
		}

		while (Bits > 0) {
			SystemBitsAvailable = BitsRemaining(BitI->BitsAvailable);
			UserBitsRequested   = BitsRemaining(Bits);

			if (UserBitsRequested < SystemBitsAvailable) {
				// User mask, and invert shift the user mask.
			} else {
				// SystemMask
			}

			CurrentByte         = BitI->BitsUnavailable / 8;
			UserBits2Read       = Bits >= 8 ? 8 : Bits; // This is the user request, not the system available.
			SystemBits2Read     = UserBits2Read > BitI->BitsAvailable % 8 ? BitI->BitsAvailable % 8 : UserBits2Read;
			Mask                = UserBits2Read <  BitI->BitsAvailable % 8 ? Power2Mask(UserBits2Read) << ((BitI->BitsAvailable % 8) - UserBits2Read) : Power2Mask(UserBits2Read);
			RawData             = BitI->Buffer[CurrentByte] & Mask;
			OutputData        <<= UserBits2Read;
			OutputData         += UserBits2Read < BitI->BitsAvailable % 8 ? (RawData >>= ((BitI->BitsAvailable % 8) - UserBits2Read)) : RawData; // If Bits2Read is less than the bits availe shift.
			// But what about if the user request is more than the system can provide? That case should be handled by Bits2Read

			// Bits2Read should be as many bits the buffer can provide.
			// Bits2Read = Bits >= 8 ? 8 : Bits;
			// if Bits2Read is more than the system can provide, Bits2Read should become what the system can provide

			/*
			if (UserBits2Read > BitI->BitsAvailable % 8) { // A : B
				SystemBits2Read = BitI->BitsAvailable % 8; // X
			} else {
				SystemBits2Read = UserBits2Read;
			}
			 */

			/*
			if (Bits2Read < BitI->BitsAvailable % 8) { // A < B // Then we need to shift RawData
				OutputData += (RawData >>= ((BitI->BitsAvailable % 8) - Bits2Read));
			} else {
				OutputData += RawData;
			}
			 */
		}

		return OutputData;
	}

    uint64_t NewPeekBits(BitInput *BitI, uint8_t Bits2Peek) { // 7
        uint64_t OutputData = 0, Bits2ReadFromByte = 0, CurrentByte = 0, Bits = Bits2Peek, Shift = 0, Mask = 0, ShiftedMask = 0, RawData = 0;

        if (BitI->BitsAvailable < Bits2Peek) {
            UpdateInputBuffer(BitI, 0);
        }

        while (Bits > 0) { // 5
            CurrentByte            = BitI->BitsUnavailable / 8; // RoundUp2NearestMultiple(BitI->BitsUnavailable, 8)
            Bits2ReadFromByte      = Bits >= 8 ? 8 : Bits;

            Mask                   = Power2Mask(Bits2ReadFromByte);
            Shift                  = (BitI->BitsAvailable % 8) - Bits2ReadFromByte; // (8 - Bits2ReadFromByte); // PerfectSubtract(BitsRemaining(BitI->BitsAvailable), Bits2ReadFromByte); // is 6, should be 1

			Mask                   = Bits2ReadFromByte <  BitI->BitsAvailable % 8 ? Power2Mask(Bits2ReadFromByte) << ((BitI->BitsAvailable % 8) - Bits2ReadFromByte) : Power2Mask(Bits2ReadFromByte);


            // You'll need to shift the mask when less than 8 bits are requested on the user side, AND the user requested less bits than the mask.
            // When you do that^ you'll also need to shift the RawData

            ShiftedMask            = (Mask << Shift); // BitI->BitsUnavailable % 8 <= 4 ? Mask << (8 - Bits2ReadFromByte) : Power2Mask(Bits2ReadFromByte);
            /*
            if (BitI->BitsUnavailable <= 4) { // A : B
                ShiftedMask = Mask << Shift; // X
            } else {
                ShiftedMask = Mask; // Y
            }
             */
            //Shift                  = BitI->BitsUnavailable % 8 > 4 ? BitI->BitsUnavailable % 8 : (8 - Bits2ReadFromByte);
            // If the byte is shifted 5 spaces, there shouldn't be a second mask. if it's shifted 8 spaces, it needs to be shifted 3 spaces.

            RawData                = BitI->Buffer[CurrentByte] & ShiftedMask; // (Mask << Shift) // PerfectSubtract(BitI->BitsUnavailable % 8, Bits2ReadFromByte) // << (8 - Bits2ReadFromByte)
            //RawData              >>= Shift; // RawData >>= Shift;
            /*
            if (BitI->BitsUnavailable % 8 > 4) { // A:B
                Shift = BitI->BitsUnavailable % 8; // X
            } else {
                Shift = (8 - Bits2ReadFromByte);
            }
            */
            OutputData           <<= Bits2ReadFromByte;
            OutputData            += RawData;

            // This does not account for BitsUnavailable. Also, it shifts when it shouldn't.


            // When should the shift kick in? Only when the user request is less than what the system can provide.
            // In this MetadataBlockTpe case, it is exactly equal to the number of bits available.
            // So, in order to get it to work properly the shift should be base on the different between the System Provided, and the UserRequested.
            // For that, they should be added together?
            // If the system has 6 bits available, and the user requests 4, 6 - 4 = 2.
            // the problem comes when the user requests 5 and the system has 4.
            Bits                  -= Bits2ReadFromByte;
            BitI->BitsAvailable   -= Bits2ReadFromByte;
            BitI->BitsUnavailable += Bits2ReadFromByte;


            /*
            if (Bits2Peek < BitI->BitsAvailable % 8) {
                // You need to shift the mask
                OutputData               += BitI->Buffer[BitI->BitsUnavailable / 8] & (Power2Mask(Bits2Peek) << ((BitI->BitsAvailable % 8) - Bits2Peek));
            } else {
                // Use regular mask that only accounts for the bits available in the buffer.
                OutputData               += BitI->Buffer[BitI->BitsUnavailable / 8] & Power2Mask(BitI->BitsUnavailable);
            }
             */

        }
        BitI->ErrorStatus->PeekBits = Success;
        BitI->BitsAvailable        += Bits2Peek;
        BitI->BitsUnavailable      -= Bits2Peek;
        return OutputData;
    }

	/*
    uint64_t PeekBits(BitInput *BitI, uint8_t Bits2Peek) {
        uint8_t  BitsAvailableInCurrentByte = 0, BitsUserWants = 0, RequestShift = 0, Bits = 0;
        int8_t   BitsUsed = 0, Bits2Read = Bits2Peek;
        uint64_t RawData = 0, OutputData = 0, BitsUnavailable = 0;


        // UserMask is just stupid, like what defines the number of bits to get per loop is the number of bits available in the buffered byte, and when that's all good, THEN the user request matters.

        // The user request will always be at least 1 bit, otherwise the loop would end and the function would return.
        // So, we don't need to worry about the user mask, ALSO if there's 1 bit available in the buffer, and the user wants 1 bit, the masks will cancel each other out.

        // So, in order to get the mask to work properly, we should take the lowest of available bits, from the request and BitsAvailable, and then shift that over by the BitsAvailable?

        // ALSO, we need to read from the MSB to the LSB in a byte, aka from the left to the right.

        // if the user wants to read 8 bits, and 7 bits have already been read, then all that's left in this byte is the LSB aka 0x1.
        // so the mufucka should mask the buffer with 0x1, and extract it. and then you may need to shift the mask?


		while (Bits2Read > 0) { // (Bits2Peek > 0) && ((Bits2Peek % 8) >= 8)
            //
            BitsUsed               = BitsRemaining(BitI->BitsUnavailable); // this is somehow hardcoded to get 8 bits.
			RawData              <<= BitsUsed;

            RawData               += BitI->Buffer[BitI->BitsUnavailable / 8] & (Power2Mask(Bits2Read % 8) << (8 - Bits2Read)); // (Power2Mask(BitsRemaining(BitI->BitsUnavailable)) << (8 - BitsRemaining(BitI->BitsUnavailable)));
            Bits2Read             -= BitsUsed;
            BitI->BitsAvailable   -= BitsUsed;
            BitI->BitsUnavailable += BitsUsed; // when negative this goes out of alignment

        }
		OutputData = RawData;
        BitI->BitsAvailable       += Bits2Peek;
        BitI->BitsUnavailable     -= Bits2Peek;




        if (Bits2Peek < BitI->BitsAvailable % 8) {
            // You need to shift the mask
            RawData               += BitI->Buffer[BitI->BitsUnavailable / 8] & (Power2Mask(Bits2Peek) << ((BitI->BitsAvailable % 8) - Bits2Peek));
        } else {
            // Use regular mask that only accounts for the bits available in the buffer.
            RawData               += BitI->Buffer[BitI->BitsUnavailable / 8] & (Power2Mask(BitI->BitsUnavailable);
        }














        // Let's try reading 1 bit.

        // Bits2Read = 1.
        // BitsUsed  = 7.
        // RawData   = BitI->Buffer[0] &

        // For 1 bit, the BitsRemaining part messes it up, for 8 bits NOT having the BitsRemaining part messes it up. so what do I need to do?
        // well, the whole point of bits remaining is to calculate the number of bits that are available in the buffered byte. 1 bit is a user request thing, so it needs to be handled differently.

        // So how do I handle those competing demands? What do those 2 demands have in common? Well they both deal with how many bits should be extracted from a byte. they differe in only how many bits should be extracted, and I'll have to take the number of bits the byte can give into account when calculating ho many bits the user requests from the byte.

        // In order to take both (system available AND user request) into account, I'll need to add both together and divide by 2?

        // UserRequest = 17.

        if (Bits2Read >= 8) { // A >= B
            UserRequest = 8; // X
        } else {
            UserRequest = Bits2Read; // Y
        }

        UserRequest = Bits2Read >= 8 ? 8 : Bits2Read; // Max bits the user can accept.

        SystemAvailable = BitI->BitsUnavailable % 8;

        if (SystemAvailable < UserRequest) {
            // Then go with the SystemAvailable
        } else {
            // Go with UserRequest. btw the UserRequest has nothing to do with where the bits are selected from.
        }

        Bits2ReadFromThisByte = UserRequest < BitI->BitsUnavailable % 8 ? UserRequest : BitI->BitsUnavailable % 8; // number of bits to read this loop. This is where the mask and shifting comes into play.
        Mask = Power2Mask(Bits2ReadFromThisByte);

        // Mask = Power2Mask(Bits2ReadFromThisByte);
        // Shift = BitI->BitsUnavailable - Bits2ReadFromThisByte

        // So lets say we should read 3 bits from the buffer, and the first 5 are unavailable; that means it shouldn't be shifted at all.

        // If 1 bit has been used, and 1 bit has been requested, that bit will be gotten from 0x40, aka 0x80 shifted to the right once.

        // So the mask is shifted by (8 - (the request - system available))?

        // BY FAR THE BIGGEST PROBLEM is in selecting how the buffer is masked.

        // ; if it's 8 - Bits2ReadFromThisByte






        // ANCIENT CODE BELOW
        /*
        BitsUnavailable = BitI->BitsUnavailable;


(BitsUnavailable + Bits2Peek)
        for (uint64_t Byte = BitI->BitsUnavailable / 8; Byte < (RoundUp2NearestMultiple(Bits2Peek, 8) + BitsUnavailable); Byte++) {
            //while (Bits < Bits2Peek) {
            //for (uint64_t Byte = (BitI->BitsUnavailable % 8); Byte < ((BitI->BitsUnavailable + Bits2Peek) % 8); Byte++) {
            // if Bits2Peek are less than BitsAvailableInCurrentByte, then you'll have to shift the user mask by BAICB - bits2peek
            BitsAvailableInCurrentByte = BitsRemaining(BitI->BitsAvailable);
            OutputData <<= BitsAvailableInCurrentByte;
            RequestShift = Bits2Peek < BitsAvailableInCurrentByte ? BitsAvailableInCurrentByte : (Bits2Peek - BitsAvailableInCurrentByte) % 8;

            RawData = BitI->Buffer[Byte] & Power2Mask(BitsAvailableInCurrentByte); // << RequestShift
            // RawData = 5 bits from byte 1, but how did we arrive to that conclusion? Because 8 bits per byte - 3 bits used = 5, AND 5 is less than or equal to Bits2Peek

            BitsUserWants = Bits2Peek >= 8 ? 8 : Bits2Peek;
            if (BitsAvailableInCurrentByte > BitsUserWants) {

            }

            OutputData += RawData;

            BitI->BitsUnavailable += BitsRemaining(BitI->BitsUnavailable);
            BitI->BitsAvailable   -= BitsRemaining(BitI->BitsUnavailable);
            Bits += abs(((BitI->BitsAvailable % 8) - (Bits2Peek > 8 ? 8 : Bits2Peek)));
        }
         */

	/*
        return OutputData;
    }
	 */

	void WriteBits(BitOutput *BitO, uint64_t Data2Write, size_t NumBits) {
		if (NumBits <= 0) {
			BitO->ErrorStatus->WriteBits = TriedWritingTooFewBits;
		} else {

			if (BitO->BitsAvailable < NumBits) {
				// Write the completed bytes out, save the uncompleted ones in the array.
			}

            if (BitO->BitsAvailable < BitOutputBufferSize) { // while (BitO->BitsUnavailable >= BitOutputBufferSize) {
                fwrite(BitO->Buffer, 1, Bits2Bytes(BitO->BitsUnavailable), BitO->File);
                fflush(BitO->File);
                memcpy(&BitO->Buffer, &BitO->Buffer + Bits2Bytes(BitO->BitsUnavailable), (Bits2Bytes(BitO->BitsUnavailable + BitO->BitsAvailable) - Bits2Bytes(BitO->BitsUnavailable)));
                memset(BitO->Buffer, 0, Bits2Bytes(BitO->BitsUnavailable + BitO->BitsAvailable));
                BitO->BitsUnavailable = (BitO->BitsUnavailable - ((BitO->BitsUnavailable / 8) * 8));
            }

            for (uint64_t Bit = 0; Bit < NumBits; Bit++) {
                uint64_t X = Data2Write & BitsRemaining(BitO->BitsUnavailable);
                BitO->Buffer[Bits2Bytes(BitO->BitsUnavailable)] += X;
                BitO->BitsUnavailable++;
            }
			// FIXME: We need to just enlarge the buffer, the call a function to flush it all out to disk.
		}
	}

	void InitBitBuffer(BitBuffer *Bits, uint8_t *Buffer, size_t BufferSize) {
		Bits->BitsAvailable   = Bytes2Bits(BufferSize);
		Bits->BitsUnavailable = 0;
		Bits->Buffer = Buffer;
	}

	uint64_t ReadBitBuffer(BitBuffer *Bits, uint8_t Bits2Read) {
		uint64_t OutputData = 0, Data = 0, BitMask = 0;
		if ((Bits2Read <= 0) || (Bits2Read > 64)) {
			char ErrorDescription[BitIOPathSize] = {0};
			snprintf(ErrorDescription, BitIOPathSize, "You requested %d bits, ReadBuffer can only read 1-64 bits at a time\n", Bits2Read);
			Log(SYSError, &Bits->ErrorStatus->ReadBitBuffer, NumberNotInRange, "BitIO", "ReadBitBuffer", ErrorDescription);
		} else {
			while ((Bits2Read > 0) && (Bits->BitsAvailable >= Bits2Read)) {
				OutputData           <<= 1;
				BitMask                = 1 << (BitsRemaining(Bits->BitsUnavailable) - 1); // 1 << ((8 - (BitI->BitsUnavailable % 8)) -1);
				Data                   = Bits->Buffer[Bits2Bytes(Bits->BitsUnavailable)] & BitMask;
				Data                 >>= BitsRemaining(Bits->BitsUnavailable) - 1; // ((8 - (BitI->BitsUnavailable % 8)) -1);
				OutputData            += Data;
				Bits->BitsAvailable   -= 1;
				Bits->BitsUnavailable += 1;
				Bits2Read             -= 1;
			}
		}
		return OutputData;
	}

	void WriteBitBuffer(BitOutput *BitO, uint8_t *Buffer2Write, size_t BufferSize) { // Use BitBuffer as the backend
		uint8_t  Bits2Keep          = 0;
		uint8_t  Byte2Keep          = 0;
		uint64_t ExpandedBufferSize = 0;
		if (BitO->BitsAvailable + BitO->BitsUnavailable < Bytes2Bits(BufferSize)) {
			// It won't fit, so we have to enlarge the array.

			/* This part is just WriteBits */
			Bits2Keep = BitsRemaining(BitO->BitsUnavailable); // BitO->BitsUnavailable % 8;
			fwrite(BitO->Buffer, 1, Bits2Bytes(BitO->BitsUnavailable - Bits2Keep), BitO->File); // Write all but the uncompleted byte
			fflush(BitO->File);
			for (uint64_t Byte = Bits2Bytes(BitO->BitsUnavailable); Byte <Bits2Bytes(BitO->BitsUnavailable)+1; Byte++) {
				Byte2Keep              = BitO->Buffer[Byte];
			}
			realloc(BitO->Buffer, BufferSize);
			ExpandedBufferSize     = Bits2Bytes(BitO->BitsUnavailable + BitO->BitsAvailable) - BufferSize; // FIXME: is this correct?
			for (uint64_t Byte = 0; Byte < Bits2Bytes(BitO->BitsUnavailable + BitO->BitsAvailable); Byte++) {
				BitO->Buffer[Byte] = 0;
			}
			BitO->Buffer[0]        = Byte2Keep; // Replace the unfinished byte in the first element.
			BitO->BitsAvailable    = BitOutputBufferSizeInBits - Bits2Keep;
			BitO->BitsUnavailable  = Bits2Keep;

			for (uint64_t Byte = 0; Byte < BufferSize; Byte++) {
				for (uint8_t Bit = 0; Bit < 8; Bit++) {
					uint8_t BitData = Buffer2Write[Byte] & (1 << Bit);
					WriteBits(BitO, BitData, 1);
				}
			}
		}
	}

    int64_t PerfectSubtract(int64_t Sub1, int64_t Sub2) { // 8, 4
        int64_t Value = 0;
        if (Sub1 >= Sub2) {
            Value = Sub1 - Sub2;
        } else {
            Value = Sub2 - Sub1;
        }
        return Value;
    }

	void SkipBits(BitInput *BitI, int64_t Bits) {
		// The point of this is to seek around in the file, in case you need to jumpover parts of memory
		// if you're skipping more than one buffer of memory, you'll need to use fseek.
		if (Bits <= BitI->BitsAvailable) {
			BitI->BitsAvailable   -= Bits;
			BitI->BitsUnavailable += Bits;
		} else {
			// fseek shit here, in multiples of BitInputBufferSize
			// So we need to skip 1,078,200 bits, which is 32 times the blocksize + 29624 bits, or 3078 bytes + (BitInputBufferSize * 32)

			// 134,775 bytes to skip.

			// so just call
			fseek(BitI->File, Bits2Bytes(Bits - BitI->BitsAvailable), SEEK_CUR); // Bits2Bytes(PerfectSubtract(BitI->BitsAvailable, Bits))
            BitI->BitsAvailable   = 0;
            BitI->BitsUnavailable = 0;
            UpdateInputBuffer(BitI, 0); // Bits2Bytes(Bits)
		}

		/*
		 if ((BitI->BitsUnavailable + Bits) > BitI->BitsAvailable) {
			UpdateInputBuffer(BitI, 0);
		 } else {
			uint64_t Bytes2Read = BitI->BitsAvailable < BitOutputBufferSizeInBits ? BitI->BitsAvailable : BitOutputBufferSizeInBits;

			uint64_t Bytes2Read2 = BitI->BitsAvailable < Bytes2Bits(BitI->BufferSize) ? BitI->BitsAvailable : BitOutputBufferSizeInBits;


			BitI->FilePosition = (uint64_t)ftell(BitI->File);
			fseek(BitI->File, BitOutputBufferSizeInBits - (BitI->BitsUnavailable + Bits), SEEK_CUR);
			memset(BitI->Buffer, 0, Bits2Bytes(BitI->BitsUnavailable + BitI->BitsAvailable));
			fread(BitI->Buffer, 1, Bytes2Read, BitI->File);
		 }
		 */
	}

	uint64_t GenerateCRC(uint8_t *DataBuffer, size_t BufferSize, uint64_t Poly, uint64_t Init, uint8_t CRCSize) {
		uint64_t Output = ~Init;
		uint64_t Polynomial = 1 << CRCSize; // Implicit bit
		Polynomial += (Poly & Power2Mask(CRCSize));

		// Create a BitInput version of the data buffer, to readbits from the buffer.
		BitBuffer *CRCBits = calloc(CRCSize, 1);
		InitBitBuffer(CRCBits, DataBuffer, BufferSize);

		Init > 1 ? Power2Mask(CRCSize) : 1;

		for (size_t Byte = 0; Byte < BufferSize; Byte++) {
			uint64_t Bits2XOR = ReadBitBuffer(CRCBits, CRCSize);
			// if there aren't enough bits, simply shift to MSB to append 0s.

		}
		return Output;
	}

	bool VerifyCRC(uint8_t *DataBuffer, size_t BufferSize, uint64_t Poly, uint64_t Init, uint8_t CRCSize, uint64_t EmbeddedCRC) {
		uint64_t GeneratedCRC = GenerateCRC(DataBuffer, BufferSize, Poly, Init, CRCSize);
		if (GeneratedCRC == EmbeddedCRC) {
			return true;
		} else {
			return false;
		}
	}

	void ReadUUID(BitInput *BitI, UUIDString *UUID) { // char *UUIDString[BitIOUUIDSize]
		//static char UUIDString[21] = {0};
		for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
			if (Character == 21) {
				UUID->String[Character] = 0x00;
			} else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
				UUID->String[Character] = 0x2D;
			} else {
				UUID->String[Character] = ReadBits(BitI, 8);
			}
		}
	}

	void ReadUUIDAsGUID(BitInput *Input, UUIDString *UUID) { // char *UUIDString[BitIOUUIDSize]
		for (uint8_t Character = 0; Character < BitIOGUIDSize; Character++) {
			if (Character == 21) {
				UUID->String[Character] = 0x00;
			} else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
				UUID->String[Character] = 0x2D;
			} else {
				uint32_t Section1 = SwapEndian32(ReadBits(Input, 32));
				// Extract bytes from int into characters
				uint16_t Section2 = SwapEndian16(ReadBits(Input, 16));
				uint16_t Section3 = SwapEndian16(ReadBits(Input, 16));
			}
		}
	}

	void WriteGUIDAsUUID(BitOutput *BitO, UUIDString *UUID) { // char *GUIDString[BitIOGUIDSize]
		for (uint8_t Byte = 0; Byte < BitIOGUIDSize - 1; Byte++) { // Don't write the NULL terminator
			if ((Byte != 4) || (Byte != 7) || (Byte != 10) || (Byte != 13)) { // Hyphens

			}
		}
	}

	void WriteUUID(BitOutput *BitO, UUIDString *UUID) { // char *UUIDString[BitIOUUIDSize]
		if (strlen(UUID->String) != BitIOUUIDSize) {
			BitO->ErrorStatus->WriteUUID = WrongStringSize;
		}
		for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
			if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13) || (Character == 21)) {
				// Character 21 is the NULL terminator, the rest are the hyphens.
				WriteBits(BitO, UUID->String[Character], 8);
			}

		}
	}

	void ReadGUIDAsUUID(BitInput *BitI, UUIDString *UUID) { // char *UUIDString[BitIOGUIDSize]
		// Basically just swap the first 3 sections
		uint32_t Section1 = ReadBits(BitI, 32);
		uint16_t Section2 = ReadBits(BitI, 16);
		uint16_t Section3 = ReadBits(BitI, 16);

		uint32_t UUID1    = SwapEndian32(Section1);
		uint16_t UUID2    = SwapEndian16(Section2);
		uint16_t UUID3    = SwapEndian16(Section3);

		uint64_t Section4 = ReadBits(BitI, 64);

		for (uint8_t Byte = 0; Byte < 21; Byte++) {
			if ((Byte == 4) || (Byte == 7) || (Byte == 10) || (Byte == 13)) { // hyphen
				UUID->String[Byte] = 0x2D;
			} else if (Byte == 21) {
				UUID->String[Byte] = 0x00; // trailing NULL
			} else if ((Byte == 0) || (Byte == 1) || (Byte == 2) || (Byte == 3)) {
				UUID->String[Byte] = UUID1 & (0xFF << Bytes2Bits(Byte));
			} else if ((Byte == 5) || (Byte == 6)) {
				UUID->String[Byte] = UUID2 & (0xFF << Bytes2Bits(Byte - 5));
			} else if ((Byte == 8) || (Byte == 9)) {
				UUID->String[Byte] = UUID3 & (0xFF << Bytes2Bits(Byte - 8));
			} else if ((Byte == 11) || (Byte == 12) || (Byte == 14) || (Byte == 15) || (Byte == 15) || (Byte == 16) || (Byte == 17) || (Byte == 18)) {
				UUID->String[Byte] = Section4 & (0xFF << Bytes2Bits(Byte - 11));
			}
		}
	}

	void CompareUUIDStrings(UUIDString *UUID1, UUIDString *UUID2) { // char UUIDString2[21]
		for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize; UUIDByte++) {

		}
	}


	void Log(int64_t SYSError, ErrorStatus *ES, int64_t ESError, char Library[BitIOStringSize], char Function[BitIOStringSize], char Description[BitIOStringSize]) {
		char ComputerName[BitIOStringSize] = {0};
		size_t StringSize = 0;

		if ((SYSError == SYSPanic) || (SYSError == SYSCritical)) {
			openlog(Library, SYSError, (LOG_PERROR|LOG_MAIL|LOG_USER));
		} else {
			openlog(Library, SYSError, (LOG_PERROR|LOG_USER));
		}

		//time_t *Time;
		struct tm *Time2 = calloc(sizeof(struct tm), 1);
        char CurrentTime[26] = {0};
		//time(Time);
		StringSize = strftime(CurrentTime, 26, "%A, %B %e, %g+1000: %I:%M:%S %p %Z", Time2);
		if ((StringSize < 0) || (StringSize > BitIOStringSize)) {
			fprintf(stderr, "BitIO - Log: String too big %zu\n", StringSize);
		}

		ES = ESError;
		// ADD SYSTEM NAME TO THE BEGINNING OF EACH LOG FILE AND LOGFILE NAME.
		errno = gethostname(ComputerName, BitIOStringSize);
		if (errno != 0) {
			fprintf(stderr, "Log error: %d\n", errno);
		}
		syslog(SYSError, "%s - %s: %s - %s: %s\n", ComputerName, CurrentTime, Library, Function, Description);

        //free(&CurrentTime);
	}

	void CreateHuffmanTree(uint16_t *SymbolOccurance) {
		
	}

	void SortArrayByValue(uint16_t *Symbols[], uint16_t *Probability[], uint16_t *SortedArray, size_t NumSymbols) {
		uint16_t PreviousProbability = 0;
		uint16_t CurrentProbabiity   = 0;
		uint16_t CurrentSymbol       = 0;
		uint16_t PreviousSymbol      = 0;

		// Symbols and probabilities have the same index. So that means we just need the probability table, "symbols" are generated by the for loop.
		// So that means we generate a table of sorted symbols, so the index of the generated symbols is their commonness, and the value is the symbol.
		// In order to do that we need to loop through each symbol, and place it in the approperiate index.
		// To do THAT we need an exmple.

		/*
		 Lets say that symbol 0x7C57 = highest probability, at 192 occurances.
		 So once the for loop hit that number, we'd need to put that 
		 */


		for (uint16_t PotentialSymbol = 0; PotentialSymbol < NumSymbols; PotentialSymbol++) {
			if (PotentialSymbol == *Symbols[PotentialSymbol]) { // Symbol is actually in the table
				CurrentSymbol = PotentialSymbol;
				if (CurrentSymbol) {

				}
			}
		}






		for (uint16_t Symbol = 0; Symbol < NumSymbols; Symbol++) {
			// Go through all the probabilities and look for any runs, if any symbols have the same probability, put them in numerical order of the symbol. aka Y before Z but after X.
			CurrentProbabiity = *Probability[Symbol];
			CurrentSymbol     = Symbol;
			if (CurrentProbabiity < PreviousProbability) {
				// Move the current Probability AND symbol up a notch, otherwise down.
				SortedArray[Symbol] = *Symbols[Symbol - 1];

			} else if ((CurrentProbabiity == PreviousProbability) && (Symbol)) {
				if (Symbols[Symbol] > Symbols[Symbol - 1]) { // if the current symbol is less than the previous symbol numerically, move it down
					SortedArray[Symbol] = *Symbols[Symbol - 1];
				}
			} else {
				SortedArray[Symbol] = *Symbols[Symbol + 1];
			}
		}
	}

	/*!
	 @abstract                     "Take the symbol table and the probabiility table to generate a Huffman Tree".
	 @param    RootValue           "Should the root of the tree start at 0 or 1?".
	 */
	/*
	void GenerateHuffmanTree(uint8_t *SymbolTable[255], uint8_t *ProbabilityTable, bool RootValue) {
		uint16_t NumSymbols = sizeof(SymbolTable);
		uint8_t *SortedArray[255];
		SortArrayByValue(SymbolTable, ProbabilityTable, SortedArray, NumSymbols);


		HuffmanTree *Tree = calloc(NumSymbols, 1);

		for (uint16_t Code = NumSymbols; Code > 0; Code--) {
			// The lowest rightmost node is SymbolTable[Code] + SymbolTable[Code - 1];
		}


		for (uint16_t Symbol = 0; Symbol< NumSymbols; Symbol++) {
			Tree->Symbol[Symbol] = SortedArray[Symbol]; // The lowest symbol in this loop, is the most common symbol, therefore it should have the lowest code, and it should grow as the loop grows.
			Tree->HuffmanCode[Symbol] = what?;

		}

	}
*/

	/* Huffman Decoding functions */
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
			char String2Print[BitIOStringSize];
			snprintf(String2Print, BitIOStringSize, "One's Compliment of Length: %d != Length %d", OnesComplimentOfLength, DataLength);
			Log(SYSWarning, &BitI->ErrorStatus->DecodeHuffman, InvalidData, "BitIO", "DecodeHuffman", String2Print);
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

	uint32_t GenerateAdler32(uint8_t *Data, size_t DataSize) { // In UnitTest
		// Add all values up, then modulo it by 65521 for Sum1. byte bound.
		// Sum2 is Sum1 ran through the algorithm again.
		// Sum2 is stored before Sum1. Big Endian.

		uint32_t Adler  = 1;
		uint32_t Sum1   = Adler & 0xFFFF;
		uint32_t Sum2   = (Adler >> 16) & 0xFFFF;

		for (uint64_t Byte = 0; Byte < DataSize; Byte++) {
			Sum1 += Data[Byte] % 65521;
			Sum2 += Sum1 % 65521;
		}
		return (Sum2 << 16) + Sum1;
	}

	bool VerifyAdler32(uint8_t *Data, size_t DataSize, uint32_t EmbeddedAdler32) { // In UnitTest
		uint32_t GeneratedAdler32 = GenerateAdler32(Data, DataSize);
		if (GeneratedAdler32 != EmbeddedAdler32) {
			return false;
		} else {
			return true;
		}
	}

	Probabilities FindProbabilityFromSymbol(Probabilities *Probability, double *MaximumTable, double *MinimumTable, size_t TableSize, uint64_t Symbol2Lookup) {
		if (Symbol2Lookup > TableSize) {
			// Not good.
		} else {
			Probability->Maximum = MaximumTable[Symbol2Lookup];
			Probability->Minimum = MinimumTable[Symbol2Lookup];
		}
		return *Probability;
	}

	// Create a function to lookup the symbol from the probabilities
	uint16_t FindSymbolFromProbability(double Probability, uint64_t	*MaximumTable, uint64_t *MinimumTable, size_t TableSize) {
		uint16_t Symbol = 0; // there is a SINGLE probability, not two...
							 // If the probability is closer to 1 than 0, start the loop at 1, instead of 0. otherwise, start it at 0. to ensure it takes half the time to traverse it.

		bool WhichEnd = round(Probability);

		if (WhichEnd == 0) {
			for (uint64_t Index = 0; Index < TableSize; Index++) {
				uint64_t MaxProb   = MaximumTable[Index];
				uint64_t MinProb   = MinimumTable[Index];
				if ((Probability  >= MinProb) && (Probability <= MaxProb)) { // You found the symbol!
					Symbol = Index;
				}
			}
		} else {
			for (uint64_t Index = TableSize; Index > 0; Index--) {
				uint64_t MaxProb   = MaximumTable[Index];
				uint64_t MinProb   = MinimumTable[Index];
				if ((Probability  >= MinProb) && (Probability <= MaxProb)) { // You found the symbol!
					Symbol = Index;
				}
			}
		}
		return Symbol;
	}

	uint64_t ReadArithmetic(BitInput *Input, uint64_t *MaximumTable, uint64_t *MinimumTable, size_t TableSize, uint64_t Bits2Decode) {
		double Maximum = 1.0;
		double Minimum = 0.0;
		double Range   = 0.0;

		uint64_t Symbol = 0;

		double Probability = ReadBits(Input, 1); // this is just a mockup.


		while (Bits2Decode > 0) { // No, this needs to be rethought
			Range = Maximum - Minimum;
			uint64_t Symbol = FindSymbolFromProbability(Probability, MaximumTable, MinimumTable, TableSize);
			Maximum = Minimum + Range; // * p.second
			Minimum = Minimum + Range; // * p.first
		}
		return Symbol;
	}

	void WriteArithmetic(BitOutput *BitO, Probabilities *Probability, uint64_t Bits2Encode) { // Use the least precision you can get away with to be as efficent as possible.
		double Maximum = 1.0;
		double Minimum = 0.0;
		double Range   = 0.0;

		FindProbabilityFromSymbol(Probability, NULL, NULL, 65535, 0xAB37);

		while (Bits2Encode > 0) {
			Range         = Probability->Maximum - Probability->Minimum;
			Maximum       = Probability->Minimum + Range;
			Minimum       = Probability->Minimum + Range;
		}

	}

#ifdef __cplusplus
}
#endif
