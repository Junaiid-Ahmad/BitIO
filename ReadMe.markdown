**BitIO:** [![Build Status](https://travis-ci.org/bumblebritches57/BitIO.svg?branch=master)](https://travis-ci.org/bumblebritches57/BitIO)
========================
* BitIO (Pronounced Bit Eye Oh) is a library for Reading, and Writing bits (and all the tasks associated with doing so, like swapping endian, processing various entropy encoded streams, etc).
* BitIO uses HeaderDoc style formatting, which is compatible with Doxygen.
* BitIO is 64 bit native, but if a function is limited to a fewer number of bits (For example, Adler32 is limited to 32 bits), there's no reason to use more, so I don't.
* I've tried optimizing the memory and CPU usage of each function to be as low as possible, while still doing everything as well as possible.
* Also, I've tried keeping BitIO modern, by using the preprocessor as little as possible. Currently, it's exclusively used for including files, for the pragma once directive, and to ensure C++ compilers don't mistake it as C++.
* Currently, there's no stable API, let alone ABI. I constantly refactor BitIO to keep everything as simple as possible.

Before You Compile:
-------------------
* You can change the size of `BitInputBufferSize` and `BitOutputBufferSize` in `BitIOConstants`, located in BitIO.h (it's currently set to 4096 bytes), but keep in mind that after initialization, those constants may not reflect the true size of those arrays.
* In order to assess the true size of those arrays, check out `GetBitInputBufferSize` or `GetBitOutputBufferSize`

Compiling:
----------
* The Makefile's default installation location is: `/usr/local/Packages/libBitIO`.
* You can change that by calling `make install` with: `PREFIX="/your/path/here" make install`, or just editing the makefile.
* The makefile by defaults builds the library as a static library, in release form, for the computer it's building on with -march=native.
* BitIO is included as a submodule in git, so you don't need to install it if you're using one of my libraries
* The makefile by defaults builds the library as a static library, in release form, for the computer it's building on with `-march=native`.
* to run the unit tests, simply call `make debug && make test && sudo make install` then `Test-BitIO <InputFile> <OutputFile>` if Test-BitIO doesn't print anything, all the tests passed.
* On my Mac, the BitIO library is about 75kb, if that's too big for your use, enable link time optimization on your program to trim unused functions.

How To Use BitIO:
-------------------------
In order to use BitIO, you need to include BitIO.h.

`BitInput`/`BitOutput`:
* In `main`, you need to call at least one instance of `InitBitInput`, `InitBitOutput`.
* You can have as many instances of `BitInput` and `BitOutput` as you want.
* After you call  `InitBitInput`, and `InitBitOutput`, you need to call `ParseCommandLineArguments` and `OpenCMDInputFile` and/or `OpenCMDOutputFile`.
* At the end of `main`, call `CloseBitInput` and/or `CloseBitOutput` to dealloc your instances of `BitInput` and `BitOutput`.

`SetCMD*`:
* After initalizing `BitInput` and `BitOutput`,  you need to call `InitCommandLineOptions`, and `InitCommandLineSwitches`, you need to set the command line options with the `SetCMD*` functions.
* Then call `ParseCommandLineArguments`.
* `ParseCommandLineArguments` automatically handles dash, double dash, and backslash arguments for you; **DO NOT INCLUDE ANY OF THOSE CHARACTERS IN YOUR FLAGS**
* Flags are case insensitive.

`ReadUUID`:
* UUIDs and GUIDs have the same size, but use a different format, so `ReadUUID` can also read GUIDs.
* The only difference is that GUIDs are little endian.
* To convert a UUID to a GUID or vice versa call `SwapUUID`.

`Log`:
* Log calls `openlog` on POSIX compatible platforms.
* Log calls `ReportEvent`on Windows.

License:
-----------
BitIO is released under the 3 clause simplified BSD license.

Here's a tl;dr of my license:
* **Don't** plagiarize BitIO.
* **Don't** relicense BitIO.
* **Don't** sue me if something goes wrong, or for using patented algorithms. I'm a programmer, not a patent clerk.
* **Do** Include in your acknowledgments a link to [BitIO on GitHub](https://www.github.com/BumbleBritches57/BitIO).

Todo:
-------
* Add a function that takes memory addresses and wraps them in Fopen-able whatever. to allow BitInput/BitOutput to be used in lower level apps, directly on the hardware.
* We can acomplish that easier by splitting out a BufferBit reader/writer, and wrapping a File bit reader/writer around it. What should we call it tho? ReadFileBits and WriteFileBits, and ReadBufferBits and WriteBufferBits? the language is kinda clumsy, but it works for now. and if i think up a better name, i can always just rename it.
* Make Power2Mask more generic and simpler.
* Finish the CRC generator and verifier.
* Finish MD5 generator and verifier.
* Write a Reed-Soloman Error correction verifier.
* Write a LDPC encoder/decoder
* Finish Deflate/Inflate
* Finish LZ77 encoder/decoder
* Write a FSE/Asymetric Numeral System encoder and decoder
* Update the Argument parser to just take in full word flags, and generate short options automatically.
