#include "../include/UUID.h"

#ifdef __cplusplus
extern "C" {
#endif

    void ReadUUID(BitInput *BitI, char *UUIDString) {
        for (uint8_t Character = 0; Character < BitIOUUIDSize - 1; Character++) {
            if (Character == 21) {
                UUIDString[Character] = 0x00;
            } else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
                UUIDString[Character] = 0x2D;
            } else {
                UUIDString[Character] = ReadBits(BitI, 8, true);
            }
        }
    }

	void SwapUUID(const char *UUIDString2Convert, char *ConvertedUUIDString) {
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

	uint8_t WriteUUID(BitOutput *BitO, const char *UUIDString) {
		if (strlen(UUIDString) != BitIOUUIDSize - 1) {
			return EXIT_SUCCESS;
		} else {
			for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize - 1; UUIDByte++) {
				if ((UUIDByte != 4) && (UUIDByte != 7) && (UUIDByte != 10) && (UUIDByte != 13) && (UUIDByte != 20)) {
					// Character 21 is the NULL terminator, the rest are the hyphens.
					WriteBits(BitO, UUIDString[UUIDByte], 8);
				}
			}
		}
		return EXIT_SUCCESS;
	}

    bool CompareUUIDs(const char *UUIDString1, const char *UUIDString2) {
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
