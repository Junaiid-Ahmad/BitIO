/*!
 @header    BWSTransform.h
 @author    Marcus Johnson aka BumbleBritches57
 @copyright 2017, Marcus Johnson
 @version   0.1.0
 @brief     This header contains code related to reading and writing BWS (Burrows-Wheeler-Scott) codes
 */

#include "../include/BitIO.h"

#include <stdlib.h>

#pragma once

#ifndef LIBBITIO_BWSTransform_H
#define LIBBITIO_BWSTransform_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void EncodeBWSTransform(uint16_t ****Array, size_t ArraySize);
    
    void RotateArray(const uint64_t DataSize, int64_t *Data, const uint64_t NumRotations, const bool RotateRight);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBBITIO_BWSTransform_H */
