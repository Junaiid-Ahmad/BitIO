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
* You can change the size of `BitInputBufferSize` and `BitOutputBufferSize` in BitIOConstants, located in BitIO.h (it's currently set to 4096 bytes), but keep in mind that after initialization, those constants may not reflect the true size of those arrays.
* In order to assess the true size of those arrays, check out `BitInput->BitsAvailable` + `BitInput->BitsUnavailable` or the `BitOutput` versions.

Compiling:
----------
* The Makefile's default installation location is: `/usr/local/Packages/libBitIO`.
* You can change that by calling `make install` with: `PREFIX="/your/path/here" make install`.
* Or, you can just change the Prefix variable in the makefile.
* The makefile by defaults builds the library as a static library, in release form, for the computer it's building on with -march=native.
* to run the unit tests, simply call `make debug && make test && sudo make install` then `Test-BitIO <InputFile> <OutputFile>` if Test-BitIO doesn't print anything, all the tests passed.
* On my Mac, the BitIO library is about 75kb, if that's too big for your use, enable link time optimization on your program to trim unused functions.

How To Use BitIO:
-------------------------
In order to use BitIO, you need to include BitIO.h.

`BitInput`/`BitOutput`:
* In `main`, you need to declare at least one instance of `BitInput`, `BitOutput`, and `ErrorStatus`.
* You can have as many instances of `BitInput` and `BitOutput` as you want.
* After you `calloc`  `BitInput`, and `BitOutput`, you need to call `ParseCommandLineArguments` and `OpenCMDInputFile` and/or `OpenCMDOutputFile`.
* At the end of `main`, call `CloseBitInput` and/or `CloseBitOutput` to dealloc your instances of `BitInput` and `BitOutput`.

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
* Implement the FILE memory address wrapper.
* Replace the `ifndef _WIN32` defines with POSIX ones for the logger.
