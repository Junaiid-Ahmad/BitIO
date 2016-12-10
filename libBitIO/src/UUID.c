#include "../include/UUID.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	// Example UUID String: f3833d76-a0e9-4d0d-be0a-8d028937d594
	// Example Binary UUID: F3833D76A0E94D0DBE0A8D028937D594
	// F3 83 3D 76 - A0 E9 - 4D 0D  - BE 0A -  8D 02 89 37 D5 94
	// 0  1  2  3  4 5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
	
	static const uint8_t UUIDSectionSize[21] = {
		4, 0, 0, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 6, 0, 0, 0, 0, 0, 1
	};

    void ReadUUID(BitInput *BitI, char *UUIDString) {
        for (uint8_t Character = 0; Character < BitIOUUIDSize - 1; Character++) {
            if (Character == 21) {
                UUIDString[Character] = 0x00;
            } else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
                UUIDString[Character] = 0x2D;
            } else {
                UUIDString[Character] = ReadBits(BitI, 8);
            }
        }
    }

	void SwapUUID(char *UUIDString2Convert, char *ConvertedUUIDString) {
		uint32_t Section1  = 0, SwappedSection1 = 0;
		uint16_t Section2  = 0, SwappedSection2 = 0;
		uint16_t Section3  = 0, SwappedSection3 = 0;
		uint16_t Section4  = 0, SwappedSection4 = 0;
		uint64_t Section5  = 0, SwappedSection5 = 0;
		
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

	void WriteUUID(BitOutput *BitO, char *UUIDString) {
		if (strlen(UUIDString) != BitIOUUIDSize - 1) {
			BitO->ErrorStatus->WriteUUID = WrongStringSize;
		} else {
			for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
				if ((UUIDByte != 4) && (UUIDByte != 7) && (UUIDByte != 10) && (UUIDByte != 13) && (UUIDByte != 20)) {
					// Character 21 is the NULL terminator, the rest are the hyphens.
					WriteBits(BitO, UUIDString[UUIDByte], 8);
				}
			}
		}
	}

    bool CompareUUIDs(char *UUIDString1, char *UUIDString2) {
		bool UUIDsMatch = 0;
        for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
			if (UUIDString1[UUIDByte] != UUIDString2[UUIDByte]) {
				UUIDsMatch = false;
			} else if ((UUIDString1[UUIDByte] == UUIDString2[UUIDByte]) && (UUIDByte = BitIOUUIDSize - 1)) {
				UUIDsMatch = true;
			}
        }
		return UUIDsMatch;
    }
    
#ifdef __cplusplus
}
#endif
