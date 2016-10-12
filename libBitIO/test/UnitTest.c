#include "../include/BitIO.h"

typedef struct CRC {
	uint64_t  Polynomial;
	uint64_t  Initializer;
	size_t    CRCSize;
	bool      InputIsReflected;
	bool      OutputIsReflected;
	uint64_t  XOROutput;
} CRC;

typedef enum CRCTypes {
	CRC32          = 0,
	CRC16_CCITT    = 1,
	CRC16_USB      = 2,
	CRC16_CDMA2000 = 3,
} CRCTypes;

void InitalizeCRC(char CRCType[BitIOStringSize], CRC *CRC) {
	if (strcasecmp(CRCType, "CRC16_USB") == 0) {
		CRC->Polynomial        = 0xC002;
		CRC->Initializer       = 0xFFFF;
		CRC->CRCSize           = 16;
		CRC->InputIsReflected  = true;
		CRC->OutputIsReflected = true;
		CRC->XOROutput         = 0xFFFF;
	} else if (strcasecmp(CRCType, "CRC32") == 0) {
		CRC->Polynomial        = 0x82608EDB;
		CRC->Initializer       = 0xFFFFFFFF;
		CRC->CRCSize           = 32;
		CRC->InputIsReflected  = true;
		CRC->OutputIsReflected = true;
		CRC->XOROutput         = 0xFFFFFFFF;
	} else if (strcasecmp(CRCType, "CRC16_CDMA2000") == 0) {
		CRC->Polynomial        = 0xC002;
		CRC->Initializer       = 0xFFFF;
		CRC->CRCSize           = 16;
		CRC->InputIsReflected  = true;
		CRC->OutputIsReflected = true;
		CRC->XOROutput         = 0xFFFF;
	} else if (strcasecmp(CRCType, "CRC16_CCITT") == 0) {
		CRC->Polynomial        = 0x8810;
		CRC->Initializer       = 0xFFFF;
		CRC->CRCSize           = 16;
		CRC->InputIsReflected  = false;
		CRC->OutputIsReflected = false;
		CRC->XOROutput         = 0x0000;
	}
}

uint8_t RandomData[64] = { // Data for verifying hashing functions
	0xD5, 0xFD, 0x7F, 0x1C, 0xB4, 0x9F, 0x47, 0x9C,
	0xBA, 0xAB, 0x52, 0xCE, 0x51, 0xDF, 0x0E, 0x27,
	0x3F, 0xAB, 0x6A, 0x28, 0xE0, 0x41, 0x55, 0x47,
	0xC1, 0x21, 0x0A, 0xA6, 0x0D, 0xC1, 0x1A, 0xD3,
	0x14, 0x5F, 0xE8, 0x23, 0x18, 0x6C, 0x93, 0xB1,
	0xAD, 0xCE, 0x73, 0x31, 0x64, 0x33, 0x6C, 0x5F,
	0xF9, 0xF3, 0x47, 0x77, 0x52, 0xF0, 0xC6, 0xE3,
	0xB2, 0xE2, 0xEB, 0x42, 0xC6, 0x8F, 0x14, 0xBB
};

bool Test_PeekBits(BitInput *Input) { // This should cover basically everything dealing with BitInput
	bool Passed = 0;
	for (uint8_t Bits2Peek = 0; Bits2Peek < 64; Bits2Peek++) {
		uint64_t PeekedData = PeekBits(Input, Bits2Peek);
		if (PeekedData != Power2Mask(Bits2Peek)) {
			char Description[BitIOStringSize];
			snprintf(Description, BitIOStringSize, "PeekBits fucked up big time on %d", Bits2Peek);
			Log(SYSCritical, &Input->ErrorStatus->PeekBits, InvalidData, "BitIO", "PeekBits", Description);
			Passed = false;
		} else {
			Passed = true;
		}
	}
	return Passed;
}

bool Test_ReadExpGolomb(BitInput *Input) {
	return false;
}

bool Test_Adler32(BitInput *Input) { // This will test both GenerateAdler32 and VerifyAdler32
									 // FIXME: I may need to swap endian...
	
	uint32_t ConfirmedAdler32 = 0xF6532055;
	
	uint32_t GeneratedAdler32 = GenerateAdler32(&RandomData[64], 64);
	if (GeneratedAdler32 != ConfirmedAdler32) {
		char ErrorString[BitIOStringSize] = {0};
		snprintf(ErrorString, BitIOStringSize, "Adler32 failed, generated: %x, should've been: %x", GeneratedAdler32, ConfirmedAdler32);
		Log(SYSError, NULL, NULL, "BitIO", "Test_Adler32", ErrorString);
		return false;
	} else {
		return true;
	}
}

bool Test_CRC(BitInput *Input) { // This will test both GenerateCRC and VerifyCRC
	uint32_t VerifiedCRC = 0xBA5370DA;
	
	// Loop throught a variety of polys, and put the log if inside the loop
	uint32_t GeneratedCRC = GenerateCRC(&RandomData[64], 64, 0x82608EDB, 0xFFFFFFFF, 32); // PNG CRC Poly
	if (GeneratedCRC != VerifiedCRC) {
		char Description[BitIOStringSize] = {0};
		snprintf(Description, BitIOStringSize, "Poly: %x, Init: %x, CRCSize: %d\n", 0x82608EDB, 0xFFFFFFFF, 32);
		Log(SYSError, NULL, InvalidCRC, "BitIO", "Test_CRC", Description);
		return false; // test failed
	} else {
		return true;
	}
}

bool Test_SwapEndian16() {
	uint16_t Swapped = SwapEndian16(0x7FFF);
	if (Swapped == 0xFF7F) {
		// test sucessful
		return true;
	} else {
		return false;
	}
}

bool Test_SwapEndian32() {
	uint32_t Swapped = SwapEndian32(0xBFFFFFF7);
	if (Swapped == 0xF7FFFFBF) {
		// test sucessful
		return true;
	} else {
		return false;
	}
}

bool Test_SwapEndian64() {
	uint64_t Swapped = SwapEndian64(0xDFFFFFFFFFFFBFFF);
	if (Swapped == 0xFFBFFFFFFFFFFFDF) {
		// test sucessful
		return true;
	} else {
		return false;
	}
}

bool Test_Signed2Unsigned() {
	int64_t Unsigned = Signed2Unsigned(-128);
	if (Unsigned != 0) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_Unsigned2Signed() {
	uint64_t Signed = Unsigned2Signed(255);
	if (Signed != 127) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_2sComplimentTo1sCompliment() {
	int64_t OnesCompliment = TwosCompliment2OnesCompliment(-64);
	if (OnesCompliment != 0xFFFFFFFFFFFFFFC0) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_1sComplimentTo2sCompliment() {
	int64_t TwosCompliment = OnesCompliment2TwosCompliment(0xFFFFFFFFFFFFFFC0); // -64
	if (TwosCompliment != -64) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_StreamAlignment() {
	bool IsAligned = IsStreamByteAligned(7, 1);
	if (IsAligned != false) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_AlignInput(BitInput *BitI) {
	BitI->BitsAvailable   = 63;
	BitI->BitsUnavailable =  1;
	AlignInput(BitI, 4); // 4 byte alignment, aka 32 bits
	if ((BitI->BitsAvailable != 32) && (BitI->BitsUnavailable != 32)) {
		// test failed
		return false;
	} else {
		return true;
	}
}

bool Test_AlignOutput(BitOutput *BitO) {
	BitO->BitsAvailable   = 63;
	BitO->BitsUnavailable =  1;
	AlignOutput(BitO, 4);
	if ((BitO->BitsAvailable != 32) && (BitO->BitsUnavailable != 32)) {
		// test failed
		return false;
	} else {
		return true;
	}
}

// Static Huffman table

// static Huffman data is Randomdata from above encoded with Huffman

void Test_StaticHuffman() {

}

void Test_All(BitInput *Input, BitOutput *Output) {
	Test_PeekBits(Input);
	Test_Adler32(Input);
	Test_CRC(Input);
	Test_SwapEndian16();
	Test_SwapEndian32();
	Test_SwapEndian64();
	Test_Signed2Unsigned();
	Test_Unsigned2Signed();
	Test_2sComplimentTo1sCompliment();
	Test_1sComplimentTo2sCompliment();
	Test_StreamAlignment();
	Test_AlignInput(Input);
	Test_AlignOutput(Output);
}

int main(int argc, const char *argv[]) {
	BitInput    *TestInput  = calloc(sizeof(BitInput), 1);
	BitOutput   *TestOutput = calloc(sizeof(BitOutput), 1);
	ErrorStatus *ES         = calloc(sizeof(ErrorStatus), 1);
	
	// Fake argv
	memset(TestInput->Buffer, 0xFF, BitInputBufferSize); // Think of it as solid state initalization.
	
	for (uint8_t Argument = 1; Argument < 4; Argument++) {
		if (Argument == 1) {
			argv[Argument] = "-i";
		} else if (Argument == 2) {
			argv[Argument] = "/dev/null";
		} else if (Argument == 3) {
			argv[Argument] = "-o";
		} else if (Argument == 4) {
			argv[Argument] = "/dev/null";
		}
	}
	
	InitBitInput(TestInput, ES, argc, argv);
	InitBitOutput(TestOutput, ES, argc, argv);
	
	Test_All(TestInput, TestOutput);
	
	return 0;
}
