#include "../include/BitIO.h"

/*
uint64_t MyRand(uint64_t Minimum, uint64_t Maximum) {
	uint64_t Hole;
	uintptr_t *HolePointer = Hole;
	if (Hole == 0) {
		HolePointer += 8;
		Hole = *HolePointer;
	}
	return Hole;
}
 */

int main(int argc, const char *argv[]) {
    BitInput  *BitI = calloc(sizeof(BitInput), 1);
    BitOutput *BitO = calloc(sizeof(BitOutput), 1);
    ErrorStatus *ES = calloc(sizeof(ErrorStatus), 1);


    InitBitInput(BitI, ES, argc, argv);
    InitBitOutput(BitO, ES, argc, argv);

    /*
    BitI->BitsAvailable   = (BitInputBufferSize * 8) - 3;
    BitI->BitsUnavailable = 2;
     */

    uint64_t Data = 0;

    for (uint8_t Wat = 1; Wat < 64; Wat++) {
        Data = ReadBits(BitI, Wat);
        printf("BitsAvailable: %llu, BitsUnavailable: %llu, Bits2Read: %d, Result: 0x%llx\n", BitI->BitsAvailable, BitI->BitsUnavailable, Wat, Data);
    }

    /*
	//memset(BitI->Buffer, 0xFF, BitInputBufferSize);
	uint64_t What[8] = {0};
	for (uint8_t Run = 0; Run < 8; Run++) { // Loop over ReadBits to test it.
		uint8_t Bits2Read = MyRand(1, 64);
		uint64_t OutputData = ReadBits(BitI, Bits2Read);
		printf("ReadBits Test: Bits2Read = %d, OutputData = 0x%llx", Bits2Read, What[Run]);
	}
    */
    /*
	//ES->AlignBits2Byte = 42;
    
	//InitBitInput(BitI, ES, argc, argv);
	//InitBitOutput(BitO, ES, argc, argv);
    
    typedef struct TwelveBitColor { // uint16_t
		/*
        uint16_t   Red : 12;
        uint16_t Green : 12;
        uint16_t  Blue : 12;
		 
		
		unsigned Red : 12, Green : 12, Blue : 12;
		
    } TwelveBitColor;
    
    TwelveBitColor *Color12 = malloc(sizeof(TwelveBitColor));
    Color12->Red = 0xFFF;
    printf("TwelveBitColor Size: %lu\n", sizeof(TwelveBitColor));
    printf("Red: %d\n", Color12->Red);
    
    /*
    uint8_t ArrayTest[25][25] = {{0},{0}};
    for (uint8_t Row1 = 0; Row1 < 25; Row1++) {
        for (uint8_t Row2 = 0; Row2 < 25; Row2++) {
            ArrayTest[Row1][Row2] = Row2;
            printf("ArrayTest: Row1 = %d, Row2 = %d, ArrayTest = %d\n", Row1, Row2, ArrayTest[Row1][Row2]);
        }
    }
    
    uint8_t ArrayTest2[24] = {0};
    for (size_t Element = 0; Element < 12; Element++) {
        ArrayTest2[Element] = (uint8_t)rand();
    }
    
    printf("Size of ArrayTest2 (24 elements, 12 used): %lu\n", sizeof(ArrayTest2));
    
    // Sizeof isn't intelligent enough to measure the number of used bytes, so I'll have to write my own function.
    */
    
    /*
    BitI->BitsAvailable   = 32762;
    BitI->BitsUnavailable = 6;
    
    //printf("BitIO: BitI->BitsUnavailable = %llu\n", BitI->BitsUnavailable);
    
    uint16_t Test = 0xFF;
    Test <<= 8;
    Test += 12;
    printf("Plus-equal Test %d\n", Test);
	
    uint64_t wat = PeekBits(BitI, 64); // When 64 it messes up. have to shift the inital value.
	printf("wat = 0x%llx\n", wat);
	
    // I want to open 2 input files, and 1 output file.
    
    // Those input files will be chosen by glob searching the directory for all files ending with .png, and ordering them by number.
    // Each file will have a second partnering file, the file names varying by wether they end in _Right or _Left.
    // in order to do this glob needs a variable to hold the current file num, and the max file num.
     */
    /*
    uint64_t ReedBits = ReadBits(BitI, 7);
    printf("ReedBits 7: %llu\n", ReedBits);
    
    uint64_t BitsRemaining1 = BitsRemaining(1);
    printf("Bits Remaining from 1: %llu\n", BitsRemaining1);
	 */
    
    return 0;
}
