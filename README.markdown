FlashSNES - Quick Guide
=======================

*More docs in /docs*


#### Before doing anything ####
  - Make sure you've turned on Alchemy (alc-on)


#### Notable Locations ####
  - `/bin` contains ROM's and some ROM utils to check for curruption and to find correct checksums
  - `/libs` contains the SWC resulting from the make-swc target
  - `/src/as3/generated_as3` contains the emitted AS3 files (VM and Application -- currently this requires some manual labor to generate!)
  - `/src/c/snes9x/flash` contains the Flash drivers
  - `/src/c/snes9x/flash/flash.cpp` is the C interface for the Flash application
  - `/src/c/snes9x/Markedup_SNES9x.as` details my attempt to understand Alchemy's C VM
  - `/src/c/snes9x/port.h` defines port-specific directives
  - `/docs/snes9x/porting.html` helpful doc detailing important preprocessor directives
 

#### Running config over SNES9x ####
  - Don't do it :)
 

#### After modifying C code ####
  - See `/docs/Building`
 

#### Generating emitted AS3 ####
  - Currently this is a manual process
  - Compile C code using Makefile.flash
  - Makefile.flash passes everything through Alchemy's gcc, which is actually just a perl script
  - *Note:* this currently depends on my hacked version of gcc, which doesn't perform cleanup!
  - Split the generated AS file into two files, the VM (everything before "// End of file scope inline assembly"), and the application
  - Move these two files into generated_as3
  - Streamline this process!
 

#### After modifying emitted AS3 ####
  - Run make-swc -- Ant target in `/build`
  - Clean and recompile flash project