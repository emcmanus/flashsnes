## FlashSNES - Quick Compile Guide ##

*More docs in /docs*

#### Before doing anything ####

  - Make sure you've turned on Alchemy (alc-on)

#### Notable Directories ####

  - */bin* contains ROM's and some ROM utils to check for curruption and to find correct checksums
  - */libs* contains the SWC resulting from the make-swc target
  - */src/as3/generated_as3* contains the emitted files 

#### Running config over SNES9x ####

  - Don't do it :)

#### After modifying C code ####

  - Compile C code using Makefile.flash
  - Refresh and recompile flash project

#### Generating emitted AS3 ####

  - Currently this is a manual process
  - Compile C code using Makefile.flash
  - Makefile.flash passes everything through Alchemy's gcc, which is actually just a perl script
  - *NOTE:* this currently depends on my hacked version of gcc, which doesn't perform cleanup!
  - Split the generated AS file into two files, the VM (everything before "// End of file scope inline assembly"), and the application
  - Move these two files into generated_as3
  - Streamline this process!

#### After modifying emitted AS3 ####

  - Run make-swc -- Ant target in */build*
  - Refresh and recompile flash project