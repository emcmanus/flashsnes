/*
 *  flash.h
 *  SNES9x_FlashSnes
 *
 *  Created by Ed McManus on 12/26/08.
 *  Copyright 2008. All rights reserved.
 *
 */


// Flash Callbacks
AS3_Val Flash_tick (void *data, AS3_Val args);
AS3_Val Flash_setup (void *data, AS3_Val args);
AS3_Val Flash_teardown (void *data, AS3_Val args);
AS3_Val Flash_getDisplayPointer (void *data, AS3_Val args);
AS3_Val Flash_setEventManager (void *data, AS3_Val args);
AS3_Val Flash_setMute(void *data, AS3_Val args);
AS3_Val Flash_paintSound( void *data, AS3_Val args );

void S9xPostRomInit();
void S9xSetupDefaultKeymap();
void S9xProcessEvents( AS3_Val keyboardEvents );
void S9xMixNewSamples( int numSamples );

void Convert16To24 (int, int);
void OutOfMemory (void);

// Test func
void putpixel(int x, int y);