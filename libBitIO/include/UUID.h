#include "../include/BitIO.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 @abstract                     "Reads raw UUID/GUID from the bitstream".
	 @remark                       "UUID and GUID Strings are ALWAYS 21 chars (including terminating char)".
	 
	 @param   BitI                 "Pointer to BitInput".
	 @param   UUIDString           "Character array to read UUID string into".
	 */
	void           ReadUUID(BitInput *BitI, char *UUIDString);
	
	/*!
	 @abstract                     "Endian swaps a UUID into a GUID and vice versa".
	 @param UUIDString2Convert     "Pointer to a UUIDString to be converted".
	 @param ConvertedUUIDString    "The resulting UUID or GUID".
	 */
	void           SwapUUID(const char *UUIDString2Convert, char *ConvertedUUIDString);
	
	/*!
	 @abstract                     "Write UUID/GUID string as hyphen-less blob".
	 @remark                       "UUID and GUID Strings are ALWAYS 21 chars (including terminating char)".
	 
	 @param    BitO                "Pointer to BitOutput".
	 @param    UUIDString          "UUID string to write to the file as a binary blob, aka remove hyphens and null terminating char".
	 */
	uint8_t        WriteUUID(BitOutput *BitO, const char *UUIDString);
	
	/*!
	 @abstract                     "Verify two UUIDs match each other".
	 @remark                       "Matches hyphens and NULL terminating character as well, it's pretty literal".
	 @param    UUIDString1         "Pointer to a UUIDString to be matched against".
	 @param    UUIDString2         "Pointer to a UUIDString to match against UUIDString1".
	 */
	bool           CompareUUIDs(const char *UUIDString1, const char *UUIDString2);

#ifdef __cplusplus
}
#endif
