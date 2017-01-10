#include "BitIO.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
	/*!
	 @abstract               "is a Grapheme, contains the size of the grapheme in bytes, and the actual grapheme data itself".
	 @constant     Size      "Size of this grapheme in bytes".
	 @constant     Data      "An array that contains the actual bytes that make up the grapheme".
	 */
	typedef struct Grapheme {
		bool       IsLittleEndian;
		bool       IsRight2Left;
		uint8_t    Size;
		uint8_t    Data[];
	} Grapheme;
	
	/*!
	 @abstract              "Contains a string of UTF-8 "characters" aka Graphemes".
	 @constant    Graphemes "Count of Graphemes in this string".
	 @constant    Grapheme  "Array of Grapheme type to contain the individual graphemes".
	 */
	typedef struct UTF8String { // UTF8Strings are ALWAYS big endian.
		size_t     Graphemes;
		Grapheme  *Grapheme[];
	} UTF8String;
	
	extern enum UTF8Constants { // UTF-8 strings are Big Endian by default, therefore 0xFEFF is correct. 0xFFFE needs to be swapped
		UTF8String_MaxGraphemes             = 16,
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
	} UTF8Constants;
	
	extern enum GraphemeClusterBoundaries { // aka user percieved character boundaries.
											// regional indicators (Emoji flags) should be seperated by U+200B ZERO WIDTH SPACE
		UTF8String_ZeroWidthSpace           = 0x2008,
		UTF8String_CarriageReturn           = 0x000D,
		UTF8String_LineFeed                 = 0x000A,
		UTF8String_ZeroWidthNonJoiner       = 0x200C,
		UTF8String_ZeroWidthJoiner          = 0x200D,
		UTF8String_VerticalTab              = 0x0B,
		UTF8String_FormFeed                 = 0x0C,
		UTF8String_NextLine                 = 0x85,
		UTF8String_LineSeperator            = 0x2028,
		UTF8String_ParagraphSeperator       = 0x2029,
		UTF8String_ThaiSaraAm               = 0x0E33,
		UTF8String_LaoVowelSignAm           = 0x0EB3,
		
		
		UTF8String_LegacyGraphemeCluster    = 0,
		UTF8String_ExtendedGraphemeCluster  = 0,
	} GraphemeClusterBoundaries;
	
	extern enum RegionalIndicator { // Emoji Flags
		UTF8String_RegionalIndicatorA = 0x1F1E6,
		UTF8String_RegionalIndicatorB = 0x1F1E7,
		UTF8String_RegionalIndicatorC = 0x1F1E8,
		UTF8String_RegionalIndicatorD = 0x1F1E9,
		UTF8String_RegionalIndicatorE = 0x1F1EA,
		UTF8String_RegionalIndicatorF = 0x1F1EB,
		UTF8String_RegionalIndicatorG = 0x1F1EC,
		UTF8String_RegionalIndicatorH = 0x1F1ED,
		UTF8String_RegionalIndicatorI = 0x1F1EE,
		UTF8String_RegionalIndicatorJ = 0x1F1EF,
		UTF8String_RegionalIndicatorK = 0x1F1F0,
		UTF8String_RegionalIndicatorL = 0x1F1F1,
		UTF8String_RegionalIndicatorM = 0x1F1F2,
		UTF8String_RegionalIndicatorN = 0x1F1F3,
		UTF8String_RegionalIndicatorO = 0x1F1F4,
		UTF8String_RegionalIndicatorP = 0x1F1F5,
		UTF8String_RegionalIndicatorQ = 0x1F1F6,
		UTF8String_RegionalIndicatorR = 0x1F1F7,
		UTF8String_RegionalIndicatorS = 0x1F1F8,
		UTF8String_RegionalIndicatorT = 0x1F1F9,
		UTF8String_RegionalIndicatorU = 0x1F1FA,
		UTF8String_RegionalIndicatorV = 0x1F1FB,
		UTF8String_RegionalIndicatorW = 0x1F1FC,
		UTF8String_RegionalIndicatorX = 0x1F1FD,
		UTF8String_RegionalIndicatorY = 0x1F1FE,
		UTF8String_RegionalIndicatorZ = 0x1F1FF,
	} RegionalIndicator;
	
	// If the first 2 bits in a code point = 10, it's a "data" code point.
	// if the first 2 bits = 11, it's the first codepoint in a character, and as a result of that it will tell how many more codepoints in the character there are
	
	extern enum BOMType {
		NoBOM   = 0,
		BOM1_BE = 1,
		BOM1_LE = 2,
		BOM2_BE = 3,
		BOM2_LE = 4,
	} BOMType;
	
	extern enum CodePointType {
		FourCodeUnitsInCodePoint  = 4,
		ThreeCodeUnitsInCodePoint = 3,
		TwoCodeUnitsInCodePoint   = 2,
		OneCodeUnitInCodePoint    = 1,
	} CodePointType;
	
	extern enum UTF8StringErrorCodes {
		NotEnoughRoomToConcatenate      = 1,
		CharacterIsContinuationCodeUnit = 2,
	} UTF8StringErrorCodes;
	
	extern enum CombiningCharacters { // Starts at 0x300, ends at 0x36F
		/* DiacriticalMarks */
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
	} CombiningCharacters;
	
	extern enum PrecomposedCodePoints {
		CapitalAcuteA = 0xC1,
		LowerAcuteA   = 0xE1,
	} PrecomposedCodePoints;
	
	typedef struct UTF8StringErrors {
		int64_t ConcatenateStrings;
		int64_t CompareStrings;
		int64_t RemoveCharacter;
	} UTF8StringErrors;
	
	extern enum UTF8InvalidCodePoints {
		LowestValidCodePoint = 0x1FFFF,
	} UTF8InvalidCodePoints;
	
	void       RemoveSubString(UTF8String *OldString, UTF8String *NewString, UTF8String String2Remove, bool RemoveAll);
	
	bool       CodeUnitIsStartCodePoint(UTF8String *String, uint64_t StartCodeUnit);
	
	void       DuplicateString(UTF8String *String, UTF8String *NewString);
	
	bool       CompareStrings(UTF8String *String1, UTF8String *String2, bool CaseSensitive);
	
	UTF8String ConcatenateStrings(UTF8String *String1, UTF8String *String2, UTF8String *NewString, size_t StartGrapheme);
	
	uint64_t   LocateOccurrence(UTF8String *String, UTF8String *Character2Locate, bool StartAtEnd);
	
	void       CheckBOM(UTF8String *String);
	
	//void       MeasureString(UTF8String *String);
	
	void       CreateString(UTF8String *String, uint8_t *StringData, size_t DataSize);
	
	UTF8String ReadUTF8String(BitInput *BitI, UTF8String *String, uint64_t Graphemes2Read);
	
	void       SplitString(UTF8String *String2Split, UTF8String *SplitString1, UTF8String *SplitString2, Grapheme *Grapheme2Split);
	
	
#ifdef __cplusplus
}
#endif
