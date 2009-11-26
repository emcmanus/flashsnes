FlashSNES - Quick Guide
=======================
*Port created by Ed McManus.*

*Follow me on Twitter!*

[http://www.twitter.com/emcmanus](http://www.twitter.com/emcmanus)

**More documentation in `/docs`**



## License ##

This is a Flash port of SNES9x released under the same license (a GPL, LGPL, non-commercial composite). If licensing is an issue, you probably shouldn't be porting an SNES emulator. But for those interested there is a GPL fork of SNES9x called SNES9x-gtk.


## Performance ##

This port does not currently support input or audio, and runs at half real-time (~10fps) on a Flash 10 release player. Using a current release player on Windows will give better performance.


## Important Locations ##

  - `/bin` contains some utils to check for ROM corruption and to find correct checksums
  - `/libs` contains the SWC resulting from the make-swc target
  - `/src/as3/generated_as3` contains the emitted AS3 files (the Alchemy VM and Application -- currently this requires some manual labor to generate!)
  - `/src/c/snes9x/flash` contains the Flash drivers
  - `/src/c/snes9x/flash/flash.cpp` is the C interface for the Flash application
  - `/src/c/snes9x/Markedup_SNES9x.as` details my attempt to understand Alchemy's C VM
  - `/src/c/snes9x/port.h` defines port-specific directives
  - `/docs/snes9x/porting.html` helpful doc detailing important preprocessor directives


## Supplemental Build Instructions ##

First, see `/docs/Building`

#### Before doing anything ####

Make sure you've turned on Alchemy (`alc-on`)

#### Generating emitted AS3 ####

Currently this is not automated by Ant.

Compile the C code using Makefile.flash

Makefile.flash passes everything through Alchemy's gcc, which is actually just a perl script. You'll want to modify gcc so it doesn't perform cleanup. Examine the script and it should be obvious what to comment out.

Split the generated AS file into two files, the VM (everything before "// End of file scope inline assembly"), and the application. Do this if you want to edit the emitted application assembly without killing your IDE. (The resulting file is ~30MB.)

Move these two files into `generated_as3/`.
 
#### After modifying emitted AS3 ####

Run make-swc -- Ant target in `/build`

Clean and recompile flash project.

#### Running config over SNES9x ####

Don't do it :)

See `/docs/Building`

#### After modifying C code ####

See `/docs/Building`