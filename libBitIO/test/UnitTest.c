#include "../include/BitIO.h"

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

void Test_PeekBits(BitInput *Input) { // This should cover basically everything dealing with BitInput
	for (uint8_t Bits2Peek = 0; Bits2Peek < 64; Bits2Peek++) {
		uint64_t PeekedData = PeekBits(Input, Bits2Peek);
		if (PeekedData != Power2Mask(Bits2Peek)) {
			char Description[BitIOStringSize];
			snprintf(Description, BitIOStringSize, "PeekBits fucked up big time on %d", Bits2Peek);
			Log(SYSCritical, &Input->ErrorStatus->PeekBits, InvalidData, "BitIO", "PeekBits", Description);
		}
	}
}

void Test_ReadExpGolomb(BitInput *Input) {
	
}

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

void Test_Adler32(BitInput *Input) { // This will test both GenerateAdler32 and VerifyAdler32
									 // FIXME: I may need to swap endian...
	
	uint32_t ConfirmedAdler32 = 0xF6532055;
	
	uint32_t GeneratedAdler32 = GenerateAdler32(&RandomData[64], 64);
	if (GeneratedAdler32 != ConfirmedAdler32) {
		char ErrorString[BitIOStringSize] = {0};
		snprintf(ErrorString, BitIOStringSize, "Adler32 failed, generated: %x, should've been: %x", GeneratedAdler32, ConfirmedAdler32);
		Log(SYSError, NULL, NULL, "BitIO", "Test_Adler32", ErrorString);
	}
}

void Test_CRC(BitInput *Input) { // This will test both GenerateCRC and VerifyCRC
	uint32_t VerifiedCRC = 0xBA5370DA;
	
	// Loop throught a variety of polys, and put the log if inside the loop
	uint32_t GeneratedCRC = GenerateCRC(&RandomData[64], 64, 0x82608EDB, 0xFFFFFFFF, 32); // PNG CRC Poly
	if (GeneratedCRC != VerifiedCRC) {
		char Description[BitIOStringSize] = {0};
		snprintf(Description, BitIOStringSize, "Poly: %x, Init: %x, CRCSize: %d\n", 0x82608EDB, 0xFFFFFFFF, 32);
		Log(SYSError, NULL, InvalidCRC, "BitIO", "Test_CRC", Description);
	}
	
}

void Test_All(BitInput *Input, BitOutput *Output) {
	Test_PeekBits(Input);
	Test_Adler32(Input);
	Test_CRC(Input);
}

int main(int argc, const char *argv[]) {
	BitInput    *TestInput  = calloc(sizeof(BitInput), 1);
	BitOutput   *TestOutput = calloc(sizeof(BitOutput), 1);
	ErrorStatus *ES         = calloc(sizeof(ErrorStatus), 1);
	
	// Fake argv
	for (uint64_t Byte = 0; Byte < BitInputBufferSize; Byte++) {
		TestInput->Buffer[Byte] = 0xFF; 
		// Init Buffer to 0xFF, so we don't have to store and load a external file.
		// Think of it as solid state initalization. 
	}
	
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
