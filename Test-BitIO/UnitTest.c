#include "../libBitIO/include/BitIO.h"

#ifdef _WIN32
#define strcasecmp stricmp
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct CRCTest {
        uint64_t  Polynomial;
        uint64_t  Initializer;
        size_t    CRCSize;
        bool      InputIsReflected;
        bool      OutputIsReflected;
        uint64_t  XOROutput;
    } CRCTest;
    
    typedef enum CRCTypes {
        CRC32          = 0,
        CRC16_CCITT    = 1,
        CRC16_USB      = 2,
        CRC16_CDMA2000 = 3,
    } CRCTypes;
    
    void InitalizeCRC(char CRCType[BitIOStringSize], CRCTest *CRC) {
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
    
    uint8_t RandomBits2Peek[64] = {
        6, 61, 20, 57, 28,  9, 37, 33, 42, 62, 18, 43, 53, 62, 61, 13,
        14, 50, 37, 37, 53, 62, 15, 35,  1, 14, 43, 46, 61,  8, 24, 45,
        38, 24,  2, 38, 43, 48, 39, 37, 48,  7, 59, 47, 44,  6,  9, 53,
        31, 63, 56,  6, 61, 59, 42, 26, 36, 63, 43, 21, 48, 43, 10, 42
    };
    
    uint16_t RandomBitsAvailable[64] = {
        0x3b1a, 0x5968, 0x5edd, 0x51ec, 0x6faf, 0x2cd1, 0x491a, 0x230a,
        0x5a3f, 0x35ca, 0x25a3, 0x08ee, 0x5b96, 0x1d02, 0x21ac, 0x1b57,
        0x75d8, 0x358b, 0x0f51, 0x70bf, 0x6921, 0x571c, 0x35e0, 0x3cfa,
        0x0b22, 0x639c, 0x3f1d, 0x5756, 0x59ec, 0x05bc, 0x63db, 0x6b37,
        0x1fd0, 0x4b96, 0x526e, 0x47bf, 0x4c9c, 0x4c5f, 0x6e60, 0x5308,
        0x09c2, 0x04d1, 0x464e, 0x61dd, 0x4f60, 0x45fb, 0x60d8, 0x3ea6,
        0x56ad, 0x403c, 0x28e5, 0x794a, 0x6c87, 0x699f, 0x5090, 0x761e,
        0x2b09, 0x05d3, 0x053c, 0x79a0, 0x11a5, 0x3585, 0x7d20, 0x17e3
    };
    
    bool Test_ReadBits(BitInput *Input) { // This should cover basically everything dealing with BitInput
        bool Passed = 0;
        uint64_t Correct = 0LLU;
        uint8_t Bits2Read = (rand() % 64);
        
        Input->BitsAvailable   -= RandomBitsAvailable[Bits2Read];
        Input->BitsUnavailable += RandomBitsAvailable[Bits2Read];
        
        for (uint8_t Bits2Peek = 0; Bits2Peek < 65; Bits2Peek++) {
            Bits2Read = RandomBits2Peek[Bits2Peek];
            uint64_t PeekedData = ReadBits(Input, Bits2Read, false);
            Correct = Power2Mask(Bits2Read);
            if (PeekedData != Correct) {
                Passed = false;
                
                char Description[BitIOStringSize];
                snprintf(Description, BitIOStringSize, "ReadBits fucked up big time on %d\n", Bits2Peek);
                Log(LOG_EMERG, "BitIO", "ReadBits", Description);
                printf("\nERROR!\n");
                printf("Bits2Peek: %1d, BitsUnavailable: %0zU, Result: 0x%llX, Correct: 0x%llX\n", Bits2Read, Input->BitsUnavailable, PeekedData, Correct);
                printf("!ERROR \n");
            } else {
                Passed = true;
            }
        }
        return Passed;
    }
    
    bool Test_ReadExpGolomb(BitInput *Input) {
        return false;
    }
    
    bool Test_Adler32(BitInput *Input) { // FIXME: I may need to swap endian...
        uint32_t ConfirmedAdler32 = 0xF6532055;
        
        uint32_t GeneratedAdler32 = GenerateAdler32(&RandomData[64], 64);
        if (GeneratedAdler32 != ConfirmedAdler32) {
            char ErrorString[BitIOStringSize] = {0};
            snprintf(ErrorString, BitIOStringSize, "Adler32 failed, generated: %x, should've been: %x", GeneratedAdler32, ConfirmedAdler32);
            Log(LOG_ERR, "BitIO", "Test_Adler32", ErrorString);
            return false;
        } else {
            return true;
        }
    }
    
    bool Test_CRC(BitInput *Input, CRC *CRCData) { // This will test both GenerateCRC and VerifyCRC
        uint32_t VerifiedCRC = 0xBA5370DA;
        
        // Loop throught a variety of polys, and put the log if inside the loop
        uint32_t GeneratedCRC = GenerateCRC(Input, 64, CRCData); // &RandomData[64], 64, 0x82608EDB, 0xFFFFFFFF, 32
        if (GeneratedCRC != VerifiedCRC) {
            char Description[BitIOStringSize] = {0};
            snprintf(Description, BitIOStringSize, "Poly: %x, Init: %x, CRCSize: %d\n", 0x82608EDB, 0xFFFFFFFF, 32);
            Log(LOG_ERR, "BitIO", "Test_CRC", Description);
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
    
    void Test_All(BitInput *Input, BitOutput *Output) {
        bool TestPassed = 0;
        CRC *PNGCRC = calloc(sizeof(CRC), 1);
        
        TestPassed  = Test_ReadBits(Input);
        if (TestPassed == false) {
            printf("Test_ReadBits failed!!!\n");
        } else {
            printf("Test_ReadBits passed!\n");
        }
        TestPassed  = Test_Adler32(Input);
        if (TestPassed == false) {
            printf("Test_Adler32 failed!!!\n");
        } else {
            printf("Test_Adler32 passed!\n");
        }
        TestPassed  = Test_CRC(Input, PNGCRC);
        if (TestPassed == false) {
            printf("Test_CRC PNGCrc failed!!!\n");
        }
        TestPassed  = Test_SwapEndian16();
        if (TestPassed == false) {
            printf("Test_SwapEndian16 failed!!!\n");
        }
        TestPassed  = Test_SwapEndian32();
        if (TestPassed == false) {
            printf("Test_SwapEndian32 failed!!!\n");
        }
        TestPassed  = Test_SwapEndian64();
        if (TestPassed == false) {
            printf("Test_SwapEndian64 failed!!!\n");
        }
        TestPassed  = Test_Signed2Unsigned();
        if (TestPassed == false) {
            printf("Test_Signed2Unsigned failed!!!\n");
        }
        TestPassed  = Test_Unsigned2Signed();
        if (TestPassed == false) {
            printf("Test_Unsigned2Signed failed!!!\n");
        }
        TestPassed  = Test_2sComplimentTo1sCompliment();
        if (TestPassed == false) {
            printf("Test_2sComplimentTo1sCompliment failed!!!\n");
        }
        TestPassed  = Test_1sComplimentTo2sCompliment();
        if (TestPassed == false) {
            printf("Test_1sComplimentTo2sCompliment failed!!!\n");
        }
        TestPassed  = Test_StreamAlignment();
        if (TestPassed == false) {
            printf("Test_StreamAlignment failed!!!\n");
        }
        TestPassed  = Test_AlignInput(Input);
        if (TestPassed == false) {
            printf("Test_AlignInput failed!!!\n");
        }
        TestPassed  = Test_AlignOutput(Output);
        if (TestPassed == false) {
            printf("Test_AlignOutput failed!!!\n");
        }
        free(PNGCRC);
    }
    
    void SetUnitTestOptions(CommandLineOptions *CMD) {
        CMD->NumSwitches                  = 2;
        CMD->ProgramName                  = "Unit-Test";
        CMD->ProgramDescription           = "Test's BitIO's various functions for runtime errors";
        CMD->AuthorCopyrightLicense       = "By BumbleBritches57, © 2017, Released under the BSD 3-clause license";
        
        CLSwitch *Switch0                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch1                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch2                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch3                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch4                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch5                 = calloc(sizeof(CLSwitch), 1);
        CLSwitch *Switch6                 = calloc(sizeof(CLSwitch), 1);
        CMD->Switch[0]                    = Switch0;
        CMD->Switch[1]                    = Switch1;
        CMD->Switch[2]                    = Switch2;
        CMD->Switch[3]                    = Switch3;
        CMD->Switch[4]                    = Switch4;
        CMD->Switch[5]                    = Switch5;
        CMD->Switch[6]                    = Switch6;
        
        CMD->Switch[0]->Switch            = "-i";
        CMD->Switch[0]->SwitchDescription = "Input file or stdin with: '-'";
        
        CMD->Switch[1]->Switch            = "-o";
        CMD->Switch[1]->SwitchDescription = "Output file or stdout with: '-'";
    }
    
    int main(int argc, const char *argv[]) {
        if (argc < 4) {
            printf("Usage: -i INPUT -o OUTPUT\n");
            exit(EXIT_FAILURE);
        } else {
            BitInput           *TestInput  = calloc(sizeof(BitInput), 1);
            BitOutput          *TestOutput = calloc(sizeof(BitOutput), 1);
            CommandLineOptions *CMD        = calloc(sizeof(CommandLineOptions), 1);
            
            OpenCMDInputFile(TestInput, CMD, 0);
            OpenCMDOutputFile(TestOutput, CMD, 1);
            
            Test_All(TestInput, TestOutput);
            
            CloseBitInput(TestInput);
            CloseBitOutput(TestOutput);
        }
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
