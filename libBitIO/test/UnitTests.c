#include "../include/BitIO.h"

void Test_All(BitInput *Input, BitOutput *Output) {
	
}

void Test_PeekBits(BitInput *Input) {
	for (uint8_t Bits2Peek = 0; Bits2Peek < 64; Bits2Peek++) {
		uint64_t PeekedData = PeekBits(Input, Bits2Peek);
		if (PeekedData != Power2Mask(Bits2Peek)) {
			Log(Input->ErrorStatus, Testing, libBitIO, PeekBits, <#char Description#>, <#uint8_t ESError#>);
		}
	}
}

int main(int argc, const char *argv[]) {
	BitInput    *TestInput  = calloc(sizeof(BitInput), 1);
	BitOutput   *TestOutput = calloc(sizeof(BitOutput), 1);
	ErrorStatus *ES         = calloc(sizeof(ErrorStatus), 1);
	
	// Fake argv
	for (uint64_t Byte = 0; Byte < BitInputBufferSize; Byte++) {
		TestInput->Buffer[Byte] = 0xFF; // Init Buffer to 0xFF, so we don't have to store and load a external file.
										// Think of it as solid state initalization. 
	}
	
	for (uint8_t Argument = 1; Argument < 4; Argument++) {
		if (Argument == 1) {
			argv[Argument] = "-i";
		}
		if (Argument == 2) {
			argv[Argument] = "/dev/null"
		}
		if (Argument == 3) {
			argv[Argument] = "-o";
		}
		if (Argument == 4) {
			argv[Argument] = "/dev/null";
		}
	}
	
	InitBitInput(TestInput, ES, argc, argv);
	InitBitOutput(TestOutput, ES, argc, argv);
	
	Test_All();
	
	return 0;
}
