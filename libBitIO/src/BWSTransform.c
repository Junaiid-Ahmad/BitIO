#include "../include/BWSTransform.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void EncodeBWSTransform(uint16_t ****Array, size_t ArraySize) {
        
    }
    
    void RotateArray(const size_t DataSize, int64_t *Data, const uint64_t NumRotations, const bool RotateRight) {
        // Theoretical speed up: just edit the Pointer to each array element, and increment or decrement it by  NumRotation.
        if (RotateRight == true) {
            for (uint64_t Rotation = 0; Rotation < NumRotations; Rotation++) {
                for (uint64_t Element = 0; Element < DataSize; Element++) {
                    Data[Element + 1] = Data[Element]; // TODO: Make sure the last element wraps around to the end.
                }
            }
        } else { // Rotate left
            for (uint64_t Rotation = 0; Rotation < NumRotations; Rotation++) {
                for (uint64_t Element = DataSize; Element > 0; Element--) {
                    Data[Element] = Data[Element - 1];
                }
            }
        }
    }
    
#ifdef __cplusplus
}
#endif
