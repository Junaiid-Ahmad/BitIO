BitIO:
======
BitIO (Pronounced Bit Eye Oh) is a library for Reading, and Writing bits (and all the tasks associated with doing so, like Swapping endian, to Decoding DEFLATE encoded data).  

BitIO uses HeaderDoc style formatting, which should be compatible with Doxygen.  

BitIO is released under a BSD style license (I haven't had the time to decide on which one just yet).  

BitIO is 64 bit native, but if a function is limited to a fewer number of bits (For example, Adler32 is limited to 32 bits, there's no reason to use more, so I don't.)  

I've tried optimizing the memory and CPU usage of each function to be as low as possible, in fact I'm incredibly proud that my various functions have been SO quick, that they don't even show up on Apple's Instruments debugger (WITH High frequency recording enabled!)  

Also, I've tried keeping BitIO modern, but using the preprocessor as little as possible. Currently, it's exclusively used for including files, for the pragma once directive, and to ensure C++ compilers don't mistake it as C++.  

Currently, there's no stable API, let alone ABI. I tend to tweak functions to use as few arguments as possible, and I tend to rename them and variables to be more clear and concise.   

Before You Compile:
-------------------
Make sure to set the size of BitInputBufferSize and BitOutputBufferSize to however large you want, I default to using 4096 bytes.  



Compiling:
------------------
The Makefile's default installation location is: /usr/local/Packages/BitIO.   
You can change that by calling make install with: sudo make install DESTINATION="/your/path/here"  

Or, you can just change the DESTINATION variable in the makefile.  

The makefile by defaults builds the executable and library as static libraries, in release form, for the computer it's building on with -march=native.  

If you're cross compiling, you'll have to change the makefile (I may get around to making this a disable-able option in the makefile, but it's not very high on my todo list.')  

On my Mac, the BitIO library is about 20kb, if that's too big for your use, enable link time optimization on your program to trim unused functions.'  

Currently the Input and Output command parsers are a little finicky, and I haven't tested the globbing as much as I should.  

How To Use BitIO:
------------------

In order to use BitIO, you need to include BitIO.h (obviously).  

* In your main function, you need to declare an instance/instances of pointer to BitInput, a pointer to BitOutput, and a single pointer to ErrorStatus (basically contains error messages returned from various functions, and may be expanded or renamed in the future to support message passing with multi-threading).  

* You can have as many instances of either BitInput or BitOutput as you want, they all use seperate copies of the structs involved.  

* After you calloc (Do NOT use malloc!) BitInput or BitOutput, you need to call InitBitInput or InitBitOutput to set it up.

* At the end of your main function, call CloseBitInput and CloseBitOutput to dealloc your instances of BitInput and BitOutput.  

Warnings:
------------------
Do NOT touch BitIOCurrentArgument at all, that's used by the command line parsers, and it's a shitty hack to keep them from re-reading the same inputs and outputs instead of moving on to later ones like they should.  

Licensing:
------------------
BitIO is released under a BSD-like license (I haven't decided on which one just yet).  

Here's a tl;dr version of what my license is:  
* Don't say that you wrote BitIO.  
* Don't sue me if something goes wrong.  
* Include in your acknowledgments a link to [BitIO on Github](github.com/BumbleBritches57/BitIO)  

* Don't copy BitIO and reupload it, with all references to me scrubbed. that's shitty, and I won't hesitate to issue a takedown. Looking at you, China.  

* Oh, and the "original and modified source code is made available" part doesn't mean you have to host it, just send in pull requests, nomatter what I do with your contributions, you're good to go, and you don't have to host it or anything like that. Just include the version of BitIO you used'  
