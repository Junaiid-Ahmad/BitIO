#include "../include/UUID.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	static const uint8_t UUIDSectionSize[21] = {
		4, 0, 0, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 6, 0, 0, 0, 0, 0, 1
	};

    void ReadUUID(BitInput *BitI, char UUIDString[BitIOUUIDSize]) {
        for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
            if (Character == 21) {
                UUIDString[Character] = 0x00;
            } else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
                UUIDString[Character] = 0x2D;
            } else {
                UUIDString[Character] = ReadBits(BitI, 8);
            }
        }
    }

	void SwapUUID(char *String2Convert, char *Converted) {
		uint8_t  Index     = 0;
		uint64_t Data2Swap = 0, SwappedData = 0;

		while (Index < BitIOUUIDSize) {
			if ((Index == 4) || (Index == 7) || (Index == 10) || (Index == 13) || (Index == 21)) {
				Converted[Index] = String2Convert[Index]; // Copy it
			} else {
				Data2Swap                = String2Convert[UUIDSectionSize[Index]]; // Depending on the index, we'll have to combine different values, so use a lookup table
				if (Index == UUIDSectionSize[Index]) { // Done reading in the data to be swapped
					SwappedData = SwapEndian64(Data2Swap);
					Converted[Index] = SwappedData;
				}
			}
		}
	}

    void WriteUUID(BitOutput *BitO, char UUIDString[BitIOUUIDSize]) {
        if (strlen(UUIDString) != BitIOUUIDSize) {
            BitO->ErrorStatus->WriteUUID = WrongStringSize;
        }
        for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
            if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13) || (Character == 21)) {
                // Character 21 is the NULL terminator, the rest are the hyphens.
                WriteBits(BitO, UUIDString[Character], 8);
            }

        }
    }

    bool CompareUUIDs(char UUIDString1[BitIOUUIDSize], char UUIDString2[BitIOUUIDSize]) {
		bool UUIDsMatch = 0;
        for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize; UUIDByte++) {
			if (UUIDString1[UUIDByte] != UUIDString2[UUIDByte]) {
				UUIDsMatch = false;
			} else if ((UUIDByte == BitIOUUIDSize) && (UUIDByte = BitIOUUIDSize - 1)) {
				UUIDsMatch = true;
			}
        }
		return UUIDsMatch;
    }
    
#ifdef __cplusplus
}
#endif
