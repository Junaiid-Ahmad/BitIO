#include "../libBitIO/include/BitIO.h"

int main(int argc, const char *argv[]) {
    BitInput  *BitI = calloc(sizeof(BitInput), 1);
    BitOutput *BitO = calloc(sizeof(BitOutput), 1);
    ErrorStatus *ES = calloc(sizeof(ErrorStatus), 1);
    
	//ES->AlignBits2Byte = 42;
    
	//InitBitInput(BitI, ES, argc, argv);
	//InitBitOutput(BitO, ES, argc, argv);
    
    typedef struct TwelveBitColor { // uint16_t
		/*
        uint16_t   Red : 12;
        uint16_t Green : 12;
        uint16_t  Blue : 12;
		 */
		
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
