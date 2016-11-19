#include "../include/BitIO.h"

#ifdef __cplusplus
extern "C" {
#endif

    void ReadUUID(BitInput *BitI, UUIDString *UUID) {
        for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
            if (Character == 21) {
                UUID->String[Character] = 0x00;
            } else if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13)) {
                UUID->String[Character] = 0x2D;
            } else {
                UUID->String[Character] = ReadBits(BitI, 8);
            }
        }
    }

	static const uint8_t UUIDSectionSize[21] = {
		4, 0, 0, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 6, 0, 0, 0, 0, 0, 1
	};

	void ConvertGUID2UUID(UUIDString *String2Convert, UUIDString *Converted) {
		uint8_t  Index     = 0;
		uint64_t Data2Swap = 0, SwappedData = 0;

		while (Index < BitIOUUIDSize) {
			if ((Index == 4) || (Index == 7) || (Index == 10) || (Index == 13) || (Index == 21)) {
				Converted->String[Index] = String2Convert->String[Index]; // Copy it
			} else {
				Data2Swap                = String2Convert->String[UUIDSectionSize[Index]]; // Depending on the index, we'll have to combine different values, so use a lookup table
				if (Index == UUIDSectionSize[Index]) { // Done reading in the data to be swapped
					SwappedData = SwapEndian64(Data2Swap);
					Converted->String[Index] = SwappedData;
				}
			}
		}
	}

    void WriteUUID(BitOutput *BitO, UUIDString *UUID) {
        if (strlen(UUID->String) != BitIOUUIDSize) {
            BitO->ErrorStatus->WriteUUID = WrongStringSize;
        }
        for (uint8_t Character = 0; Character < BitIOUUIDSize; Character++) {
            if ((Character == 4) || (Character == 7) || (Character == 10) || (Character == 13) || (Character == 21)) {
                // Character 21 is the NULL terminator, the rest are the hyphens.
                WriteBits(BitO, UUID->String[Character], 8);
            }

        }
    }

    bool CompareUUIDStrings(UUIDString *UUID1, UUIDString *UUID2) {
		bool UUIDsMatch = 0;
        for (uint8_t UUIDByte = 0; UUIDByte < BitIOUUIDSize; UUIDByte++) {
			if (UUID1->String[UUIDByte] != UUID2->String[UUIDByte]) {
				UUIDsMatch = false;
			} else if (UUIDByte == BitIOUUIDSize) {
				UUIDsMatch = true;
			}
        }
		return UUIDsMatch;
    }
    
#ifdef __cplusplus
}
#endif
