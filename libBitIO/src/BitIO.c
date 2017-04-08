#include "../include/BitIO.h"

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <sys/socket.h>
#include <syslog.h>
#include <unistd.h>
#else
#include <winbase.h>
#include <winsock.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
#ifdef _WIN32
#define strcasecmp stricmp
#endif
    
    /*!
     @typedef           BitBuffer
     @abstract                              "Contains variables and a pointer to a buffer for reading and writing bits".
     @constant          BitsAvailable       "The number of bits available for reading".
     @constant          BitsUnavailable     "The number of bits previously read, or available for writing".
     @constant          Buffer              "A pointer to an unsigned byte buffer".
     */
    typedef struct BitBuffer {
        size_t             BitsAvailable;
        size_t             BitsUnavailable;
        uint8_t           *Buffer;
    } BitBuffer;
    
    /*!
     @typedef           BitInput
     @abstract                              "Contains File/Socket pointers for reading to a BitBuffer.
     @constant          File                "Input File/Socket to read into a BitBuffer".
     @constant          FileSize            "Size of the File in bytes".
     @constant          FilePosition        "Current byte in the file".
     @constant          SystemEndian        "Endian of the running system".
     @constant          BitB                "Pointer to an instance of BitBuffer".
     */
    typedef struct BitInput {
        FILE              *File;
        size_t             FileSize;
        size_t             FilePosition;
        uint8_t            SystemEndian:2;
        BitBuffer         *BitB;
    } BitInput;
    
    /*!
     @typedef           BitOutput
     @abstract                              "Contains File/Socket pointers for writing from a BitBuffer".
     @constant          File                "Input File/Socket to write a BitBuffer into".
     @constant          FilePosition        "Current byte in the file".
     @constant          SystemEndian        "Endian of the running system".
     @constant          BitB                "Pointer to an instance of BitBuffer".
     */
    typedef struct BitOutput {
        FILE              *File;
        size_t             FilePosition;
        uint8_t            SystemEndian:2;
        BitBuffer         *BitB;
    } BitOutput;
    
    /*!
     @typedef           CommandLineSwitch
     @abstract                              "Contains the data to support a single switch".
     @remark                                "You MUST include the NULL padding at the end of @Switch".
     @constant          SwitchFound         "If the switch was found in argv, this will be set to true".
     @constant          Resultless          "Is the mere presence of the switch what you're looking for? if so, set to true"
     @constant          Flag                "Actual flag, WITHOUT dash(s) or backslash, Flags are case insensitive".
     @constant          SwitchDescription   "Message to print explaining what the switch does".
     @constant          SwitchResult        "String to contain the result of this switch, NULL if not found".
     */
    typedef struct CommandLineSwitch {
        bool               SwitchFound;
        bool               Resultless;
        const char        *Flag;
        const char        *SwitchDescription;
        const char        *SwitchResult;
    } CommandLineSwitch;
    
    /*!
     @typedef           CommandLineOptions
     @abstract                              "Type to contain a variable amount of CLSwitches".
     @remark                                "The switches are zero indexed, and @NumSwitches is zero indexed, so count from 0".
     @constant          NumSwitches         "The number of switches".
     @constant          MinSwitches         "The minimum number of switches this program requires to run".
     @constant          ProgramName         "The name you want output when the help is printed".
     @constant          ProgramDescription  "The description of the program when the help is printed".
     @constant          Author              "The author of the program".
     @constant          Copyright           "The starting and ending copyright years".
     @constant          License             "The license this program is released under".
     @constant          Switch              "A pointer to an array of CLSwitch instances containing the properties of the switches".
     */
    typedef struct CommandLineOptions {
        size_t              NumSwitches;
        uint64_t            MinSwitches;
        const char         *Name;
        const char         *Description;
        const char         *Author;
        const char         *Copyright;
        const char         *License;
        CommandLineSwitch **Switch;
    } CommandLineOptions;
    
    BitInput *InitBitInput(void) {
        BitInput *BitI = calloc(1, sizeof(BitInput));
        return BitI;
    }
    
    BitOutput *InitBitOutput(void) {
        BitOutput *BitO = calloc(1, sizeof(BitOutput));
        return BitO;
    }
    
    BitBuffer *InitBitBuffer(const size_t BufferSize) {
        BitBuffer *BitB       = calloc(1, sizeof(BitBuffer));
        BitB->BitsAvailable   = Bytes2Bits(BufferSize);
        BitB->BitsUnavailable = 0;
        return BitB;
    }
    
    CommandLineOptions *InitCommandLineOptions(void) {
        CommandLineOptions *CMD = calloc(1, sizeof(CommandLineOptions));
        return CMD;
    }
    
    void InitCommandLineSwitches(CommandLineOptions *CMD, uint64_t NumSwitches) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "InitCommandLineSwitches", "Pointer to CommandLineOptions is NULL\n");
        } else {
            CMD->NumSwitches          += NumSwitches;
            CMD->Switch                = calloc(NumSwitches, sizeof(CommandLineSwitch));
            for (uint64_t Option = 0; Option < NumSwitches; Option++) {
                CMD->Switch[Option]    = calloc(1, sizeof(CommandLineSwitch));
            }
        }
    }
    
    void AddCommandLineSwitch(CommandLineOptions *CMD) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "AddCommandLineSwitch", "Pointer to CommandLineOptions is NULL\n");
        } else {
            CMD->NumSwitches += 1;
            CMD->Switch[CMD->NumSwitches] = calloc(1, sizeof(CommandLineSwitch));
        }
    }
    
    uint16_t SwapEndian16(const uint16_t Data2Swap) {
        return ((Data2Swap & 0xFF00) >> 8) | ((Data2Swap & 0x00FF) << 8);
    }
    
    uint32_t SwapEndian32(const uint32_t Data2Swap) {
        return ((Data2Swap & 0xFF000000) >> 24) | ((Data2Swap & 0x00FF0000) >> 8) | ((Data2Swap & 0x0000FF00) << 8) | ((Data2Swap & 0x000000FF) << 24);
    }
    
    uint64_t SwapEndian64(const uint64_t Data2Swap) {
        return (((Data2Swap & 0xFF00000000000000) >> 56) | ((Data2Swap & 0x00FF000000000000) >> 40) | \
                ((Data2Swap & 0x0000FF0000000000) >> 24) | ((Data2Swap & 0x000000FF00000000) >>  8) | \
                ((Data2Swap & 0x00000000FF000000) <<  8) | ((Data2Swap & 0x0000000000FF0000) << 24) | \
                ((Data2Swap & 0x000000000000FF00) << 40) | ((Data2Swap & 0x00000000000000FF) << 56));
    }
    
    int64_t Bits2Bytes(const int64_t Bits, const bool RoundUp) {
        if (RoundUp == true) {
            return (Bits / 8) + (8 - (Bits % 8));
        } else {
            return (Bits / 8);
        }
    }
    
    int64_t Bytes2Bits(const int64_t Bytes) {
        return (Bytes * 8);
    }
    
    uint8_t BitsRemainingInByte(const uint64_t BitsAvailable) {
        return BitsAvailable > 8 ? 8 : BitsAvailable;
    }
    
    uint64_t BytesRemainingInFile(BitInput *BitI) {
        uint64_t BytesLeft = 0;
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "BytesRemainingInFile", "Pointer to BitInput is NULL\n");
        } else {
            BytesLeft = BitI->FilePosition - BitI->FileSize;
        }
        return BytesLeft;
    }
    
    uint64_t GetInputFileSize(BitInput *BitI) {
        uint64_t InputSize = 0;
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetInputFileSize", "Pointer to BitInput is NULL\n");
        } else {
            InputSize = BitI->FileSize;
        }
        return InputSize;
    }
    
    uint64_t Signed2Unsigned(const int64_t Signed) {
        return ~Signed + 1;
    }
    
    int64_t Unsigned2Signed(const uint64_t Unsigned) {
        return ~Unsigned - 1;
    }
    
    int64_t Powi(int64_t Base, const int64_t Exponent) {
        return Base *= Exponent;
    }
    
    int64_t Floori(const long double Number2Floor) {
        return (int64_t)floor(Number2Floor);
    }
    
    int64_t Ceili(const long double Number2Ceil) {
        return (int64_t)ceil(Number2Ceil);
    }
    
    uint8_t CountBitsSet(const uint64_t Bits2Count) {
        uint8_t DataBit = 0, BitCount = 0;
        if (Bits2Count == 0) {
            return 0;
        } else {
            for (uint8_t Bit = 0; Bit < 64; Bit++) {
                DataBit = (Bits2Count & (1 << Bit)) >> Bit;
                if (DataBit == 1) {
                    BitCount += 1;
                }
            }
        }
        return BitCount;
    }
    
    uint64_t Power2Mask(const uint8_t Exponent) {
        uint64_t Mask = 0;
        if (Exponent > 64) {
            Log(LOG_ERR, "libBitIO", "Power2Mask", "Exponent %d is too large\n", Exponent);
        } else {
            if (Exponent == 1) {
                Mask = 1;
            } else if (Exponent == 64) {
                Mask = ((1ULL << Exponent) - 1) + (((1ULL << Exponent) - 1) - 1);
            } else {
                Mask = (1ULL << Exponent) - 1;
            }
        }
        return Mask;
    }
    
    uint64_t OnesCompliment2TwosCompliment(const int64_t OnesCompliment) {
        return ~OnesCompliment + 1;
    }
    
    uint64_t TwosCompliment2OnesCompliment(const int64_t TwosCompliment) {
        return TwosCompliment ^ 0xFFFFFFFFFFFFFFFF;
    }
    
    bool IsOdd(const int64_t Number2Check) {
        return Number2Check % 2 == 0 ? true : false;
    }
    
    uint8_t  FindHighestBitSet(const uint64_t UnsignedInt2Search) {
        uint8_t  HighestBitSet = 0;
        uint64_t Shift = 0ULL;
        
        for (uint8_t Bit = 64; Bit > 0; Bit--) {
            Shift = ((1ULL << Bit) - 1);
            if (((UnsignedInt2Search & Shift) >> Shift) == 1) {
                HighestBitSet = Bit;
                break;
            }
        }
        return HighestBitSet;
    }
    
    uint8_t DetectSystemEndian(void) {
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
    
    uint8_t GetBitInputEndian(BitInput *BitI) {
        uint8_t Endian = 0;
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetBitInputEndian", "Pointer to BitInput is NULL\n");
        } else {
            Endian = BitI->SystemEndian;;
        }
        return Endian;
    }
    
    uint8_t GetBitOutputEndian(BitOutput *BitO) {
        uint8_t Endian = 0;
        if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "GetBitOutputEndian", "Pointer to BitOutput is NULL\n");
        } else {
            Endian = BitO->SystemEndian;;
        }
        return Endian;
    }
    
    void DisplayCMDHelp(CommandLineOptions *CMD) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayCMDHelp", "Pointer to CommandLineOptions is NULL\n");
        } else {
            printf("%s by %s ©%s: %s, Released under the %s license\n\n", CMD->Name, CMD->Author, CMD->Copyright, CMD->Description, CMD->License);
            printf("Options:\n");
            for (uint8_t Option = 0; Option < CMD->NumSwitches; Option++) {
                printf("%s\t", CMD->Switch[Option]->Flag);
                printf("%s\n", CMD->Switch[Option]->SwitchDescription);
            }
        }
    }
    
    void ParseCommandLineArguments(CommandLineOptions *CMD, int argc, const char *argv[]) {
        // TODO: Scan for equals signs as well, if found, after the equal sign is the result, everything before is the switch.
        // TODO: add support for generating the short versions of the arguments.
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Pointer to CommandLineOptions is NULL\n");
        } else {
            if (CMD->NumSwitches < CMD->MinSwitches) {
                DisplayCMDHelp(CMD);
            } else {
                AddCommandLineSwitch(CMD);
                
                SetSwitchFlag(CMD, CMD->NumSwitches, "Help");
                SetSwitchDescription(CMD, CMD->NumSwitches, "Prints all the command line options\n");
                SetSwitchResultStatus(CMD, CMD->NumSwitches, true);
                
                for (uint8_t Argument = 1; Argument < argc; Argument++) { // the executable path is skipped over
                    for (uint8_t Switch = 0; Switch < CMD->NumSwitches; Switch++) {
                        char *SingleDash = calloc(strlen(CMD->Switch[Switch]->Flag + 2), 1);
                        snprintf(SingleDash, sizeof(SingleDash), "-%s\n", CMD->Switch[Switch]->Flag);
                        
                        char *DoubleDash = calloc(strlen(CMD->Switch[Switch]->Flag + 3), 1);
                        snprintf(DoubleDash, sizeof(DoubleDash), "--%s\n", CMD->Switch[Switch]->Flag);
                        
                        char *Slash      = calloc(strlen(CMD->Switch[Switch]->Flag + 2), 1);
                        snprintf(Slash, sizeof(Slash), "/%s\n", CMD->Switch[Switch]->Flag);
                        
                        if (strcasecmp(SingleDash, argv[Argument]) == 0 || strcasecmp(DoubleDash, argv[Argument]) == 0 || strcasecmp(Slash, argv[Argument]) == 0) {
                            if (Argument == CMD->NumSwitches - 1) {
                                DisplayCMDHelp(CMD);
                            } else {
                                CMD->Switch[Switch]->SwitchFound = true;
                                if (CMD->Switch[Switch]->Resultless == false) {
                                    char *SwitchResult = calloc(1, strlen(argv[Argument] - strlen(CMD->Switch[Switch]->SwitchResult)));
                                    snprintf(SwitchResult, BitIOStringSize, "%s", argv[Argument + 1]);
                                    CMD->Switch[Switch]->SwitchResult = SwitchResult;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    void OpenCMDInputFile(BitInput *BitI, CommandLineOptions *CMD, const uint8_t InputSwitch) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "OpenCMDInputFile", "Pointer to CommandLineOptions is NULL\n");
        } else if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "OpenCMDInputFile", "Pointer to BitInput is NULL\n");
        } else {
            BitI->File             = fopen(CMD->Switch[InputSwitch]->SwitchResult, "rb");
            fseek(BitI->File, 0, SEEK_END);
            BitI->FileSize = (uint64_t)ftell(BitI->File);
            fseek(BitI->File, 0, SEEK_SET);
            BitI->FilePosition          = ftell(BitI->File);
            uint64_t Bytes2Read         = BitI->FileSize > BitInputBufferSize ? BitInputBufferSize : BitI->FileSize;
            uint64_t BytesRead          = fread(BitI->BitB->Buffer, 1, Bytes2Read, BitI->File);
            BitI->BitB->BitsAvailable   = Bytes2Bits(BytesRead);
            BitI->BitB->BitsUnavailable = 0;
            DetectSystemEndian();
        }
    }
    
    void OpenCMDOutputFile(BitOutput *BitO, CommandLineOptions *CMD, const uint8_t OutputSwitch) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "OpenCMDOutputFile", "Pointer to CommandLineOptions is NULL\n");
        } else if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "OpenCMDOutputFile", "Pointer to BitOutput is NULL\n");
        } else {
            BitO->File             = fopen(CMD->Switch[OutputSwitch]->SwitchResult, "rb");
            BitO->BitB->BitsAvailable = BitOutputBufferSizeInBits;
            BitO->BitB->BitsUnavailable = 0;
            DetectSystemEndian();
        }
    }
    
    void SetCMDName(CommandLineOptions *CMD, const char *Name) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDName", "Pointer to CommandLineOptions is NULL\n");
        } else if (Name == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDName", "Pointer to Name is NULL\n");
        } else {
            CMD->Name = Name;
        }
    }
    
    void SetCMDDescription(CommandLineOptions *CMD, const char *Description) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDDescription", "Pointer to CommandLineOptions is NULL\n");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDDescription", "Pointer to Description is NULL\n");
        } else {
            CMD->Description = Description;
        }
    }
    
    void SetCMDAuthor(CommandLineOptions *CMD, const char *Author) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDAuthor", "Pointer to CommandLineOptions is NULL\n");
        } else if (Author == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDAuthor", "Pointer to Author is NULL\n");
        } else {
            CMD->Author = Author;
        }
    }
    
    void SetCMDCopyright(CommandLineOptions *CMD, const char *Copyright) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDCopyright", "Pointer to CommandLineOptions is NULL\n");
        } else if (Copyright == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDCopyright", "Pointer to Copyright is NULL\n");
        } else {
            CMD->Copyright = Copyright;
        }
    }
    
    void SetCMDLicense(CommandLineOptions *CMD, const char *License) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDLicense", "Pointer to CommandLineOptions is NULL\n");
        } else if (License == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCMDLicense", "Pointer to License is NULL\n");
        } else {
            CMD->License = License;
        }
    }
    
    void SetCMDMinSwitches(CommandLineOptions *CMD, const uint64_t MinSwitches) {
        CMD->MinSwitches = MinSwitches;
    }
    
    void SetSwitchFlag(CommandLineOptions *CMD, uint64_t SwitchNum, const char *Flag) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetSwitchFlag", "Pointer to CommandLineOptions is NULL\n");
        } else if (Flag == NULL) {
            Log(LOG_ERR, "libBitIO", "SetSwitchFlag", "Pointer to switch Flag is NULL\n");
        } else if (SwitchNum > CMD->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetSwitchFlag", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CMD->NumSwitches);
        } else {
            CMD->Switch[SwitchNum]->Flag = Flag;
        }
    }
    
    void SetSwitchDescription(CommandLineOptions *CMD, uint64_t SwitchNum, const char *Description) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetSwitchDescription", "Pointer to CommandLineOptions is NULL\n");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetSwitchDescription", "Pointer to switch Description is NULL\n");
        } else if (SwitchNum > CMD->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetSwitchDescription", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CMD->NumSwitches);
        } else {
            CMD->Switch[SwitchNum]->SwitchDescription = Description;
        }
    }
    
    void SetSwitchResultStatus(CommandLineOptions *CMD, uint64_t SwitchNum, bool IsSwitchResultless) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "SetSwitchResultStatus", "Pointer to CommandLineOptions is NULL\n");
        } else if (SwitchNum >= CMD->NumSwitches) { // - 1 so the hidden help option isn't exposed
            Log(LOG_ERR, "libBitIO", "SetSwitchResultStatus", "SwitchNum: %d, should be between 0 and %d\n", SwitchNum, CMD->NumSwitches);
        } else if (SwitchNum > CMD->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetSwitchResultStatus", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CMD->NumSwitches);
        } else {
            CMD->Switch[SwitchNum]->Resultless = (IsSwitchResultless & 1);
        }
    }
    
    const char *GetSwitchResult(CommandLineOptions *CMD, uint64_t SwitchNum) {
        const char *Result = 0;
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "GetSwitchResult", "Pointer to CommandLineOptions is NULL\n");
        } else if (SwitchNum > CMD->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetSwitchResult", "SwitchNum: %d, should be between 0 and %d\n", SwitchNum, CMD->NumSwitches);
        } else {
            Result = calloc(1, strlen(CMD->Switch[SwitchNum]->SwitchResult));
            Result = CMD->Switch[SwitchNum]->SwitchResult;
        }
        return Result;
    }
    
    bool GetSwitchPresence(CommandLineOptions *CMD, uint64_t SwitchNum) {
        bool Status = 0;
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "GetSwitchPresence", "Pointer to CommandLineOptions is NULL\n");
        } else if (SwitchNum > CMD->NumSwitches) { // - 1 so the hidden help option isn't exposed
            Log(LOG_ERR, "libBitIO", "GetSwitchPresence", "SwitchNum: %d, should be between 0 and %d\n", SwitchNum, CMD->NumSwitches);
        } else {
            Status = CMD->Switch[SwitchNum]->SwitchFound;
        }
        return Status;
    }
    
    void CloseCommandLineOptions(CommandLineOptions *CMD) {
        if (CMD == NULL) {
            Log(LOG_ERR, "libBitIO", "CloseCommandLineOptions", "Pointer to CommandLineOptions is NULL\n");
        } else {
            for (uint64_t Option = 0; Option < CMD->NumSwitches; Option++) {
                free(&CMD->Switch[Option]);
            }
            free(CMD);
        }
    }
    
    void UpdateInputBuffer(BitInput *BitI, const int64_t RelativeOffsetInBytes) {
        uint64_t Bytes2Read = 0, BytesRead = 0;
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "UpdateInputBuffer", "Pointer to BitInput is NULL\n");
        } else {
            fseek(BitI->File, RelativeOffsetInBytes, SEEK_CUR);
            BitI->FilePosition = ftell(BitI->File);
            memset(BitI->BitB->Buffer, 0, sizeof(BitI->BitB->Buffer));
            Bytes2Read = BitI->FileSize - BitI->FilePosition >= BitInputBufferSize ? BitInputBufferSize : BitI->FileSize - BitI->FilePosition;
            BytesRead  = fread(BitI->BitB->Buffer, 1, Bytes2Read, BitI->File);
            if (BytesRead != Bytes2Read) {
                Log(LOG_WARNING, "libBitIO", "UpdateInputBuffer", "Supposed to read %llu bytes, but read %llu\n", Bytes2Read, BytesRead);
            }
            uint64_t NEWBitsUnavailable = BitI->BitB->BitsUnavailable % 8; // FIXME: This assumes UpdateBuffer was called with at most 7 unused bits in the buffer...
            
            BitI->BitB->BitsUnavailable = NEWBitsUnavailable;
            BitI->BitB->BitsAvailable   = Bytes2Bits(BytesRead);
        }
    }
    
    uint64_t ReadBits(BitBuffer *BitB, const uint8_t Bits2Read, bool ReadFromMSB) {
        uint8_t Bits = Bits2Read, UserBits = 0, SystemBits = 0, Mask = 0, Data = 0, Mask2Shift = 0;
        uint64_t OutputData = 0;
        
        if ((Bits2Read <= 0) || (Bits2Read > 64)) {
            Log(LOG_ERR, "libBitIO", "ReadBits", "ReadBits: %d, only supports reading 1-64 bits\n", Bits2Read);
        } else {
            SystemBits             = 8 - (BitB->BitsUnavailable % 8);
            UserBits               = BitsRemainingInByte(Bits);
            Bits                   = SystemBits >= UserBits  ? UserBits : SystemBits;
            Mask2Shift             = ReadFromMSB ? BitB->BitsAvailable % 8 : 0;
            if (ReadFromMSB == true) {
                Mask2Shift         = SystemBits <= UserBits  ? 0 : SystemBits - UserBits;
                Mask               = (Power2Mask(Bits) << Mask2Shift);
            } else {
                Mask               = (Powi(2, Bits) - 1) << BitB->BitsUnavailable % 8;
            }
            Data                   = BitB->Buffer[BitB->BitsUnavailable / 8] & Mask;
            Data                 >>= Mask2Shift;
            OutputData           <<= SystemBits >= UserBits ? UserBits : SystemBits;
            OutputData            += Data;
            BitB->BitsAvailable   -= SystemBits >= UserBits ? UserBits : SystemBits;
            BitB->BitsUnavailable += SystemBits >= UserBits ? UserBits : SystemBits;
            Bits                  -= SystemBits >= UserBits ? UserBits : SystemBits;
        }
        return OutputData;
    }
    
    uint64_t PeekBits(BitBuffer *BitB, const uint8_t Bits2Peek, const bool ReadFromMSB) {
        uint64_t OutputData = 0ULL;
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "PeekBits", "Pointer to BitBuffer is NULL\n");
        } else {
            OutputData                 = ReadBits(BitB, Bits2Peek, ReadFromMSB);
            BitB->BitsAvailable       += Bits2Peek; // Backwards to set the counter back to where it was.
            BitB->BitsUnavailable     -= Bits2Peek;
        }
        return OutputData;
    }
    
    uint64_t  ReadRICE(BitBuffer *BitB, const bool Truncated, const bool StopBit) {
        uint64_t BitCount = 0;
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "ReadRICE", "Pointer to BitBuffer is NULL\n");
        } else {
            while (ReadBits(BitB, 1, false) != (StopBit & 1)) {
                BitCount += 1;
            }
            if (Truncated == true) {
                BitCount++;
            }
        }
        return BitCount;
    }
    
    int64_t ReadExpGolomb(BitBuffer *BitB, const bool IsSigned) {
        int64_t  Final = 0;
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "ReadExpGolomb", "Pointer to BitBuffer is NULL\n");
        } else {
            uint64_t Zeros   = 0;
            uint64_t CodeNum = 0;
            
            while (ReadBits(BitB, 1, false) != 1) {
                Zeros += 1;
            }
            
            if (IsSigned == false) {
                CodeNum  = (1ULL << Zeros);
                CodeNum += ReadBits(BitB, Zeros, false);
                Final    = CodeNum;
            } else { // Signed
                if (IsOdd(CodeNum) == true) {
                    Final = CodeNum - 1;
                } else {
                    Final = -(CodeNum - 1);
                }
            }
        }
        return Final;
    }
    
    void SkipBits(BitBuffer *BitB, const int64_t Bits2Skip) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "SkipBits", "Pointer to BitBuffer is NULL\n");
        } else {
            BitB->BitsAvailable   -= Bits2Skip;
            BitB->BitsUnavailable += Bits2Skip;
            // The file/stream updating functions need to keep this in mind.
        }
    }
    
    bool IsBitBufferAligned(BitBuffer *BitB, const uint8_t BytesOfAlignment) {
        bool AlignmentStatus = 0;
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "IsOutputStreamByteAligned", "Pointer to BitBuffer is NULL\n");
        } else if (BytesOfAlignment % 2 != 0 && BytesOfAlignment != 1) {
            Log(LOG_ERR, "libBitIO", "IsOutputStreamByteAligned", "BytesOfAlignment: %d isn't a power of 2 (or 1)\n", BytesOfAlignment);
        } else {
            if ((BitB->BitsUnavailable % Bytes2Bits(BytesOfAlignment)) == 0) {
                AlignmentStatus = true;
            } else {
                AlignmentStatus = false;
            }
        }
        return AlignmentStatus;
    }
    
    void AlignBitBuffer(BitBuffer *BitB, const uint8_t BytesOfAlignment) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "AlignInput", "Pointer to BitBuffer is NULL\n");
        } else if (BytesOfAlignment % 2 != 0 && BytesOfAlignment != 1) {
            Log(LOG_ERR, "libBitIO", "AlignInput", "BytesOfAlignment: %d isn't a power of 2 (or 1)\n", BytesOfAlignment);
        } else {
            uint8_t Bits2Align = BitB->BitsUnavailable % Bytes2Bits(BytesOfAlignment);
            if (Bits2Align != 0) {
                BitB->BitsAvailable   -= Bits2Align;
                BitB->BitsUnavailable += Bits2Align;
            }
        }
    }
    
    void AlignOutput(BitOutput *BitO, const uint8_t BytesOfAlignment) {
        if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "AlignOutput", "Pointer to BitOutput is NULL\n");
        } else if (BytesOfAlignment % 2 != 0 && BytesOfAlignment != 1) {
            Log(LOG_ERR, "libBitIO", "AlignOutput", "BytesOfAlignment: %d isn't a power of 2 (or 1)\n", BytesOfAlignment);
        } else {
            uint8_t Bits2Align = BitO->BitB->BitsUnavailable % Bytes2Bits(BytesOfAlignment);
            if (Bits2Align != 0) {
                BitO->BitB->BitsAvailable    -= Bits2Align;
                BitO->BitB->BitsUnavailable  += Bits2Align;
            }
        }
    }
    
    size_t GetBitInputBufferSize(BitInput *BitI) {
        uint64_t BufferSize = 0;
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetBitInputBufferSize", "Pointer to BitOutput is NULL\n");
        } else {
            BufferSize = sizeof(BitI->BitB->Buffer);
        }
        return BufferSize;
    }
    
    size_t GetBitOutputBufferSize(BitOutput *BitO) {
        uint64_t BufferSize = 0;
        if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "GetBitOutputBufferSize", "Pointer to BitOutput is NULL\n");
        } else {
            BufferSize = sizeof(BitO->BitB->Buffer);
        }
        return BufferSize;
    }
    
    void WriteBits(BitBuffer *BitB, const uint64_t Data2Write, uint8_t NumBits, const bool WriteFromMSB) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "WriteBits", "Pointer to BitBuffer is NULL\n");
        } else {
            // FIXME: WriteBits currently copies NumBits bits to the file, even if the input is shorter than that. we need to prepend 0 bits if that's the case
            
            uint8_t Bits2Write2BufferByte, Bits2ShiftMask, Mask;
            
            // so we're trying to write 9 bits, 0x1FF.
            // There are only 5 bits available in the current byte.
            // So, that means we have to pull out 5 bits from Data2Write, and pop it into BitO->Buffer.
            // The Mask should be 0x1F if LSBfirst, or 0xF8 if MSB first.
            // The buffer's representation is MSB first.
            // if we're supposed to write this data LSB first we need to shift it after extraction to get it ready to be applied.
            while (NumBits > 0) {
                Bits2Write2BufferByte  = 8 - (BitB->BitsAvailable % 8); // extract 5 bits from the buffer
                Bits2ShiftMask         = WriteFromMSB ? BitB->BitsAvailable % 8 : 0;
                Mask                   = Power2Mask(Bits2Write2BufferByte) << Bits2ShiftMask;
                BitB->Buffer[Bits2Bytes(BitB->BitsAvailable, false)] = Data2Write & Mask;
                NumBits               -= Bits2Write2BufferByte;
                BitB->BitsAvailable   -= Bits2Write2BufferByte;
                BitB->BitsUnavailable += Bits2Write2BufferByte;
            }
        }
    }
    
    void WriteRICE(BitBuffer *BitB, const bool Truncated, const bool StopBit, const uint64_t Data2Write, const bool WriteFromMSB) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "WriteRICE", "Pointer to BitBuffer is NULL\n");
        } else {
            for (uint64_t Bit = 0; Bit < Data2Write; Bit++) {
                WriteBits(BitB, (~StopBit), 1, WriteFromMSB);
            }
            WriteBits(BitB, StopBit, 1, WriteFromMSB);
        }
    }
    
    void WriteExpGolomb(BitBuffer *BitB, const bool IsSigned, const uint64_t Data2Write, const bool WriteFromMSB) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "WriteExpGolomb", "Pointer to BitOutput is NULL\n");
        } else {
            uint64_t NumBits = 0;
            
            NumBits = FindHighestBitSet(Data2Write);
            
            if (IsSigned == false) {
                WriteBits(BitB, 0, NumBits, WriteFromMSB);
                WriteBits(BitB, Data2Write + 1, NumBits + 1, WriteFromMSB);
            } else {
                NumBits -= 1;
                WriteBits(BitB, 0, NumBits, WriteFromMSB);
                if (IsOdd(Data2Write +1) == false) {
                    WriteBits(BitB, Data2Write + 1, NumBits + 1, WriteFromMSB);
                } else {
                    WriteBits(BitB, Data2Write + 1, NumBits + 1, WriteFromMSB);
                }
            }
        }
    }
    
    void CloseBitInput(BitInput *BitI) {
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "CloseBitInput", "Pointer to BitInput is NULL\n");
        } else {
            fclose(BitI->File);
            free(BitI);
        }
    }
    
    void FlushOutputBuffer(BitOutput *BitO, const size_t Bytes2Flush) {
        if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "FlushOutputBuffer", "Pointer to BitOutput is NULL\n");
        } else if (Bytes2Flush <= 0) {
            Log(LOG_ERR, "libBitIO", "FlushOutputBuffer", "Bytes2Flush is %d, should be >= 1\n", Bytes2Flush);
        } else {
            size_t BytesWritten = 0;
            BytesWritten = fwrite(BitO->BitB->Buffer, 1, Bytes2Flush, BitO->File);
            if (BytesWritten != Bytes2Flush) {
                Log(LOG_EMERG, "libBitIO", "FlushOutputBuffer", "Wrote %d bytes out of %d", BytesWritten, Bytes2Flush);
            }
        }
    }
    
    void CloseBitOutput(BitOutput *BitO) {
        if (BitO == NULL) {
            Log(LOG_ERR, "libBitIO", "CloseBitOutput", "Pointer to BitOutput is NULL");
        } else {
            if (IsOutputStreamByteAligned(BitO, 1) == false) {
                AlignOutput(BitO, 1);
            }
            fwrite(BitO->BitB->Buffer, Bits2Bytes(BitO->BitB->BitsUnavailable, true), 1, BitO->File);
            fflush(BitO->File);
            fclose(BitO->File);
            free(BitO);
        }
    }
    
    void CloseBitBuffer(BitBuffer *BitB) {
        if (BitB == NULL) {
            Log(LOG_ERR, "libBitIO", "CloseBitBuffer", "Pointer to BitBuffer is NULL");
        } else {
            free(BitB->Buffer);
            free(BitB);
        }
    }
    
    uint64_t GenerateCRC(const uint8_t *Data2CRC, const size_t DataSize, const uint64_t ReciprocalPoly, const uint8_t PolySize, const uint64_t PolyInit) {
        if (PolySize % 8 != 0) {
            // You have to do it bitwise
        } else if (DataSize % PolySize) {
            // do it word wise aka grab PolySize bits from Data2CRC at once
        } else {
            // Do it bytewise
        }
        /*
        uint16_t CRCResult = 0;
        for (uint64_t Byte = 0; Byte < DataSize; Byte++) {
            CRCResult = ReciprocalPoly ^ Data2CRC[Byte] << 8;
            for (uint8_t Bit = 0; Bit < 8; Bit++) {
                if ((CRCResult & 0x8000) == true) {
                } else {
                    CRCResult <<= 1;
                }
            }
        }
         */
        return 0;
    }
    
    bool VerifyCRC(const uint8_t *Data2CRC, const size_t Data2CRCSize, const uint64_t RecipricalPoly, const uint8_t PolySize, const uint64_t PolyInit, const uint64_t PrecomputedCRC) {
        return false;
    }
    
    uint32_t GenerateAdler32(const uint8_t *Data, const size_t DataSize) {
        uint32_t Adler  = 1;
        uint32_t Sum1   = Adler & 0xFFFF;
        uint32_t Sum2   = (Adler >> 16) & 0xFFFF;
        
        for (uint64_t Byte = 0; Byte < DataSize; Byte++) {
            Sum1 += Data[Byte] % 65521;
            Sum2 += Sum1 % 65521;
        }
        return (Sum2 << 16) + Sum1;
    }
    
    bool VerifyAdler32(const uint8_t *Data, const size_t DataSize, const uint32_t EmbeddedAdler32) {
        uint32_t GeneratedAdler32 = GenerateAdler32(Data, DataSize);
        if (GeneratedAdler32 != EmbeddedAdler32) {
            return false;
        } else {
            return true;
        }
    }
    
    void Log(const uint8_t ErrorLevel, const char *LibraryOrProgram, const char *Function, const char *ErrorDescription, ...) {
        char ErrorString[BitIOStringSize];
        char VariadicArguments[BitIOStringSize];
        
        va_list ExtraArguments;
        va_start(ExtraArguments, ErrorDescription);
        //va_arg(ExtraArguments, const char);
        //vsnprintf(VariadicArguments, BitIOStringSize, "%s", ExtraArguments);
        char *VarArgString = va_arg(ExtraArguments, char);
        vsnprintf(VariadicArguments, BitIOStringSize, "%s", VarArgString);
        va_end(ExtraArguments);
        
        snprintf(ErrorString, BitIOStringSize, "%s - %s: %s - %s\n", LibraryOrProgram, Function, ErrorDescription, VariadicArguments);
#ifdef _WIN32
        uintptr_t *EventLog = RegisterEventSource(NULL, LibraryOrProgram);
        uint32_t ErrorCode = ReportEvent(EventLog, ErrorLevel, 1, 0xF000FFFF, NULL, 1, strlen(ErrorString), ErrorString, NULL);
        if (ErrorCode != 0) {
            fprintf(stderr, "BitIO - Log: Windows version of Logger failed\n");
            fprintf(stderr, ErrorString);
        }
        bool DeregisterSucceeded = DeregisterEventSource(EventLog);
        if (DeregisterSucceeded > 0) {
            fprintf(stderr, "BitIO - Log: Deregistering EventLog failed\n");
        }
#else
        if ((ErrorLevel == LOG_EMERG) || (ErrorLevel == LOG_CRIT)) {
            openlog(LibraryOrProgram, ErrorLevel, (LOG_PERROR|LOG_MAIL|LOG_USER));
        } else {
            openlog(LibraryOrProgram, ErrorLevel, (LOG_PERROR|LOG_USER));
        }
        syslog(LOG_ERR, "%s\n", ErrorString);
#endif
    }
    
    void ReadUUID(BitBuffer *BitB, uint8_t *UUIDString) {
        if (sizeof(UUIDString) != BitIOUUIDSize) {
            Log(LOG_ERR, "libBitIO", "ReadUUID", "UUIDString is %d bytes long, should be 21\n", sizeof(UUIDString));
        } else {
            for (uint8_t Character = 0; Character < BitIOUUIDSize - 1; Character++) {
                if (Character == 21) {
                    UUIDString[Character] = 0x00;
                } else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
                    UUIDString[Character] = 0x2D;
                } else {
                    UUIDString[Character] = ReadBits(BitB, 8, true);
                }
            }
        }
    }
    
    void SwapUUID(const uint8_t *UUIDString2Convert, uint8_t *ConvertedUUIDString) {
        uint32_t Section1  = 0, SwappedSection1 = 0;
        uint16_t Section2  = 0, SwappedSection2 = 0;
        uint16_t Section3  = 0, SwappedSection3 = 0;
        uint16_t Section4  = 0, SwappedSection4 = 0;
        uint64_t Section5  = 0, SwappedSection5 = 0;
        
        if (sizeof(UUIDString2Convert) != BitIOUUIDSize || sizeof(ConvertedUUIDString) != BitIOUUIDSize) {
            Log(LOG_ERR, "libBitIO", "SwapUUID", "UUIDString2Convert is %d bytes long, ConvertedUUIDString is %d bytes long, should be 21\n", sizeof(UUIDString2Convert), sizeof(ConvertedUUIDString));
        } else {
            for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
                if ((UUIDByte == 0) || (UUIDByte == 1) || (UUIDByte == 2) || (UUIDByte == 3)) {
                    Section1 <<= 8;
                    Section1  += UUIDString2Convert[UUIDByte];
                } else if ((UUIDByte == 5) || (UUIDByte == 6)) {
                    Section2 <<= 8;
                    Section2  += UUIDString2Convert[UUIDByte];
                } else if ((UUIDByte == 8) || (UUIDByte == 9)) {
                    Section3 <<= 8;
                    Section3  += UUIDString2Convert[UUIDByte];
                } else if ((UUIDByte == 11) || (UUIDByte == 12)) {
                    Section4 <<= 8;
                    Section4  += UUIDString2Convert[UUIDByte];
                } else if ((UUIDByte == 14) || (UUIDByte == 15) || (UUIDByte == 16) || (UUIDByte == 17) || (UUIDByte == 18) || (UUIDByte == 19)) {
                    Section5 <<= 8;
                    Section5 += UUIDString2Convert[UUIDByte];
                }
            }
            SwappedSection1 = SwapEndian32(Section1);
            SwappedSection2 = SwapEndian16(Section2);
            SwappedSection3 = SwapEndian16(Section3);
            SwappedSection4 = SwapEndian16(Section4);
            SwappedSection5 = SwapEndian64(Section5);
            for (uint8_t UUIDByte = BitIOUUIDSize - 1; UUIDByte > 0; UUIDByte--) {
                if ((UUIDByte == 0) || (UUIDByte == 1) || (UUIDByte == 2) || (UUIDByte == 3)) {
                    ConvertedUUIDString[UUIDByte] = SwappedSection1 & Power2Mask(8);
                } else if ((UUIDByte == 5) || (UUIDByte == 6)) {
                    ConvertedUUIDString[UUIDByte] = SwappedSection2 & Power2Mask(8);
                } else if ((UUIDByte == 8) || (UUIDByte == 9)) {
                    ConvertedUUIDString[UUIDByte] = SwappedSection3 & Power2Mask(8);
                } else if ((UUIDByte == 11) || (UUIDByte == 12)) {
                    ConvertedUUIDString[UUIDByte] = SwappedSection4 & Power2Mask(8);
                } else if ((UUIDByte == 14) || (UUIDByte == 15) || (UUIDByte == 16) || (UUIDByte == 17) || (UUIDByte == 18) || (UUIDByte == 19)) {
                    ConvertedUUIDString[UUIDByte] = SwappedSection5 & Power2Mask(8);
                } else if (UUIDByte == 20) {
                    ConvertedUUIDString[UUIDByte] = 0x00;
                } else {
                    ConvertedUUIDString[UUIDByte] = 0x2D;
                }
            }
        }
    }
    
    uint8_t WriteUUID(BitBuffer *BitB, const uint8_t *UUIDString) {
        if (sizeof(UUIDString) != BitIOUUIDSize) {
            Log(LOG_ERR, "libBitIO", "WriteUUID", "UUIDString is %d bytes long, should be 21\n", sizeof(UUIDString));
        } else {
            for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
                if ((UUIDByte != 4) && (UUIDByte != 7) && (UUIDByte != 10) && (UUIDByte != 13) && (UUIDByte != 20)) { // Bullshit bytes
                    WriteBits(BitB, UUIDString[UUIDByte], 8, true);
                }
            }
        }
        return EXIT_SUCCESS;
    }
    
    bool CompareUUIDs(const uint8_t *UUIDString1, const uint8_t *UUIDString2) {
        bool UUIDsMatch = 0;
        
        if (sizeof(UUIDString1) != BitIOUUIDSize || sizeof(UUIDString2) != BitIOUUIDSize) {
            Log(LOG_ERR, "libBitIO", "CompareUUIDs", "UUIDString1 is %d bytes long, UUIDString2 is %d bytes long, should be 21\n", sizeof(UUIDString1), sizeof(UUIDString2));
        } else {
            for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
                if (UUIDString1[UUIDByte] != UUIDString2[UUIDByte]) {
                    UUIDsMatch = false;
                } else if ((UUIDString1[UUIDByte] == UUIDString2[UUIDByte]) && (UUIDByte = BitIOUUIDSize - 1)) {
                    UUIDsMatch = true;
                }
            }
        }
        return UUIDsMatch;
    }
    
    void OpenSocket() {
        
    }
    
    void CreateSocket(const int Domain, const int Type, const int Protocol) {
        int Socket;
#ifndef _WIN32
        Socket = socket(Domain, Type, Protocol);
#elif _WIN32
        
#endif
    }
    
    // So, I need to accept packets of variable size, from the network, disk, whatever.
    
    // Should BitInput and BitOutput store a value saying what type of "file" it's being written to? or just write to the file handle regardless?
    // So, the gist is we need a function to read from a file or network stream, and create a buffer from that data, and pop it into the struct.
    // Ok, so I need a function to read a file/socket into a buffer, and one to write a buffer to a file/socket.
    // I don't know if FILE pointers work with sockets, but i'm going to ignore that for now.
    
    void ReadFile2Buffer(BitInput *BitI, size_t Bytes2Read) {
        // Should this just take in BitInput?
        if (BitI == NULL) {
            Log(LOG_ERR, "libBitIO", "ReadFile2Buffer", "Pointer to BitInput is NULL\n");
        } else {
            uint8_t *Buffer             = calloc(1, Bytes2Read);
            fread(Buffer, 1, Bytes2Read, BitI->File);
            BitI->BitB->Buffer          = Buffer;
            BitI->FilePosition          = ftell(BitI->File);
            BitI->BitB->BitsAvailable   = Bytes2Bits(Bytes2Read);
            BitI->BitB->BitsUnavailable = 0;
        }
    }
    
    void ReadSocket2Buffer(BitInput *BitI, size_t Bytes2Read) {
        
    }
    
    void WriteBuffer2File(BitOutput *BitO) {
        
    }
    
    void WriteBuffer2Socket(BitOutput *BitO) {
        
    }
    
#ifdef __cplusplus
}
#endif
