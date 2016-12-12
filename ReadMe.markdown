![BitIO](./BitIOLogo.svg):
========================
* BitIO (Pronounced Bit Eye Oh) is a library for Reading, and Writing bits (and all the tasks associated with doing so, like Swapping endian, to Decoding DEFLATE encoded data).  

* BitIO uses HeaderDoc style formatting, which should be compatible with Doxygen.  

* BitIO is 64 bit native, but if a function is limited to a fewer number of bits (For example, Adler32 is limited to 32 bits), there's no reason to use more, so I don't.

* I've tried optimizing the memory and CPU usage of each function to be as low as possible, while still doing everything as well as possible.  

* Also, I've tried keeping BitIO modern, by using the preprocessor as little as possible. Currently, it's exclusively used for including files, for the pragma once directive, and to ensure C++ compilers don't mistake it as C++.  

* Currently, there's no stable API, let alone ABI. I tweak the shit out of BitIO constantly to keep everything simple.

Before You Compile:
-------------------
* You can change the size of `BitInputBufferSize` and `BitOutputBufferSize` from 4096 bytes if you want, but keep in mind that after initialization, those constants may not reflect the true size of those arrays, because you can dynamically adjust them by calling `WriteBuffer`

* In order to assess the true size of those arrays, check out `BitI->BufferSize` or `BitO->BufferSize`



Compiling:
----------
* The Makefile's default installation location is: /usr/local/Packages/BitIO  

* You can change that by calling `make install` with: `make install DESTINATION="/your/path/here"` 

* Or, you can just change the DESTINATION variable in the makefile.  

* The makefile by defaults builds the library as a static library, in release form, for the computer it's building on with -march=native.

* to run the unit tests, simply call `make test`

* On my Mac, the BitIO library is about 20kb, if that's too big for your use, enable link time optimization on your program to trim unused functions.'

* Currently the Input and Output command parsers are a little finicky, and I haven't tested the globbing as much as I should.  

How To Use BitIO:
-----------------

In order to use BitIO, you need to include BitIO.h (obviously). 

`BitInput`/`BitOutput`:

* In `main`, you need to declare at least one instance of `BitInput`, `BitOutput`, and `ErrorStatus`.  

* You can have as many instances of `BitInput` and `BitOutput` as you want.

* After you `calloc` (**Do NOT** use `malloc`!) `BitInput`, `BitOutput`, and `ErrorStatus`, you need to call `InitBitInput`, and `InitBitOutput` to set it up. (assuming you're running on an OS, and that you're reading from files; if you are running directly on hardware, or need to access network packets, you need to call `CreateFakeFileFromAddress` first, then init like ususal).

* At the end of `main`, call `CloseBitInput` and `CloseBitOutput` to dealloc your instances of `BitInput` and `BitOutput`.  

`ReadUUID`:

* Use calloc to initalize a char array of 21 chars, and free it after `ReadUUID` returns.

* UUIDs and GUIDs use the same format, so `ReadUUID` can also read GUIDs. 
* The only difference is that GUIDs are Little Endian.
* To convert a UUID to a GUID use `ConvertUUID2GUID`.
* To convert a GUID to a UUID use `ConvertGUID2UUID`.

`Log`:

* If you need to use format specifiers in *Description*, call `snprintf`.

Warnings:
---------
**Don't** touch `BitInputCurrentArgument` or `BitOutputCurrentArgument` at all, they're used by the command line parsers.

License:
--------
BitIO is released under the 3 clause simplified BSD license..

Here's a tl;dr of my license: 

* **Don't** plagiarize BitIO.

*  **Don't** change BitIO's license. it is perpetually under a BSD-like license.

* **Don't** sue me if something goes wrong, or for using patented algorithms. I'm a programmer, not a patent clerk.

* **Do** Include in your acknowledgments a link to [BitIO on GitLab](https://www.gitlab.com/BumbleBritches57/BitIO)

Todo:
-----

* Add a function that takes memory addresses and wraps them in Fopen-able whatever. to allow BitInput/BitOutput to be used in lower level apps, directly on the hardware.
* A better way to handle reading from/writing to memory addresses is to include a bool saying the type of "file" we're reading from, and a starting and ending memory address
