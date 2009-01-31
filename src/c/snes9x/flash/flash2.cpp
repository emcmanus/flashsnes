/*
 *  flash.cpp
 *  SNES9x_FlashSnes
 *
 *  Created by Ed McManus on 12/26/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#include "AS3.h"

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "cpuexec.h"
#include "ppu.h"
#include "snapshot.h"
#include "apu.h"
#include "display.h"
#include "gfx.h"
#include "soundux.h"
#include "spc700.h"
#include "spc7110.h"
#include "controls.h"
#include "conffile.h"
#include "logger.h"

#include "flash.h"


/*
 * Screen buffer copy
 */
uint32 *FlashDisplayBuffer;

/*
 * CLib Setup
 */
int main (int argc, char **argv) {
	
	// Create callbacks
	AS3_Val setupMethod = AS3_Function(NULL, Flash_setup);
	AS3_Val tickMethod = AS3_Function(NULL, Flash_tick);
	AS3_Val quitApplicationMethod = AS3_Function(NULL, Flash_teardown);
	AS3_Val getDisplayPointerMethod = AS3_Function(NULL, Flash_getDisplayPointer);
	AS3_Val setEventManagerMethod = AS3_Function(NULL, Flash_setEventManager);
	
    AS3_Val libSDL = AS3_Object(
		"setup:AS3ValType, tick:AS3ValType, getDisplayPointer:AS3ValType, quit:AS3ValType, setEventManager:AS3ValType", 
		setupMethod, tickMethod, getDisplayPointerMethod, quitApplicationMethod, setEventManagerMethod
	);
    
	AS3_Release( setupMethod );
	AS3_Release( tickMethod );
	AS3_Release( getDisplayPointerMethod );
	AS3_Release( quitApplicationMethod );
	AS3_Release( setEventManagerMethod );
	
    AS3_LibInit(libSDL);
	
	return (0);
}


/*
 * Flash Callbacks
 */
AS3_Val Flash_tick (void *data, AS3_Val args) {
//	while (!Settings.Paused){
//		S9xTracef("Loop");
//		S9xMainLoop();
//	}
	return AS3_Int(0);
}

AS3_Val Flash_setup (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_setup"));
	
	
	
	return AS3_Int(0);
}

AS3_Val Flash_teardown (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_teardown"));
	Memory.Deinit();
	return AS3_Int(0);
}

AS3_Val Flash_getDisplayPointer (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_getDisplayPointer"));
	return AS3_Ptr(0);
}

AS3_Val Flash_setEventManager (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_setEventManager"));
	return AS3_Int(0);
}


/*
 * Required interface methods as specified in Porting.html
 */
bool8 S9xOpenSnapshotFile (const char *filepath, bool8 read_only, STREAM *file) {
	AS3_Trace(AS3_String("S9xOpenSnapshotFile"));
}

void S9xClosesnapshotFile (STREAM file) {
	AS3_Trace(AS3_String("S9xClosesnapshotFile"));
}

void S9xExit (void) {
	AS3_Trace(AS3_String("S9xExit"));
}

// Screen is *about* to be rendered
bool8 S9xInitUpdate (void) {
	AS3_Trace(AS3_String("S9xInitUpdate"));
}

// Screen has been rendered
bool8 S9xDeinitUpdate (int width, int height) {
	AS3_Trace(AS3_String("S9xDeinitUpdate"));
}

void S9xMessage (int type, int number, const char *message) {
	AS3_Trace(AS3_String(">>>>>>>>>>>>>>>>>>>> EMULATOR MESSAGE >>>>>>>>>>>>>>>>>>>>"));
	AS3_Trace(AS3_String(message));
}

bool8 S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size) {
	AS3_Trace(AS3_String("S9xOpenSoundDevice"));
}

const char *S9xGetFilename (const char *extension, enum s9x_getdirtype dirtype) {
	AS3_Trace(AS3_String("*S9xGetFilename"));
	
	static char filename [PATH_MAX + 1];
    char dir [_MAX_DIR + 1];
    char drive [_MAX_DRIVE + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
	_splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    snprintf(filename, sizeof(filename), "%s" SLASH_STR "%s%s",
             S9xGetDirectory(dirtype), fname, extension);
			 
	AS3_Trace(AS3_String(filename));
	
    return (filename);
}

const char *S9xGetFilenameInc (const char *extension, enum s9x_getdirtype dirtype) {
	AS3_Trace(AS3_String("*S9xGetFilenameInc"));
}

const char *S9xGetDirectory (enum s9x_getdirtype dirtype) {
	AS3_Trace(AS3_String("*S9xGetDirectory"));
	return ".";
}

START_EXTERN_C
char* osd_GetPackDir() {
	AS3_Trace(AS3_String("osd_GetPackDir"));
}
END_EXTERN_C

const char *S9xChooseFilename (bool8 read_only) {
	AS3_Trace(AS3_String("S9xChooseFilename"));
}

const char *S9xChooseMovieFilename (bool8 read_only) {
	AS3_Trace(AS3_String("S9xChooseMovieFilename"));
}

const char *S9xBasename (const char *path) {
	AS3_Trace(AS3_String("S9xBasename"));
}

void S9xAutoSaveSRAM (void) {
	AS3_Trace(AS3_String("S9xAutoSaveSRAM"));
}

void S9xGenerateSound (void) {
	AS3_Trace(AS3_String("S9xGenerateSound"));
}

void S9xToggleSoundChannel (int c) {
	AS3_Trace(AS3_String("S9xToggleSoundChannel"));
}

void S9xSetPalette (void) {
	AS3_Trace(AS3_String("S9xSetPalette"));
}

void S9xSyncSpeed (void) {
	AS3_Trace(AS3_String("S9xSyncSpeed"));
}

void S9xLoadSDD1Data (void) {
	AS3_Trace(AS3_String("S9xLoadSDD1Data"));
}

// These methods are part of the driver interface but aren't mentioned in porting.html!
void _makepath (char *path, const char *, const char *dir,
		const char *fname, const char *ext)
{
    if (dir && *dir)
    {
	strcpy (path, dir);
	strcat (path, "/");
    }
    else
	*path = 0;
    strcat (path, fname);
    if (ext && *ext)
    {
        strcat (path, ".");
        strcat (path, ext);
    }
}

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
  *drive = 0;

  char *slash = strrchr(path, SLASH_CHAR);
  char *dot = strrchr(path, '.');

  if (dot && slash && dot < slash) {
    dot = 0;
  }

  if (!slash) {
    *dir = 0;
    strcpy(fname, path);
    if (dot) {
      fname[dot - path] = 0;
      strcpy(ext, dot + 1);
    } else {
      *ext = 0;
    }
  } else {
    strcpy(dir, path);
    dir[slash - path] = 0;
    strcpy(fname, slash + 1);
    if (dot) {
      fname[(dot - slash) - 1] = 0;
      strcpy(ext, dot + 1);
    } else {
      *ext = 0;
    }
  }
}


/*
 * Other methods
 */
void OutOfMemory (void) {
    S9xTracef( "STDERR: Snes9X: Memory allocation failure -"
             " not enough RAM/virtual memory available.\n"
             "S9xExiting...\n");
    Memory.Deinit ();

    exit (1);
}

// Color Depth Upcast: RGB565 -> RGB888
void Convert16To24 (int width, int height){
	for (register int y = 0; y < height; y++) {	// For each row
	    register uint32 *d = (uint32 *) (FlashDisplayBuffer + y * GFX.Pitch);			// destination ptr - is this offset right?
	    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.Pitch);					// source ptr
	    for (register int x = 0; x < width; x++) { // For each column
			uint32 pixel = *s++;
//			if (pixel != 0) AS3_Trace(AS3_Int(*(int *)s));	// Print non-black pixels - see if GFX.Screen is empty
			*d++ = (((pixel >> 11) & 0x1F) << (16 + 3)) |
				   (((pixel >> 5) & 0x3F) << (8 + 2)) |
				   ((pixel & 0x1f) << (3));
	    }
	}
}

void putpixel(int x, int y) {
	int bpp = 2;
	
	uint8 *p = (uint8 *)GFX.Screen + y * GFX.Pitch + x * bpp;
	*(uint16 *)p = 0x1F;
}

// All printf statements are re-routed here

#include <stdarg.h>
void S9xTracef( const char *format, ... ){
	AS3_Trace(AS3_String(format));
}


void S9xTraceInt( int val ){
	AS3_Trace(AS3_Int((int)val));
}

