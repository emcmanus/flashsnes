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
 * Flash constants
 */
AS3_Val FLASH_STATIC_PROFILER;
AS3_Val FLASH_EMPTY_PARAMS;

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
	if (!Settings.Paused){
		S9xMainLoop();
		// check for events here
	}
	return AS3_Int(0);
}

AS3_Val Flash_setup (void *data, AS3_Val args) {

	AS3_Trace(AS3_String("Flash_setup"));
	
	// Flash Setup
	AS3_Val utils_ns = AS3_String("utils");
	FLASH_STATIC_PROFILER = AS3_NSGetS(utils_ns, "Profiler");
	FLASH_EMPTY_PARAMS = AS3_Array("");
	
	// Flash Releases
	AS3_Release( utils_ns );
	
	// Clear the settings struct
	ZeroMemory (&Settings, sizeof (Settings));
	
	// Recommended defaults
	Settings.ShutdownMaster = false;
	Settings.BlockInvalidVRAMAccess = true;
	Settings.HDMATimingHack = 100;
	Settings.SoundPlaybackRate = 32000;
	Settings.Stereo = true;
	Settings.SixteenBitSound = true;
	Settings.SoundEnvelopeHeightReading = true;
	Settings.DisableSampleCaching = true;
	Settings.InterpolatedSound = true;
	Settings.Transparency = true;
	Settings.SupportHiRes = false;
	Settings.SDD1Pack = true;
	
	Settings.JoystickEnabled = FALSE;
	Settings.MouseMaster = FALSE;
	Settings.SuperScopeMaster = FALSE;
	Settings.MultiPlayer5Master = FALSE;
	Settings.JustifierMaster = FALSE;
	
	Settings.APUEnabled = Settings.NextAPUEnabled = false;
	Settings.Multi = FALSE;
	Settings.StopEmulation = TRUE;
	
	// So listen, snes9x, we don't have any controllers. That's OK, yeah?
	S9xReportControllers();
	
	// Initialize system memory
	if (!Memory.Init() || !S9xInitAPU())	// We'll add sound init here later!
		OutOfMemory ();
	
	Memory.PostRomInitFunc = S9xPostRomInit;
	
	// Further sound initialization
	S9xInitSound (Settings.SoundPlaybackRate, Settings.Stereo, Settings.SoundBufferSize);
	
	uint32 saved_flags = CPU.Flags;
	
	// Load test ROM
	bool8 loaded;
	AS3_Trace(AS3_String("About to call LoadROM"));
	loaded = Memory.LoadROM( "testrom" );
	AS3_Trace(AS3_String("Returned from LoadROM"));
	
	Settings.StopEmulation = !loaded;
	
	if (!loaded) {
		AS3_Trace(AS3_String("FAILED TO LOAD ROM!!!"));
		return AS3_Int(1);
	}// else {
//		Memory.LoadSRAM( "0.srm" );			// Fake
//	}
	
	CPU.Flags = saved_flags;
	
	//S9xInitInputDevices();	// Not necessary!
	
	// Initialize GFX members
	GFX.Pitch = IMAGE_WIDTH * 2;
	GFX.Screen = (uint16 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.SubScreen = (uint16 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.ZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	GFX.SubZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	
	if (!GFX.Screen || !GFX.SubScreen)
	    OutOfMemory();
	
	// Initialize 32-bit version of GFX.Screen
	FlashDisplayBuffer = (uint32 *) malloc ((GFX.Pitch << 1) * IMAGE_HEIGHT);
	
	ZeroMemory (FlashDisplayBuffer, (GFX.Pitch << 1) * IMAGE_HEIGHT);
	ZeroMemory (GFX.Screen, GFX.Pitch * IMAGE_HEIGHT);
	ZeroMemory (GFX.SubScreen, GFX.Pitch * IMAGE_HEIGHT);
	ZeroMemory (GFX.ZBuffer, (GFX.Pitch>>1) * IMAGE_HEIGHT);
	ZeroMemory (GFX.SubZBuffer, (GFX.Pitch>>1) * IMAGE_HEIGHT);
	
	if (!S9xGraphicsInit())
		OutOfMemory();
	
	S9xSetupDefaultKeymap();
	// S9xGraphicsMode();	// Not needed?
	
	return AS3_Int(0);
}

AS3_Val Flash_teardown (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_teardown"));
	Memory.Deinit();
	return AS3_Int(0);
}

AS3_Val Flash_getDisplayPointer (void *data, AS3_Val args) {
//	AS3_Trace(AS3_String("Flash_getDisplayPointer"));
	return AS3_Ptr(FlashDisplayBuffer);
}

AS3_Val Flash_setEventManager (void *data, AS3_Val args) {
	AS3_Trace(AS3_String("Flash_setEventManager"));
	return AS3_Int(0);
}



/*
 * Required interface methods specified in Porting.html
 */
bool8 S9xOpenSnapshotFile (const char *filepath, bool8 read_only, STREAM *file) {
	AS3_Trace(AS3_String("S9xOpenSnapshotFile"));
	return false;
}

void S9xClosesnapshotFile (STREAM file) {
	AS3_Trace(AS3_String("S9xClosesnapshotFile"));
}

void S9xExit (void) {
	AS3_Trace(AS3_String("S9xExit"));
}

bool8 S9xInitUpdate (void) { // Screen is *about* to be rendered
//	AS3_Trace(AS3_String("S9xInitUpdate"));
	return true;
}

bool8 S9xDeinitUpdate (int width, int height) { // Screen has been rendered
//	AS3_Trace(AS3_String("S9xDeinitUpdate"));
	if (!Settings.StopEmulation){
		Convert16To24(IMAGE_WIDTH, IMAGE_HEIGHT);
	}
	return true;
}

void S9xMessage (int type, int number, const char *message) {
	AS3_Trace(AS3_String(">>>>>>>>>>>>>>>>>>>> EMULATOR MESSAGE >>>>>>>>>>>>>>>>>>>>"));
	AS3_Trace(AS3_String(message));
}

bool8 S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size) {
	AS3_Trace(AS3_String("S9xOpenSoundDevice"));
	return FALSE;
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
	return "";
}

const char *S9xGetDirectory (enum s9x_getdirtype dirtype) {
	AS3_Trace(AS3_String("*S9xGetDirectory"));
	return ".";
}

START_EXTERN_C
char* osd_GetPackDir() {
	AS3_Trace(AS3_String("osd_GetPackDir"));
	return "";
}
END_EXTERN_C

const char *S9xChooseFilename (bool8 read_only) {
	AS3_Trace(AS3_String("S9xChooseFilename"));
	return "";
}

const char *S9xChooseMovieFilename (bool8 read_only) {
	AS3_Trace(AS3_String("S9xChooseMovieFilename"));
	return "";
}

const char *S9xBasename (const char *path) {
	AS3_Trace(AS3_String("S9xBasename"));
	return "";
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
//	AS3_Trace(AS3_String("S9xSetPalette"));
}

void S9xSyncSpeed (void) {
//	AS3_Trace(AS3_String("\tS9xSyncSpeed"));
	/*
	if (Settings.HighSpeedSeek > 0)
    	Settings.HighSpeedSeek--;
    if (Settings.TurboMode)
    {
        if((++IPPU.FrameSkip >= Settings.TurboSkipFrames || Settings.OldTurbo) && !Settings.HighSpeedSeek)
        {
            IPPU.FrameSkip = 0;
            IPPU.SkippedFrames = 0;
            IPPU.RenderThisFrame = TRUE;
        }
        else
        {
            ++IPPU.SkippedFrames;
            IPPU.RenderThisFrame = FALSE;
        }
        return;
    }*/
	
	
	/* Check events */
	
//	static struct timeval next1 = {0, 0};
//    struct timeval now;
//
//    S9xProcessEvents(FALSE);
	
//    while (gettimeofday (&now, NULL) < 0) ;

    /* If there is no known "next" frame, initialize it now */
//    if (next1.tv_sec == 0) { next1 = now; ++next1.tv_usec; }

    /* If we're on AUTO_FRAMERATE, we'll display frames always
     * only if there's excess time.
     * Otherwise we'll display the defined amount of frames.
     */
//    unsigned limit = Settings.SkipFrames == AUTO_FRAMERATE
//                     ? (timercmp(&next1, &now, <) ? 10 : 1)
//                     : Settings.SkipFrames;
//	unsigned limit = 1;
//
//    IPPU.RenderThisFrame = TRUE; ++IPPU.SkippedFrames; // >= limit;
//    if(IPPU.RenderThisFrame)
//    {
//        IPPU.SkippedFrames = 0;
//    }
	
	
//	if ( IPPU.SkippedFrames <= 3 ){
//		IPPU.RenderThisFrame = false;
//		IPPU.SkippedFrames++;
//	} else {
		IPPU.RenderThisFrame = true;
		IPPU.FrameSkip = 0;
		IPPU.SkippedFrames = 0;
//	}
	
	/*
    else
    {
        /* If we were behind the schedule, check how much it is *
        if(timercmp(&next1, &now, <))
        {
            unsigned lag =
                (now.tv_sec - next1.tv_sec) * 1000000
               + now.tv_usec - next1.tv_usec;
            if(lag >= 500000)
            {
                /* More than a half-second behind means probably
                 * pause. The next line prevents the magic
                 * fast-forward effect.
                 *
                next1 = now;
            }
        }
    }*/

    /* Delay until we're completed this frame */

    /* Can't use setitimer because the sound code already could
     * be using it. We don't actually need it either.
     */

    /*while(timercmp(&next1, &now, >))
    {
        /* If we're ahead of time, sleep a while *
        unsigned timeleft =
            (next1.tv_sec - now.tv_sec) * 1000000
           + next1.tv_usec - now.tv_usec;
        //S9xTracef( "STDERR: <%u>", timeleft);
//        usleep(timeleft);

        CHECK_SOUND(); S9xProcessEvents(FALSE);

        while (gettimeofday (&now, NULL) < 0) ;
        /* Continue with a while-loop because usleep()
         * could be interrupted by a signal
         *
    }*/

    /* Calculate the timestamp of the next frame. */
//    next1.tv_usec += Settings.FrameTime;
//    if (next1.tv_usec >= 1000000)
//    {
//        next1.tv_sec += next1.tv_usec / 1000000;
//        next1.tv_usec %= 1000000;
//    }
}

void S9xLoadSDD1Data (void) {
	AS3_Trace(AS3_String("S9xLoadSDD1Data"));
}

void S9xProcessEvents (bool8 block) {
	;
}

void S9xSetupDefaultKeymap(){
	S9xUnmapAllControls();
}


// These methods are part of the driver interface although aren't mentioned in porting.html
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
	
	// Profiler setup
//	AS3_Val profilerParams = AS3_Array("StrType", "colorUpcast");
//	AS3_CallS( "begin", FLASH_STATIC_PROFILER, profilerParams );
	
	for (register int y = 0; y < height; y++) {	// For each row
	    register uint32 *d = (uint32 *) (FlashDisplayBuffer + y * (GFX.Pitch>>1));		// destination ptr
	    register uint16 *s = (uint16 *) (GFX.Screen + y * (GFX.Pitch>>1));				// source ptr
	    for (register int x = 0; x < width; x++) { // For each column
			uint16 pixel = *s++;
			*d++ = (((pixel >> 11) & 0x1F) << (16 + 3)) |
				   (((pixel >> 5) & 0x3F) << (8 + 2)) |
				   ((pixel & 0x1f) << (3));
	    }
	}
	
	// Profiler teardown
//	AS3_CallS( "end", FLASH_STATIC_PROFILER, profilerParams );
//	AS3_CallS( "printReport", FLASH_STATIC_PROFILER, FLASH_EMPTY_PARAMS );
//	AS3_Release( profilerParams );
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



void S9xPostRomInit(){
  S9xTracef("S9xPostRomInit");
  
  if (!strncmp((const char *)Memory.NSRTHeader+24, "NSRT", 4))
  {
    //First plug in both, they'll change later as needed
    S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
    S9xSetController(1, CTL_JOYPAD,     1, 0, 0, 0);

    switch (Memory.NSRTHeader[29])
    {
      case 0: //Everything goes
       break;

      case 0x10: //Mouse in Port 0
        S9xSetController(0, CTL_MOUSE,      0, 0, 0, 0);
        break;

      case 0x01: //Mouse in Port 1
        S9xSetController(1, CTL_MOUSE,      1, 0, 0, 0);
        break;

      case 0x03: //Super Scope in Port 1
        S9xSetController(1, CTL_SUPERSCOPE, 0, 0, 0, 0);
        break;

      case 0x06: //Multitap in Port 1
        S9xSetController(1, CTL_MP5,        1, 2, 3, 4);
        break;

      case 0x66: //Multitap in Ports 0 and 1
        S9xSetController(0, CTL_MP5,        0, 1, 2, 3);
        S9xSetController(1, CTL_MP5,        4, 5, 6, 7);
        break;

      case 0x08: //Multitap in Port 1, Mouse in new Port 1
        S9xSetController(1, CTL_MOUSE,      1, 0, 0, 0);
        //There should be a toggle here for putting in Multitap instead
        break;

      case 0x04: //Pad or Super Scope in Port 1
        S9xSetController(1, CTL_SUPERSCOPE, 0, 0, 0, 0);
        //There should be a toggle here for putting in a pad instead
        break;

      case 0x05: //Justifier - Must ask user...
        S9xSetController(1, CTL_JUSTIFIER,  1, 0, 0, 0);
        //There should be a toggle here for how many justifiers
        break;

      case 0x20: //Pad or Mouse in Port 0
        S9xSetController(0, CTL_MOUSE,      0, 0, 0, 0);
        //There should be a toggle here for putting in a pad instead
        break;

      case 0x22: //Pad or Mouse in Port 0 & 1
        S9xSetController(0, CTL_MOUSE,      0, 0, 0, 0);
        S9xSetController(1, CTL_MOUSE,      1, 0, 0, 0);
        //There should be a toggles here for putting in pads instead
        break;

      case 0x24: //Pad or Mouse in Port 0, Pad or Super Scope in Port 1
        //There should be a toggles here for what to put in, I'm leaving it at gamepad for now
        break;

      case 0x27: //Pad or Mouse in Port 0, Pad or Mouse or Super Scope in Port 1
        //There should be a toggles here for what to put in, I'm leaving it at gamepad for now
        break;

      //Not Supported yet
      case 0x99: break; //Lasabirdie
      case 0x0A: break; //Barcode Battler
    }
  }
}







