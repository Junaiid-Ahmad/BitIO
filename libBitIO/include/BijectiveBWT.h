#include "../include/BitIO.h"

#pragma once

#ifndef LIBBITIO_BIJECTIVEBWT_H
#define LIBBITIO_BIJECTIVEBWT_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void EncodeBijectiveBWT(uint16_t ****Array, size_t ArraySize);
    
    void RotateArray(const size_t DataSize, int64_t *Data, const uint64_t NumRotations, const bool RotateRight);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBBITIO_BIJECTIVEBWT_H */
