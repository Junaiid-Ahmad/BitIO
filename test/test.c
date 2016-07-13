#include "../libBitIO/include/BitIO.h"

int main(int argc, const char *argv[]) {
    BitInput  *BitI = malloc(sizeof(BitInput));
    BitOutput *BitO = malloc(sizeof(BitOutput));
    
    InitBitInput(BitI, argc, argv);
    InitBitOutput(BitO, argc, argv);
    
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
    /*
    uint64_t ReedBits = ReadBits(BitI, 7);
    printf("ReedBits 7: %llu\n", ReedBits);
    
    uint64_t BitsRemaining1 = BitsRemaining(1);
    printf("Bits Remaining from 1: %llu\n", BitsRemaining1);
	 */
    
    return 0;
}
