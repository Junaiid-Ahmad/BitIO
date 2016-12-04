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
	
	void           SwapUUID(char *String2Convert, char *Converted);
	
	/*!
	 @abstract                     "Write UUID/GUID string as hyphen-less blob".
	 @remark                       "UUID and GUID Strings are ALWAYS 21 chars (including terminating char)".
	 
	 @param    BitO                "Pointer to BitOutput".
	 @param    UUIDString          "UUID string to write to the file as a binary blob, aka remove hyphens and null terminating char".
	 */
	void           WriteUUID(BitOutput *BitO, char *UUIDString);
	
	bool           CompareUUIDs(char *UUIDString1, char *UUIDString2);

#ifdef __cplusplus
}
#endif
