#include "BitIO.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct UTF8String {
		uint8_t    Endian:2;
		uint64_t   GraphemeCount;
		Grapheme  *Graphemes[];
	} UTF8String;
	
	// If the first 2 bits in a code point = 10, it's a "data" code point.
	// if the first 2 bits = 11, it's the first codepoint in a character, and as a result of that it will tell how many more codepoints in the character there are
	
	extern enum UTF8Constants { // UTF-8 strings are Big Endian by default, therefore 0xFEFF is correct. 0xFFFE needs to be swapped
		UTF8String_MaxStringSize            = 65535,
		UTF8String_MinStringSize            = 255,
		UTF8String_MaxCodeUnitsInCodePoint  = 4,
		UTF8String_MaxCodeUnits             = UTF8String_MaxStringSize * UTF8String_MaxCodeUnitsInCodePoint,
		UTF8String_MinCodeUnits             = UTF8String_MinStringSize * UTF8String_MaxCodeUnitsInCodePoint,
		UTF8String_MaxValue                 = 0x10FFFF, // highest valid Unicode codepoint.
		UTF8String_BOM1_BE                  = 0xFEFF,
		UTF8String_BOM1_LE                  = 0xFFFE,
		UTF8String_BOM2_BE                  = 0xEFBBBF,
		UTF8String_BOM2_LE                  = 0xBFBBEF,
		UTF8String_LineFeed                 = 0x0A,
		UTF8String_VerticalTab              = 0x0B,
		UTF8String_FormFeed                 = 0x0C,
		UTF8String_CarriageReturn           = 0x0D,
		UTF8String_NextLine                 = 0x85,
		UTF8String_LineSeperator            = 0x2028,
		UTF8String_ParagraphSeperator       = 0x2029,
	} UTF8Constants;
	
	extern enum BOMType {
		NoBOM   = 0,
		BOM1_BE = 1,
		BOM1_LE = 2,
		BOM2_BE = 3,
		BOM2_LE = 4,
	} BOMType;
	
	extern enum CodePointType {
		FourCodeUnitsInCodePoint,
		ThreeCodeUnitsInCodePoint,
		TwoCodeUnitsInCodePoint,
		OneCodeUnitInCodePoint,
	} CodePointType;
	
	extern enum UTF8StringErrorCodes {
		NotEnoughRoomToConcatenate      = 1,
		CharacterIsContinuationCodeUnit = 2,
	} UTF8StringErrorCodes;
	
	extern enum DiacriticalMarks { // Starts at 0x300, ends at 0x36F
		GraveAccent                   = 0x300,
		AcuteAccent                   = 0x301,
		CircumflexAccent              = 0x302,
		TildeAccent                   = 0x303,
		MacronAccent                  = 0x304,
		OverlineAccent                = 0x305,
		BreveAccent                   = 0x306,
		DotAccent                     = 0x307,
		DieresisAccent                = 0x308,
		TopHookAccent                 = 0x309,
		RingAccent                    = 0x30A,
		DoubleAcuteAccent             = 0x30B,
		CaronAccent                   = 0x30C,
		VerticalLineAboveAccent       = 0x30D,
		DoubleVerticalLineAboveAccent = 0x30E,
		DoubleGraveAccent             = 0x30F,
		CandrabinduAccent             = 0x310,
		InvertedBreveAccent           = 0x311,
		TurnedCommaAboveAccent        = 0x312,
		CommaAboveAccent              = 0x313,
		ReversedCommaAboveAccent      = 0x314,
		CommaAboveRightAccent         = 0x315,
		GraveBelowAccent              = 0x316,
		AcuteBelowAccent              = 0x317,
		LeftTackBelowAccent           = 0x318,
		RightTackBelowAccent          = 0x319,
		LeftAngleAboveAccent          = 0x31A,
		HornAccent                    = 0x31B,
		LeftHalfRingBelow             = 0x31C,
		UpTackBelow                   = 0x31D,
		DownTackBelow                 = 0x31E,
		PlusSignBelow                 = 0x31F,
		MinusSignBelow                = 0x320,
		PalatalizedHookBelow          = 0x321,
		RetroflexHookBelow            = 0x322,
		DotBelow                      = 0x323,
		DieresisBelow                 = 0x324,
		RingBelow                     = 0x325,
		CommaBelow                    = 0x326,
		Cedilla                       = 0x327,
		Ogonek                        = 0x328,
		VerticalLineBelow             = 0x329,
		BridgeBelow                   = 0x32A,
		InvertedDoubleArchBelow       = 0x32B,
		CaronBelow                    = 0x32C,
		CircumflexAccentBelow         = 0x32D,
		BreveBelow                    = 0x32E,
		InvertedBreveBelow            = 0x32F,
		TildeBelow                    = 0x330,
		MacronBelow                   = 0x331,
	} DiacriticalMarks;
	
	extern enum PrecomposedCodePoints {
		CapitalAcuteA = 0xC1,
		LowerAcuteA   = 0xE1,
	} PrecomposedCodePoints;
	
	typedef struct UTF8StringErrors {
		int64_t ConcatenateStrings;
		int64_t CompareStrings;
		int64_t RemoveCharacter;
	} UTF8StringErrors;
	
	/*!
	 @abstract                        "Structure to hold various variables to contain a string".
	 @constant StringSizeInCodeUnits  "Total number of bytes in the string".
	 @constant StringSizeInCodePoints "Size of string in the Unicode equilivent of characters"
	 @constant CharacterSize          "Array to hold the number of Code Points in each UTF8 character, 2 bits each, 0 = 1, 1 = 2, etc.".
	 @constant
	 */
	/*
	 typedef struct UTF8String {
		UTF8StringErrors *Error;
		bool              StringEndian:1;
		size_t            StringSizeInCodeUnits; // aka bytes
		size_t            StringSizeInCodePoints; // aka characters
		size_t            StringSizeInGraphemes;
		uint8_t           CodePoints[UTF8String_MaxCodeUnits];
		// Because of combining characters and accents and whatnot, we' can't have a 4 bitfield, we'll have to scan each byte
	 } UTF8String;
	 */
	
	/*!
	 @abstract                        "The SubString can be a character, or it's own string, all that matters is that each code point is adjacent".
	 */
	
	extern enum UTF8RopeConstants {
		MaxGraphemes = 4096,
		MaxCodeUnits = 16,
	} UTF8RopeConstants;
	
	extern enum UTF8InvalidCodePoints {
		LowestValidCodePoint = 0x1FFFF,
	} UTF8InvalidCodePoints;
	
	typedef struct Grapheme { // Singular Grapheme
		uint8_t    GraphemeSize;
		uint8_t    Data[];
	} Grapheme;
	
	typedef struct UTF8String {
		uint8_t    Endian:2;
		uint64_t   GraphemeCount;
		Grapheme  *Graphemes[];
	} UTF8String;
	
	void       RemoveSubString(UTF8String *OldString, UTF8String *NewString, UTF8String String2Remove, bool RemoveAll);
	
	bool       CodeUnitIsStartCodePoint(UTF8String *String, uint64_t StartCodeUnit);
	
	void       DuplicateString(UTF8String *String, UTF8String *NewString);
	
	bool       CompareStrings(UTF8String *String1, UTF8String *String2, bool CaseSensitive);
	
	UTF8String ConcatenateStrings(UTF8String *String1, UTF8String *String2, UTF8String *NewString);
	
	uint64_t   LocateOccurrence(UTF8String *String, UTF8String *Character2Locate, bool StartAtEnd);
	
	void       CheckBOM(UTF8String *String);
	
	//void       MeasureString(UTF8String *String);
	
	void       CreateString(UTF8String *String2Create, char CodeUnits[]);
	
	UTF8String ReadUTF8String(BitInput *BitI, UTF8String *String, uint64_t Graphemes2Read);
	
	
#ifdef __cplusplus
}
#endif
