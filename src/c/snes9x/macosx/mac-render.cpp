/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2007  Brad Jorsch (anomie@users.sourceforge.net),
                             Nach (n-a-c-h@users.sourceforge.net),
                             zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007  nitsuja


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2007  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/



/**********************************************************************************
  SNES9X for Mac OS (c) Copyright John Stiles

  Snes9x for Mac OS X

  (c) Copyright 2001 - 2007  zones
  (c) Copyright 2002 - 2005  107
  (c) Copyright 2002         PB1400c
  (c) Copyright 2004         Alexander and Sander
  (c) Copyright 2004 - 2005  Steven Seeger
  (c) Copyright 2005         Ryan Vogt
**********************************************************************************/

#include <sys/time.h>

#include "memmap.h"
#include "gfx.h"
#include "display.h"
#include "ppu.h"
#include "soundux.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLRenderers.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <AGL/agl.h>

#include "mac-prefix.h"
#include "mac-2xsai.h"
#include "mac-epx.h"
#include "mac-hq2x.h"
#include "mac-blit.h"
#include "mac-cheatfinder.h"
#include "mac-os.h"
#include "mac-quicktime.h"
#include "mac-screenshot.h"
#include "mac-render.h"

#ifdef MAC_COREIMAGE_SUPPORT
#include "mac-coreimage.h"
#endif

void DisplayChar(uint16 *, uint8);

static OSStatus BlitMPTask(void *);
static OSStatus BlitMPGLTask(void *);
static OSStatus PrepareMPBlit(void);
static OSStatus PrepareMPBlitGL(void);
static void S9xInitFullScreen(void);
static void S9xDeinitFullScreen(void);
static void S9xInitWindowMode(void);
static void S9xDeinitWindowMode(void);
static void S9xInitDirectFullScreen(void);
static void S9xDeinitDirectFullScreen(void);
static void S9xInitDirectWindowMode(void);
static void S9xDeinitDirectWindowMode(void);
static void S9xInitOpenGLFullScreen(void);
static void S9xDeinitOpenGLFullScreen(void);
static void S9xInitOpenGLWindowMode(void);
static void S9xDeinitOpenGLWindowMode(void);
static void S9xInitBlitGL(void);
static void S9xDeinitBlitGL(void);
static void S9xInitOpenGLContext(void);
static void S9xDeinitOpenGLContext(void);
static void S9xInitCoreImage(void);
static void S9xDeinitCoreImage(void);
static void S9xPutImageDirect(int, int);
static void S9xPutImageOpenGL(int, int);
static void S9xPutImageBlitGL(int, int);
static void S9xPutImageCoreImage(int, int);
static void S9xPutImageOverscanDirect(int, int);
static void S9xPutImageOverscanOpenGL(int, int);
static void S9xPutImageOverscanBlitGL(int, int);
static void S9xPutImageOverscanCoreImage(int, int);
static void S9xPutImageBlitGL2(int, int);
static void S9xPutImageBlitGL2CoreImage(int, int);
static void GLMakeScreenMesh(GLdouble *, int, int);
static void GLMakeTextureMesh(GLdouble *, int, int, double, double);

#define	BYTES_PER_PIXEL	2

#define kMarginDouble		((kMacWindowHeight - (SNES_HEIGHT          << 1)) >> 1)		/* 15 */
#define kMarginSingle		((kMacWindowHeight - (SNES_HEIGHT          << 1)) >> 2)		/*  7 */
#define kMarginDoubleExt	((kMacWindowHeight - (SNES_HEIGHT_EXTENDED << 1)) >> 1)		/*  0 */
#define kMarginSingleExt	((kMacWindowHeight - (SNES_HEIGHT_EXTENDED << 1)) >> 2)		/*  0 */

#ifdef MAC_ENABLE_DIRECT_MODE
#define BEGIN_UPDATE_WINDOW_BACKBUFFER \
	LockPortBits(gWindowPort); \
	PixMapHandle pixmap = GetPortPixMap(gWindowPort); \
	gMacRowBytes = GetPixRowBytes(pixmap); \
	gMacScreenPtr = (unsigned char *) GetPixBaseAddr(pixmap); \
	gWinScreenPtr = gMacScreenPtr + gWindowBarHeight * gMacRowBytes;
#define END_UPDATE_WINDOW_BACKBUFFER \
	UnlockPortBits(gWindowPort); \
	QDSetDirtyRegion(gWindowPort, gWindowRgn); \
	QDFlushPortBuffer(gWindowPort, nil);
#else
#define BEGIN_UPDATE_WINDOW_BACKBUFFER	;
#define END_UPDATE_WINDOW_BACKBUFFER	;
#endif

enum
{
	kMPBlitFrame = 1,
	kMPBlitDone,
	kMPBlitNone
};

enum
{
	kGL256256 = 0,
	kGL512256,
	kGL512512,
	kGLBlit2x,
	kGLBlit3x,
	kGLBlit4x,
	kGLNumTextures
};

enum
{
	kSC2xNormal = 0,
	kSC2xExtend,
	kSC2xNHiRes,
	kSC2xEHiRes,
	kSC3xNormal,
	kSC3xExtend,
	kSC3xNHiRes,
	kSC3xEHiRes,
	kSCNumTextures
};

enum
{
	kSCMeshX = 10,
	kSCMeshY = 9
};

typedef void (* Blitter) (register unsigned char *, register unsigned char *, register long, register Rect *, register Rect *);

typedef struct
{
	Blitter	blitFn;
	Rect	srcRect;
	Rect	copyRect;
	int		dstRowBytes;
	uint16	*gfxBuffer;
}	MPData;

typedef struct
{
	GLint		internal_format;
	GLint		format;
	GLint		type;
	GLenum		target;
	GLuint		textures[kGLNumTextures];
	GLdouble		vertex[kGLNumTextures][8];
	GLint		texW[kGLNumTextures];
	GLint		texH[kGLNumTextures];
	GLboolean   rangeExt;
}	OpenGLData;

static uint16				*gfxScreen[2],
							*snesScreenA,
							*snesScreenB;
static uint8				*blitGLBuffer;

static CGDirectDisplayID	gGameDisplayID;

static unsigned char		*gMacScreenPtr,
							*gWinScreenPtr;
static long					gMacRowBytes;

static Blitter				BlitFn256x240     = nil,
							BlitFn512x240     = nil,
							BlitFn512x480     = nil;

static MPTaskID				taskID            = nil;
static MPQueueID			notificationQueue = nil,
							taskQueue         = nil;
static MPSemaphoreID		readySemaphore    = nil;
static MPData				*mpDataBuffer     = nil;

static OpenGLData			OpenGL;
static CGLContextObj		glContext;
static AGLContext			agContext;
static CGLPixelFormatObj	cglpix;
static AGLPixelFormat		aglpix;
static long					glSwapInterval    = 0;
static GLint				agSwapInterval    = 0;
static CFDictionaryRef		oldDisplayMode;
static CGImageRef			cgGameImage       = nil,
							cgBlitImage       = nil;

static int					whichBuf          = 0;
static int					textureNum        = 0;
static int					prevBlitWidth, prevBlitHeight;
static int					imageWidth[2], imageHeight[2];
static int					nx                = 2;

static GLdouble				*scTexArray[kSCNumTextures];
static GLdouble				*scScnArray;

static struct timeval		bencht1, bencht2;

void InitGraphics(void)
{
	int	safebuffersize = 520 * 520 * BYTES_PER_PIXEL - 512 * 512 * BYTES_PER_PIXEL;

	GFX.Pitch    = 512 * BYTES_PER_PIXEL;

	snesScreenA  = (uint16 *) calloc( 520 *  520 * BYTES_PER_PIXEL, 1);
	snesScreenB  = (uint16 *) calloc( 520 *  520 * BYTES_PER_PIXEL, 1);
	blitGLBuffer = (uint8  *) calloc(1024 * 1024 * BYTES_PER_PIXEL, 1);

	gfxScreen[0] = snesScreenA + (safebuffersize >> 2);
	gfxScreen[1] = snesScreenB + (safebuffersize >> 2);

	GFX.Screen   = gfxScreen[0];

    if (!snesScreenA || !snesScreenB || !blitGLBuffer)
		QuitWithFatalError(0, "render 01");

	#ifdef GFX_MULTI_FORMAT
	S9xSetRenderPixelFormat(RGB555);
	#endif

	Init_2xSaI(555);
	InitLUTs();
}

void DeinitGraphics(void)
{
	if (snesScreenA)
		free(snesScreenA);
	if (snesScreenB)
		free(snesScreenB);
	if (blitGLBuffer)
		free(blitGLBuffer);
}

static OSStatus BlitMPTask(void *parameter)
{
	OSStatus	err = noErr;
    long		theCommand, param1, param2;

	#pragma unused (parameter)

	S9xTracef( "Print f: MP: Entered DirectDraw thread.\n");

	for (;;)
    {
		err = MPWaitOnQueue(taskQueue, (void **) &theCommand, (void **) &param1, (void **) &param2, kDurationForever);
        if (err)
			break;

		if (theCommand == kMPBlitFrame)
		{
			if (!fullscreen)
			{
				BEGIN_UPDATE_WINDOW_BACKBUFFER
				(mpDataBuffer->blitFn) ((uint8 *) mpDataBuffer->gfxBuffer, gWinScreenPtr, gMacRowBytes, &mpDataBuffer->srcRect, &mpDataBuffer->copyRect);
				END_UPDATE_WINDOW_BACKBUFFER
			}
			else
				(mpDataBuffer->blitFn) ((uint8 *) mpDataBuffer->gfxBuffer, gWinScreenPtr, gMacRowBytes, &mpDataBuffer->srcRect, &mpDataBuffer->copyRect);

			MPSignalSemaphore(readySemaphore);
		}
		else
		if (theCommand == kMPBlitNone)
			MPSignalSemaphore(readySemaphore);
		else
		if (theCommand == kMPBlitDone)
			break;
		else
		{
			err = userCanceledErr;
			break;
		}
	}

	MPFree(mpDataBuffer);
	MPDeleteSemaphore(readySemaphore);
    MPDeleteQueue(taskQueue);
	mpDataBuffer   = nil;
	readySemaphore = nil;
	taskQueue      = nil;

	S9xTracef( "Print f: MP: Exited DirectDraw thread.\n");

	return err;
}

static OSStatus BlitMPGLTask(void *parameter)
{
	OSStatus	err = noErr;
    long		theCommand, param1, param2;

	#pragma unused (parameter)

	S9xTracef( "Print f: MP: Entered BlitGL thread.\n");

	for (;;)
    {
		err = MPWaitOnQueue(taskQueue, (void **) &theCommand, (void **) &param1, (void **) &param2, kDurationForever);
        if (err)
			break;

		if (theCommand == kMPBlitFrame)
		{
			(mpDataBuffer->blitFn) ((uint8 *) mpDataBuffer->gfxBuffer, blitGLBuffer, mpDataBuffer->dstRowBytes, &mpDataBuffer->srcRect, &mpDataBuffer->copyRect);
 			if (!ciFilterEnable)
				S9xPutImageBlitGL2(mpDataBuffer->copyRect.right, mpDataBuffer->copyRect.bottom);
			else
				S9xPutImageBlitGL2CoreImage(mpDataBuffer->copyRect.right, mpDataBuffer->copyRect.bottom);
			MPSignalSemaphore(readySemaphore);
		}
		else
		if (theCommand == kMPBlitNone)
			MPSignalSemaphore(readySemaphore);
		else
		if (theCommand == kMPBlitDone)
			break;
		else
		{
			err = userCanceledErr;
			break;
		}
	}

	MPFree(mpDataBuffer);
	MPDeleteSemaphore(readySemaphore);
    MPDeleteQueue(taskQueue);
	mpDataBuffer   = nil;
	readySemaphore = nil;
	taskQueue      = nil;

	S9xTracef( "Print f: MP: Exited BlitGL thread.\n");

	return err;
}

static OSStatus PrepareMPBlit(void)
{
	OSStatus	err;

	mpDataBuffer = (MPData *) MPAllocateAligned(sizeof(MPData), kMPAllocateDefaultAligned, kMPAllocateClearMask);
	if (!mpDataBuffer)
		return memFullErr;

	err = MPCreateQueue(&notificationQueue);
	if (err == noErr)
	{
		err = MPCreateQueue(&taskQueue);
		if (err == noErr)
		{
			err = MPCreateBinarySemaphore(&readySemaphore);
			if (err == noErr)
			{
				MPSignalSemaphore(readySemaphore);
				err = MPCreateTask(BlitMPTask, nil, 0, notificationQueue, nil, nil, 0, &taskID);
			}
		}
	}

	return err;
}

static OSStatus PrepareMPBlitGL(void)
{
	OSStatus	err;

	mpDataBuffer = (MPData *) MPAllocateAligned(sizeof(MPData), kMPAllocateDefaultAligned, kMPAllocateClearMask);
	if (!mpDataBuffer)
		return memFullErr;

	err = MPCreateQueue(&notificationQueue);
	if (err == noErr)
	{
		err = MPCreateQueue(&taskQueue);
		if (err == noErr)
		{
			err = MPCreateBinarySemaphore(&readySemaphore);
			if (err == noErr)
			{
				MPSignalSemaphore(readySemaphore);
				err = MPCreateTask(BlitMPGLTask, nil, 0, notificationQueue, nil, nil, 0, &taskID);
			}
		}
	}

	return err;
}

void DrawPauseScreen(CGContextRef ctx, HIRect bounds)
{
	CGImageRef	image;
	CGRect		crt;
	Rect		copyRect;
	short		width, height;

	width  = IPPU.RenderedScreenWidth;
	height = IPPU.RenderedScreenHeight;

	if ((width == 0) || (height == 0))
		return;

	copyRect.top  = 0;
	copyRect.left = 0;

	if (width > 256)
	{
		copyRect.right = width;

		if (height > 256)
		{
			copyRect.bottom = height;

			if (!drawoverscan)
			{
				if (height < (SNES_HEIGHT_EXTENDED << 1))
				{
					copyRect.top     = kMarginDouble;
					copyRect.bottom += kMarginDouble;
				}
				else
				{
					copyRect.top     = kMarginDoubleExt;
					copyRect.bottom += kMarginDoubleExt;
				}
			}
		}
		else
		{
			copyRect.bottom = height << 1;

			if (!drawoverscan)
			{
				if (height < SNES_HEIGHT_EXTENDED)
				{
					copyRect.top     = kMarginDouble;
					copyRect.bottom += kMarginDouble;
				}
				else
				{
					copyRect.top     = kMarginDoubleExt;
					copyRect.bottom += kMarginDoubleExt;
				}
			}
		}
	}
	else
	{
		if (doubleSize)
		{
			copyRect.right  = width  << 1;
			copyRect.bottom = height << 1;

			if (!drawoverscan)
			{
				if (height < SNES_HEIGHT_EXTENDED)
				{
					copyRect.top     = kMarginDouble;
					copyRect.bottom += kMarginDouble;
				}
				else
				{
					copyRect.top     = kMarginDoubleExt;
					copyRect.bottom += kMarginDoubleExt;
				}
			}
		}
		else
		{
			copyRect.top    = 120;
			copyRect.left   = 128;
			copyRect.bottom = 120 + height;
			copyRect.right  = 128 + width;

			if (!drawoverscan)
			{
				if (height < SNES_HEIGHT_EXTENDED)
				{
					copyRect.top    += kMarginSingle;
					copyRect.bottom += kMarginSingle;
				}
			}
		}
	}

	image = CreateGameScreenCGImage();
	if (image)
	{
		double	rx, ry;
		int		ofs;

		rx = bounds.size.width / 512.0;

		if (!drawoverscan)
		{
			if (drawingMethod == kDrawingDirect)
			{
				ofs = copyRect.top;
				ry = bounds.size.height / (double) kMacWindowHeight;
			}
			else
			if (windowExtend)
			{
				ofs = copyRect.top;
				ry = bounds.size.height / (double) kMacWindowHeight;
			}
			else
			{
				ofs = 0;
				ry = bounds.size.height / (double) ((height <= 256) ? (height << 1) : height);
			}
		}
		else
		{
			if (!doubleSize && (width <= SNES_WIDTH))
				ofs = kMacWindowHeight - (120 + height);
			else
				ofs = kMacWindowHeight - ((height <= 256) ? (height << 1) : height);

			ry = bounds.size.height / (double) kMacWindowHeight;
		}

		CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);
		CGContextFillRect(ctx, bounds);

		crt = CGRectMake(0, 0, copyRect.right - copyRect.left, copyRect.bottom - copyRect.top);
		crt.size.width  *= rx;
		crt.size.height *= ry;
		crt.origin.x = (double) (int) (rx * (double) copyRect.left);
		crt.origin.y = (double) (int) (ry * (double) ofs);
		CGContextDrawImage(ctx, crt, image);

		CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 0.5);
		CGContextFillRect(ctx, bounds);

		CGImageRelease(image);
	}
}

void DrawFreezeDefrostScreen(uint8 *draw)
{
	const int	w = SNES_WIDTH << 1, h = kMacWindowHeight;

	imageWidth[0] = imageHeight[0] = 0;
	imageWidth[1] = imageHeight[1] = 0;
	prevBlitWidth = prevBlitHeight = 0;

	if (((drawingMethod == kDrawingDirect) || (drawingMethod == kDrawingBlitGL)) && multiprocessor)
	{
		MPWaitOnSemaphore(readySemaphore, kDurationForever);
		S9xTracef( "Print f: MP: Send dummy signal.\n");
		MPNotifyQueue(taskQueue, (void *) kMPBlitNone, 0, 0);
	}

	memcpy(blitGLBuffer, draw, w * h * 2);

	if (drawingMethod == kDrawingDirect)
	{
		Rect	rct  = { 0, 0, kMacWindowHeight, 512 };

		if (!fullscreen)
		{
			BEGIN_UPDATE_WINDOW_BACKBUFFER
			BlitPixSmall16(blitGLBuffer, gWinScreenPtr, gMacRowBytes, &rct, &rct);
			END_UPDATE_WINDOW_BACKBUFFER
		}
		else
			BlitPixSmall16(blitGLBuffer, gWinScreenPtr, gMacRowBytes, &rct, &rct);
	}
	else
	{
		if (!ciFilterEnable)
			S9xPutImageBlitGL2(512, kMacWindowHeight);
		else
			S9xPutImageBlitGL2CoreImage(512, kMacWindowHeight);
	}
}

void ClearGFXScreen(void)
{
	uint16	*p, *q, x, y;

	p = gfxScreen[0];
	q = gfxScreen[1];

	for (x = 0; x < 512; x++)
		for (y = 0; y < 512; y++)
			*p++ = *q++ = 0;

	BlitUpdateEntireDelta();

	imageWidth[0] = imageHeight[0] = 0;
	imageWidth[1] = imageHeight[1] = 0;
	prevBlitWidth = prevBlitHeight = 0;

	if (drawingMethod == kDrawingDirect)
		S9xPutImageDirect(512, kMacWindowHeight);
	else
	{
		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);
			glViewport(0, 0, glScreenW, glScreenH);
		}
		else
		{
			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);
			glViewport(0, 0, gWindowRect.right - gWindowRect.left, gWindowRect.bottom - gWindowRect.top);
		}

		glClearColor(0.0, 0.0, 0.0, 0.0);

		for (int i = 0; i < 2; i++)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			if (fullscreen)
				CGLFlushDrawable(glContext);
			else
				aglSwapBuffers(agContext);
		}
	}
}

static void S9xInitFullScreen(void)
{
	CFDictionaryRef	mode;
	boolean_t		exactMatch;
	size_t 			width, height;
	int				depth = 16;

	DeinitGameWindow();

	oldDisplayMode = CGDisplayCurrentMode(gGameDisplayID);

	if (autoRes)
	{
		width  = 640;
		height = 480;
	}
	else
	{
		width  = CGDisplayPixelsWide(gGameDisplayID);
		height = CGDisplayPixelsHigh(gGameDisplayID);
	}

	if (((drawingMethod == kDrawingOpenGL) || (drawingMethod == kDrawingBlitGL)) && gl32bit)
		depth = 32;

	mode = CGDisplayBestModeForParameters(gGameDisplayID, depth, width, height, &exactMatch);

	CGDisplayCapture(gGameDisplayID);
	CGDisplaySwitchToMode(gGameDisplayID, mode);

	CGDisplayErr		cgErr;
	CGDisplayCount		numDisplays, maxDisplays = 32;
	CGDirectDisplayID	activeDisplays[32];

	cgErr = CGGetActiveDisplayList(maxDisplays, activeDisplays, &numDisplays);
	if (cgErr == noErr)
	{
		if ((macControllerOption == SNES_MOUSE) || (macControllerOption == SNES_MOUSE_SWAPPED) || (numDisplays == 1))
			CGDisplayHideCursor(gGameDisplayID);

		if ((macControllerOption == SNES_MOUSE) || (macControllerOption == SNES_MOUSE_SWAPPED))
		{
			CGDisplayMoveCursorToPoint(gGameDisplayID, CGPointMake((double) (width >> 1), (double) (height >> 1)));
			CGAssociateMouseAndMouseCursorPosition(false);
		}
	}
}

static void S9xDeinitFullScreen(void)
{
	CGAssociateMouseAndMouseCursorPosition(true);
	CGDisplayShowCursor(gGameDisplayID);

	CGDisplaySwitchToMode(gGameDisplayID, oldDisplayMode);
    CGDisplayRelease(gGameDisplayID);
}

static void S9xInitWindowMode(void)
{
	CFDictionaryRef	mode;
	Rect			rct;
	boolean_t		exactMatch;
	size_t			width, height;
	int				depth = 16;

	oldDisplayMode = CGDisplayCurrentMode(gGameDisplayID);

	width  = CGDisplayPixelsWide(gGameDisplayID);
	height = CGDisplayPixelsHigh(gGameDisplayID);

	if (((drawingMethod == kDrawingOpenGL) || (drawingMethod == kDrawingBlitGL)) && gl32bit)
		depth = 32;

	mode = CGDisplayBestModeForParameters(gGameDisplayID, depth, width, height, &exactMatch);

	CGDisplaySwitchToMode(gGameDisplayID, mode);

	InitGameWindow();
	ShowWindow(gWindow);

	gWindowPort = GetWindowPort(gWindow);

#ifdef MAC_ENABLE_DIRECT_MODE
	gWindowRgn = NewRgn();
	SetRectRgn(gWindowRgn, 0, 0, 512, kMacWindowHeight);
#endif

	GetWindowBounds(gWindow, kWindowContentRgn, &gWindowRect);

	GetWindowBounds(gWindow, kWindowStructureRgn, &rct);
	gWindowBarHeight = gWindowRect.top - rct.top;

	if (drawingMethod != kDrawingDirect)
		UpdateGameWindow();
}

static void S9xDeinitWindowMode(void)
{
#ifdef MAC_ENABLE_DIRECT_MODE
	DisposeRgn(gWindowRgn);
#endif
	gWindowPort = nil;
	gWindowRgn  = nil;

    CGDisplaySwitchToMode(gGameDisplayID, oldDisplayMode);

	UpdateGameWindow();
}

static void S9xInitDirectFullScreen(void)
{
	size_t	offsetX, offsetY;

	glScreenW = CGDisplayPixelsWide(gGameDisplayID);
	glScreenH = CGDisplayPixelsHigh(gGameDisplayID);

	offsetX = (glScreenW - 512) >> 1;
	offsetY = (glScreenH - kMacWindowHeight) >> 1;

	gMacRowBytes  = CGDisplayBytesPerRow(gGameDisplayID);
	gMacScreenPtr = (unsigned char *) CGDisplayBaseAddress(gGameDisplayID);
	gWinScreenPtr = gMacScreenPtr + offsetY * gMacRowBytes + offsetX * 2;

	BlitFn256x240 = doubleSize ? (hq2xMode ? BlitPixHQ2x16 : (epxMode ? BlitPixEPX16: (supsaiMode ? BlitPixSuper2xSaI16 : (saiMode ? BlitPix2xSaI16 : (eagleMode ? BlitPixEagle16 : (tvMode ? BlitPixScaledTV16 : (smoothMode ? BlitPixFiltered16 : BlitPixScaled16))))))) : BlitPixSmall16;
	BlitFn512x240 = tvMode ? BlitPixHiResTV16 : BlitPixHiRes16;
	BlitFn512x480 = BlitPixSmall16;

	BlitUpdateEntireDelta();

	if (multiprocessor)
	{
		S9xTracef( "Print f: MP: Creating DirectDraw thread.\n");

		if (noErr != PrepareMPBlit())
			multiprocessor = false;
	}
}

static void S9xDeinitDirectFullScreen(void)
{
	if (multiprocessor)
	{
		MPNotifyQueue(taskQueue, (void *) kMPBlitDone, 0, 0);
		MPWaitOnQueue(notificationQueue, nil, nil, nil, kDurationForever);
		MPDeleteQueue(notificationQueue);
		notificationQueue = nil;

		S9xTracef( "Print f: MP: Successfully received terminate signal from DirectDraw thread.\n");
	}
}

static void S9xInitDirectWindowMode(void)
{
	gMacRowBytes  = 0;
	gMacScreenPtr = nil;
	gWinScreenPtr = nil;

	BlitFn256x240 = doubleSize ? (hq2xMode ? BlitPixHQ2x16 : (epxMode ? BlitPixEPX16: (supsaiMode ? BlitPixSuper2xSaI16 : (saiMode ? BlitPix2xSaI16 : (eagleMode ? BlitPixEagle16 : (tvMode ? BlitPixScaledTV16 : (smoothMode ? BlitPixFiltered16 : BlitPixScaled16))))))) : BlitPixSmall16;
	BlitFn512x240 = tvMode ? BlitPixHiResTV16 : BlitPixHiRes16;
	BlitFn512x480 = BlitPixSmall16;

	BlitUpdateEntireDelta();

	if (multiprocessor)
	{
		S9xTracef( "Print f: MP: Creating DirectDraw thread.\n");

		if (noErr != PrepareMPBlit())
			multiprocessor = false;
	}
}

static void S9xDeinitDirectWindowMode(void)
{
	if (multiprocessor)
	{
		MPNotifyQueue(taskQueue, (void *) kMPBlitDone, 0, 0);
		MPWaitOnQueue(notificationQueue, nil, nil, nil, kDurationForever);
		MPDeleteQueue(notificationQueue);
		notificationQueue = nil;

		S9xTracef( "Print f: MP: Successfully received terminate signal from DirectDraw thread.\n");
	}
}

static void S9xInitOpenGLFullScreen(void)
{
	CGOpenGLDisplayMask	displayMask;
	long 				numPixelFormats;

	displayMask = CGDisplayIDToOpenGLDisplayMask(gGameDisplayID);
	CGLPixelFormatAttribute	attribs[] = { (CGLPixelFormatAttribute) kCGLPFAFullScreen,
										  (CGLPixelFormatAttribute) kCGLPFADoubleBuffer,
										  (CGLPixelFormatAttribute) kCGLPFAAccelerated,
										  (CGLPixelFormatAttribute) kCGLPFANoRecovery,
										  (CGLPixelFormatAttribute) kCGLPFAColorSize,
										  (CGLPixelFormatAttribute) (gl32bit ? 32 : 16),
										  (CGLPixelFormatAttribute) kCGLPFADisplayMask,
										  (CGLPixelFormatAttribute) displayMask,
										  (CGLPixelFormatAttribute) 0 };

	CGLChoosePixelFormat(attribs, &cglpix, &numPixelFormats);
	CGLCreateContext(cglpix, nil, &glContext);
	glSwapInterval = vsync ? 1 : 0;
	if (extraOptions.benchmark)
		glSwapInterval = 0;
	CGLSetParameter(glContext, kCGLCPSwapInterval, &glSwapInterval);
	CGLSetCurrentContext(glContext);
	CGLSetFullScreen(glContext);

	glScreenW = CGDisplayPixelsWide(gGameDisplayID);
	glScreenH = CGDisplayPixelsHigh(gGameDisplayID);
}

static void S9xDeinitOpenGLFullScreen(void)
{
	if (glContext)
	{
		CGLSetCurrentContext(nil);
		CGLClearDrawable(glContext);
		CGLDestroyContext(glContext);
		CGLDestroyPixelFormat(cglpix);
	}
}

static void S9xInitOpenGLWindowMode(void)
{
	GLint	attribs[] = { AGL_RGBA,
						  AGL_DOUBLEBUFFER,
						  AGL_ACCELERATED,
						  AGL_NO_RECOVERY,
						  AGL_PIXEL_SIZE, gl32bit ? 32 : 16,
						  AGL_NONE };

	aglpix = aglChoosePixelFormat(nil, 0, attribs);
	agContext = aglCreateContext(aglpix, nil);
	aglSetDrawable(agContext, GetWindowPort(gWindow));
	agSwapInterval = vsync ? 1 : 0;
	if (extraOptions.benchmark)
		agSwapInterval = 0;
	aglSetInteger(agContext, AGL_SWAP_INTERVAL, &agSwapInterval);
	aglSetCurrentContext(agContext);

	if (systemVersion >= 0x1040)
	{
		aglGetCGLPixelFormat(aglpix, (void **) &cglpix);
		aglGetCGLContext(agContext, (void **) &glContext);
	}
}

static void S9xDeinitOpenGLWindowMode(void)
{
	if (agContext)
	{
		aglSetDrawable(agContext, nil);
		aglSetCurrentContext(nil);
		aglDestroyContext(agContext);
		aglDestroyPixelFormat(aglpix);
	}
}

static void S9xInitBlitGL(void)
{
	if (multiprocessor)
	{
		S9xTracef( "Print f: MP: Creating BlitGL thread.\n");

		if (noErr != PrepareMPBlitGL())
			multiprocessor = false;
	}
}

static void S9xDeinitBlitGL(void)
{
	if (multiprocessor)
	{
		MPNotifyQueue(taskQueue, (void *) kMPBlitDone, 0, 0);
		MPWaitOnQueue(notificationQueue, nil, nil, nil, kDurationForever);
		MPDeleteQueue(notificationQueue);
		notificationQueue = nil;

		S9xTracef( "Print f: MP: Successfully received terminate signal from BlitGL thread.\n");
	}
}

static void S9xInitOpenGLContext(void)
{
	OpenGL.internal_format = GL_RGB5_A1;
	OpenGL.format          = GL_BGRA;
	OpenGL.type            = GL_UNSIGNED_SHORT_1_5_5_5_REV;
	OpenGL.rangeExt        = gluCheckExtension((const GLubyte *) "GL_APPLE_texture_range", glGetString(GL_EXTENSIONS));
	OpenGL.target          = OpenGL.rangeExt ? GL_TEXTURE_RECTANGLE_EXT : GL_TEXTURE_2D;

	GLint	storage_hint  = GL_STORAGE_SHARED_APPLE;
	GLint	storage_apple = 1;
	GLdouble	agp_texturing = 0.0f;

	storage_apple = extraOptions.glUseClientStrageApple ? 1 : 0;

	agp_texturing = extraOptions.glUseTexturePriority ? 0.0f : 1.0f;

	switch (extraOptions.glStorageHint)
	{
		case 1:	storage_hint = GL_STORAGE_PRIVATE_APPLE;	break;
		case 2:	storage_hint = GL_STORAGE_CACHED_APPLE;		break;
		case 3:	storage_hint = GL_STORAGE_SHARED_APPLE;		break;
	}

	if (screencurvature || extraOptions.glForceNoTextureRectangle)
	{
		OpenGL.rangeExt = false;
		OpenGL.target   = GL_TEXTURE_2D;
	}

	S9xTracef( "Print f: TextureRange: %s\n", OpenGL.rangeExt ? "enable" : "disable");

	glDisable(GL_BLEND);
	glDisable(GL_DITHER);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	glEnable(OpenGL.target);

	glGenTextures(kGLNumTextures, OpenGL.textures);

	glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, storage_apple);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 8);

	// 256 * 256 (256 * 224, 256 * 239)

	OpenGL.texW[kGL256256] = 256;
	OpenGL.texH[kGL256256] = OpenGL.rangeExt ? SNES_HEIGHT_EXTENDED : 256;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGL256256][0] = 0;						OpenGL.vertex[kGL256256][1] = 0;
		OpenGL.vertex[kGL256256][2] = OpenGL.texW[kGL256256];   OpenGL.vertex[kGL256256][3] = 0;
		OpenGL.vertex[kGL256256][4] = OpenGL.texW[kGL256256];   OpenGL.vertex[kGL256256][5] = OpenGL.texH[kGL256256];
		OpenGL.vertex[kGL256256][6] = 0;						OpenGL.vertex[kGL256256][7] = OpenGL.texH[kGL256256];
	}
	else
	{
		OpenGL.vertex[kGL256256][0] = 0.0;						OpenGL.vertex[kGL256256][1] = 0.0;
		OpenGL.vertex[kGL256256][2] = 1.0;						OpenGL.vertex[kGL256256][3] = 0.0;
		OpenGL.vertex[kGL256256][4] = 1.0;						OpenGL.vertex[kGL256256][5] = 1.0;
		OpenGL.vertex[kGL256256][6] = 0.0;						OpenGL.vertex[kGL256256][7] = 1.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGL256256]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGL256256] * OpenGL.texH[kGL256256] * BYTES_PER_PIXEL, GFX.Screen);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGL256256], OpenGL.texH[kGL256256], 0, OpenGL.format, OpenGL.type, GFX.Screen);

	// 512 * 256 (512 * 224, 512 * 239)

	OpenGL.texW[kGL512256] = 512;
	OpenGL.texH[kGL512256] = OpenGL.rangeExt ? SNES_HEIGHT_EXTENDED : 256;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGL512256][0] = 0;						OpenGL.vertex[kGL512256][1] = 0;
		OpenGL.vertex[kGL512256][2] = OpenGL.texW[kGL512256];   OpenGL.vertex[kGL512256][3] = 0;
		OpenGL.vertex[kGL512256][4] = OpenGL.texW[kGL512256];   OpenGL.vertex[kGL512256][5] = OpenGL.texH[kGL512256];
		OpenGL.vertex[kGL512256][6] = 0;						OpenGL.vertex[kGL512256][7] = OpenGL.texH[kGL512256];
	}
	else
	{
		OpenGL.vertex[kGL512256][0] = 0.0;						OpenGL.vertex[kGL512256][1] = 0.0;
		OpenGL.vertex[kGL512256][2] = 1.0;						OpenGL.vertex[kGL512256][3] = 0.0;
		OpenGL.vertex[kGL512256][4] = 1.0;						OpenGL.vertex[kGL512256][5] = 1.0;
		OpenGL.vertex[kGL512256][6] = 0.0;						OpenGL.vertex[kGL512256][7] = 1.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGL512256]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGL512256] * OpenGL.texH[kGL512256] * BYTES_PER_PIXEL, GFX.Screen);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGL512256], OpenGL.texH[kGL512256], 0, OpenGL.format, OpenGL.type, GFX.Screen);

	// 512 * 512 (512 * 448, 512 * 478)

	OpenGL.texW[kGL512512] = 512;
	OpenGL.texH[kGL512512] = OpenGL.rangeExt ? (SNES_HEIGHT_EXTENDED << 1) : 512;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGL512512][0] = 0;						OpenGL.vertex[kGL512512][1] = 0;
		OpenGL.vertex[kGL512512][2] = OpenGL.texW[kGL512512];   OpenGL.vertex[kGL512512][3] = 0;
		OpenGL.vertex[kGL512512][4] = OpenGL.texW[kGL512512];   OpenGL.vertex[kGL512512][5] = OpenGL.texH[kGL512512];
		OpenGL.vertex[kGL512512][6] = 0;						OpenGL.vertex[kGL512512][7] = OpenGL.texH[kGL512512];
	}
	else
	{
		OpenGL.vertex[kGL512512][0] = 0.0;						OpenGL.vertex[kGL512512][1] = 0.0;
		OpenGL.vertex[kGL512512][2] = 1.0;						OpenGL.vertex[kGL512512][3] = 0.0;
		OpenGL.vertex[kGL512512][4] = 1.0;						OpenGL.vertex[kGL512512][5] = 1.0;
		OpenGL.vertex[kGL512512][6] = 0.0;						OpenGL.vertex[kGL512512][7] = 1.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGL512512]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGL512512] * OpenGL.texH[kGL512512] * BYTES_PER_PIXEL, GFX.Screen);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, smoothMode ? GL_LINEAR : GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGL512512], OpenGL.texH[kGL512512], 0, OpenGL.format, OpenGL.type, GFX.Screen);

	// Blit 2x

	OpenGL.texW[kGLBlit2x] = 512;
	OpenGL.texH[kGLBlit2x] = OpenGL.rangeExt ? (SNES_HEIGHT_EXTENDED * 2) : 512;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGLBlit2x][0] = 0;						OpenGL.vertex[kGLBlit2x][1] = 0;
		OpenGL.vertex[kGLBlit2x][2] = OpenGL.texW[kGLBlit2x];   OpenGL.vertex[kGLBlit2x][3] = 0;
		OpenGL.vertex[kGLBlit2x][4] = OpenGL.texW[kGLBlit2x];   OpenGL.vertex[kGLBlit2x][5] = OpenGL.texH[kGLBlit2x];
		OpenGL.vertex[kGLBlit2x][6] = 0;						OpenGL.vertex[kGLBlit2x][7] = OpenGL.texH[kGLBlit2x];
	}
	else
	{
		OpenGL.vertex[kGLBlit2x][0] = 0.0;						OpenGL.vertex[kGLBlit2x][1] = 0.0;
		OpenGL.vertex[kGLBlit2x][2] = 1.0;						OpenGL.vertex[kGLBlit2x][3] = 0.0;
		OpenGL.vertex[kGLBlit2x][4] = 1.0;						OpenGL.vertex[kGLBlit2x][5] = 1.0;
		OpenGL.vertex[kGLBlit2x][6] = 0.0;						OpenGL.vertex[kGLBlit2x][7] = 1.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGLBlit2x]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGLBlit2x] * OpenGL.texH[kGLBlit2x] * BYTES_PER_PIXEL, blitGLBuffer);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGLBlit2x], OpenGL.texH[kGLBlit2x], 0, OpenGL.format, OpenGL.type, blitGLBuffer);

	// Blit 3x

	OpenGL.texW[kGLBlit3x] = OpenGL.rangeExt ? (SNES_WIDTH * 3) : 1024;
	OpenGL.texH[kGLBlit3x] = OpenGL.rangeExt ? (SNES_HEIGHT_EXTENDED * 3) : 1024;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGLBlit3x][0] = 0;						OpenGL.vertex[kGLBlit3x][1] = 0;
		OpenGL.vertex[kGLBlit3x][2] = OpenGL.texW[kGLBlit3x];   OpenGL.vertex[kGLBlit3x][3] = 0;
		OpenGL.vertex[kGLBlit3x][4] = OpenGL.texW[kGLBlit3x];   OpenGL.vertex[kGLBlit3x][5] = OpenGL.texH[kGLBlit3x];
		OpenGL.vertex[kGLBlit3x][6] = 0;						OpenGL.vertex[kGLBlit3x][7] = OpenGL.texH[kGLBlit3x];
	}
	else
	{
		OpenGL.vertex[kGLBlit3x][0] = 0.0;						OpenGL.vertex[kGLBlit3x][1] = 0.0;
		OpenGL.vertex[kGLBlit3x][2] = SNES_WIDTH * 3 / 1024.0;	OpenGL.vertex[kGLBlit3x][3] = 0.0;
		OpenGL.vertex[kGLBlit3x][4] = SNES_WIDTH * 3 / 1024.0;	OpenGL.vertex[kGLBlit3x][5] = SNES_HEIGHT_EXTENDED * 3 / 1024.0;
		OpenGL.vertex[kGLBlit3x][6] = 0.0;						OpenGL.vertex[kGLBlit3x][7] = SNES_HEIGHT_EXTENDED * 3 / 1024.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGLBlit3x]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGLBlit3x] * OpenGL.texH[kGLBlit3x] * BYTES_PER_PIXEL, blitGLBuffer);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGLBlit3x], OpenGL.texH[kGLBlit3x], 0, OpenGL.format, OpenGL.type, blitGLBuffer);

	// Blit 4x

	OpenGL.texW[kGLBlit4x] = 1024;
	OpenGL.texH[kGLBlit4x] = OpenGL.rangeExt ? (SNES_HEIGHT_EXTENDED * 4) : 1024;

	if (OpenGL.rangeExt)
	{
		OpenGL.vertex[kGLBlit4x][0] = 0;						OpenGL.vertex[kGLBlit4x][1] = 0;
		OpenGL.vertex[kGLBlit4x][2] = OpenGL.texW[kGLBlit4x];   OpenGL.vertex[kGLBlit4x][3] = 0;
		OpenGL.vertex[kGLBlit4x][4] = OpenGL.texW[kGLBlit4x];   OpenGL.vertex[kGLBlit4x][5] = OpenGL.texH[kGLBlit4x];
		OpenGL.vertex[kGLBlit4x][6] = 0;						OpenGL.vertex[kGLBlit4x][7] = OpenGL.texH[kGLBlit4x];
	}
	else
	{
		OpenGL.vertex[kGLBlit4x][0] = 0.0;						OpenGL.vertex[kGLBlit4x][1] = 0.0;
		OpenGL.vertex[kGLBlit4x][2] = 1.0;						OpenGL.vertex[kGLBlit4x][3] = 0.0;
		OpenGL.vertex[kGLBlit4x][4] = 1.0;						OpenGL.vertex[kGLBlit4x][5] = 1.0;
		OpenGL.vertex[kGLBlit4x][6] = 0.0;						OpenGL.vertex[kGLBlit4x][7] = 1.0;
	}

	glBindTexture(OpenGL.target, OpenGL.textures[kGLBlit4x]);
	if (OpenGL.rangeExt)
	{
		glTextureRangeAPPLE(OpenGL.target, OpenGL.texW[kGLBlit4x] * OpenGL.texH[kGLBlit4x] * BYTES_PER_PIXEL, blitGLBuffer);
		glTexParameteri(OpenGL.target, GL_TEXTURE_STORAGE_HINT_APPLE, storage_hint);
	}
	glTexParameterf(OpenGL.target, GL_TEXTURE_PRIORITY, agp_texturing);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(OpenGL.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(OpenGL.target, 0, OpenGL.internal_format, OpenGL.texW[kGLBlit4x], OpenGL.texH[kGLBlit4x], 0, OpenGL.format, OpenGL.type, blitGLBuffer);

	//

	if (!screencurvature)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1.0, 1.0, -1.0, 1.0, 0.95, 5.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		scTexArray[kSC2xNormal] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC2xNormal], kSCMeshX, kSCMeshY, 1.0, 224.0 / 256.0);

		scTexArray[kSC2xExtend] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC2xExtend], kSCMeshX, kSCMeshY, 1.0, 239.0 / 256.0);

		scTexArray[kSC2xNHiRes] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC2xNHiRes], kSCMeshX, kSCMeshY, 1.0, 224.0 / 512.0);

		scTexArray[kSC2xEHiRes] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC2xEHiRes], kSCMeshX, kSCMeshY, 1.0, 239.0 / 512.0);

		scTexArray[kSC3xNormal] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC3xNormal], kSCMeshX, kSCMeshY, 768.0 / 1024.0, 672.0 / 1024.0);

		scTexArray[kSC3xExtend] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC3xExtend], kSCMeshX, kSCMeshY, 768.0 / 1024.0, 717.0 / 1024.0);

		scTexArray[kSC3xNHiRes] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC3xNHiRes], kSCMeshX, kSCMeshY, 768.0 / 1024.0, 672.0 / 2048.0);

		scTexArray[kSC3xEHiRes] = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 2];
		GLMakeTextureMesh(scTexArray[kSC3xEHiRes], kSCMeshX, kSCMeshY, 768.0 / 1024.0, 717.0 / 2048.0);

		scScnArray = new GLdouble [(kSCMeshX + 1) * 2 * kSCMeshY * 3];
		GLMakeScreenMesh(scScnArray, kSCMeshX, kSCMeshY);
	}

	if (fullscreen)
	{
		CGLSetCurrentContext(glContext);
		glViewport(0, 0, glScreenW, glScreenH);
	}
	else
	{
		aglSetCurrentContext(agContext);
		aglUpdateContext(agContext);
		glViewport(0, 0, gWindowRect.right - gWindowRect.left, gWindowRect.bottom - gWindowRect.top);
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 2; i++)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if (fullscreen)
			CGLFlushDrawable(glContext);
		else
			aglSwapBuffers(agContext);
	}
}

static void S9xDeinitOpenGLContext(void)
{
	if (screencurvature)
	{
		delete [] scTexArray[kSC2xNormal];
		delete [] scTexArray[kSC2xExtend];
		delete [] scTexArray[kSC2xNHiRes];
		delete [] scTexArray[kSC2xEHiRes];
		delete [] scTexArray[kSC3xNormal];
		delete [] scTexArray[kSC3xExtend];
		delete [] scTexArray[kSC3xNHiRes];
		delete [] scTexArray[kSC3xEHiRes];
		delete [] scScnArray;

		scTexArray[kSC2xNormal] = nil;
		scTexArray[kSC2xExtend] = nil;
		scTexArray[kSC2xNHiRes] = nil;
		scTexArray[kSC2xEHiRes] = nil;
		scTexArray[kSC3xNormal] = nil;
		scTexArray[kSC3xExtend] = nil;
		scTexArray[kSC3xNHiRes] = nil;
		scTexArray[kSC3xEHiRes] = nil;
		scScnArray              = nil;
	}

	glDeleteTextures(kGLNumTextures, OpenGL.textures);
}

static void S9xInitCoreImage(void)
{
#ifdef MAC_COREIMAGE_SUPPORT
	cgGameImage = nil;
	cgBlitImage = nil;

	InitCoreImageContext(glContext, cglpix);
#endif
}

static void S9xDeinitCoreImage(void)
{
#ifdef MAC_COREIMAGE_SUPPORT
	DeinitCoreImageContext();

	if (cgGameImage)
	{
		CGImageRelease(cgGameImage);
		cgGameImage = nil;
	}

	if (cgBlitImage)
	{
		CGImageRelease(cgBlitImage);
		cgBlitImage = nil;
	}
#endif
}

void S9xInitDisplay(int argc, char **argv)
{
	#pragma unused (argc, argv)

	if (directDisplay)
		return;

	gGameDisplayID = CGMainDisplayID();

	CGDisplayErr		cgErr;
	CGDisplayCount		numDisplays, maxDisplays = 32;
	CGDirectDisplayID	activeDisplays[32];
	CGPoint				windowAt;

	windowAt = CGPointMake((double) windowPos[kWindowScreen].h, (double) windowPos[kWindowScreen].v);

	cgErr = CGGetDisplaysWithPoint(windowAt, maxDisplays, activeDisplays, &numDisplays);
	if ((cgErr == noErr) && (numDisplays > 0))
	{
		for (unsigned int i = 0; i < numDisplays; i++)
		{
			if (activeDisplays[i] != CGMainDisplayID())
				gGameDisplayID = activeDisplays[i];
		}
	}

	glScreenBounds = CGDisplayBounds(gGameDisplayID);

	unlimitedCursor = CGPointMake(0.0, 0.0);

	imageWidth[0] = imageHeight[0] = 0;
	imageWidth[1] = imageHeight[1] = 0;
	prevBlitWidth = prevBlitHeight = 0;
	GFX.Screen    = gfxScreen[0];
	whichBuf      = 0;
	textureNum    = 0;
	nx            = hq4xMode ? 4 : (hq3xMode ? 3 : 2);

	if (fullscreen)
	{
		S9xInitFullScreen();

		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				gSrcP = 512;
				Settings.OpenGLEnable = true;
				S9xInitOpenGLFullScreen();
				S9xInitOpenGLContext();
				if (ciFilterEnable)
					S9xInitCoreImage();
				break;

			case kDrawingDirect:
				gSrcP = 1024;
				Settings.OpenGLEnable = false;
				S9xInitDirectFullScreen();
				break;

			case kDrawingBlitGL:
				gSrcP = 1024;
				Settings.OpenGLEnable = false;
				S9xInitOpenGLFullScreen();
				S9xInitOpenGLContext();
				if (ciFilterEnable)
					S9xInitCoreImage();
				S9xInitBlitGL();
		}
	}
	else
	{
		S9xInitWindowMode();

		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				gSrcP = 512;
				Settings.OpenGLEnable = true;
				S9xInitOpenGLWindowMode();
				S9xInitOpenGLContext();
				if (ciFilterEnable)
					S9xInitCoreImage();
				break;

			case kDrawingDirect:
				gSrcP = 1024;
				Settings.OpenGLEnable = false;
				S9xInitDirectWindowMode();
				break;

			case kDrawingBlitGL:
				gSrcP = 1024;
				Settings.OpenGLEnable = false;
				S9xInitOpenGLWindowMode();
				S9xInitOpenGLContext();
				if (ciFilterEnable)
					S9xInitCoreImage();
				S9xInitBlitGL();
		}
	}

	S9xSetSoundMute(!Settings.APUEnabled);
	Microseconds((UnsignedWide *) &lastFrame);

	windowResizeCount = 1;

	gettimeofday(&bencht1, nil);

	directDisplay = true;
}

void S9xDeinitDisplay(void)
{
	if (!directDisplay)
		return;

	S9xSetSoundMute(true);

	if (fullscreen)
	{
		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				if (ciFilterEnable)
					S9xDeinitCoreImage();
				S9xDeinitOpenGLContext();
				S9xDeinitOpenGLFullScreen();
				break;

			case kDrawingDirect:
				S9xDeinitDirectFullScreen();
				break;

			case kDrawingBlitGL:
				S9xDeinitBlitGL();
				if (ciFilterEnable)
					S9xDeinitCoreImage();
				S9xDeinitOpenGLContext();
				S9xDeinitOpenGLFullScreen();
		}

		S9xDeinitFullScreen();
	}
	else
	{
		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				if (ciFilterEnable)
					S9xDeinitCoreImage();
				S9xDeinitOpenGLContext();
				S9xDeinitOpenGLWindowMode();
				break;

			case kDrawingDirect:
				S9xDeinitDirectWindowMode();
				break;

			case kDrawingBlitGL:
				S9xDeinitBlitGL();
				if (ciFilterEnable)
					S9xDeinitCoreImage();
				S9xDeinitOpenGLContext();
				S9xDeinitOpenGLWindowMode();
		}

		S9xDeinitWindowMode();
	}

	Settings.OpenGLEnable = false;
	directDisplay = false;

	lastDrawingMethod = drawingMethod;
}

bool8 S9xInitUpdate(void)
{
	return true;
}

bool8 S9xDeinitUpdate(int width, int height)
{
	if (directDisplay)
		S9xPutImage(width, height);

	return true;
}

bool8 S9xContinueUpdate(int width, int height)
{
	return true;
}

void S9xPutImage(int width, int height)
{
	static double	fps   = 0.0;
	static long		count = 0;
	static char		text[32];

	if (extraOptions.benchmark)
	{
		uint16	*basePtr;
		long	delta, len;

		count++;

		gettimeofday(&bencht2, nil);

		delta = 1000000 * (bencht2.tv_sec - bencht1.tv_sec) + (bencht2.tv_usec - bencht1.tv_usec);
		if (delta > 1000000)
		{
			fps = (1000000.0 * (double) count) / (double) delta;
			count = 0;

			gettimeofday(&bencht1, nil);
		}

		sprintf(text, "%.1f", fps);

		basePtr = GFX.Screen + 1;
		len = strlen(text);

		for (int i = 0; i < len; i++)
		{
			DisplayChar(basePtr, text[i]);
			basePtr += (8 - 1);
		}
	}
	else
	{
		if (cfIsWatching)
			CheatFinderDrawWatchAddr();

		if (Settings.DisplayFrameRate)
		{
			static int	drawnFrames[60] = { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
											1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 };
			static int	tableIndex = 0;
			int			frameCalc = 0, count;

			drawnFrames[tableIndex] = skipFrames;

			if (Settings.TurboMode)
			{
				drawnFrames[tableIndex] = (drawnFrames[tableIndex] + (macFastForwardRate / 2)) / macFastForwardRate;
				if (drawnFrames[tableIndex] == 0)
					drawnFrames[tableIndex] = 1;
			}

			tableIndex = (tableIndex + 1) % 60;

			for (count = 0; count < 60; count++)
				frameCalc += drawnFrames[count];

			IPPU.DisplayedRenderedFrameCount = (Memory.ROMFramesPerSecond * 60) / frameCalc;
		}
	}

	if (!drawoverscan)
	{
		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				if (!ciFilterEnable)
					S9xPutImageOpenGL(width, height);
				else
					S9xPutImageCoreImage(width, height);
				break;

			case kDrawingDirect:
				S9xPutImageDirect(width, height);
				break;

			case kDrawingBlitGL:
				S9xPutImageBlitGL(width, height);
		}
	}
	else
	{
		switch (drawingMethod)
		{
			case kDrawingOpenGL:
				if (!ciFilterEnable)
					S9xPutImageOverscanOpenGL(width, height);
				else
					S9xPutImageOverscanCoreImage(width, height);
				break;

			case kDrawingDirect:
				S9xPutImageOverscanDirect(width, height);
				break;

			case kDrawingBlitGL:
				S9xPutImageOverscanBlitGL(width, height);
		}
	}
}

static void S9xPutImageDirect(int width, int height)
{
	Rect	srcRect = { 0, 0, height, width }, copyRect;
	Blitter	BlitFn;

	if ((width <= 256) && ((imageWidth[whichBuf] != width) || (imageHeight[whichBuf] != height)))
	{
		if (eagleMode || supsaiMode || saiMode || epxMode || hq2xMode)
			BlitClearExtendArea((uint8 *) GFX.Screen, &srcRect);
		else
			BlitUpdateEntireDelta();
	}

	copyRect.top  = 0;
	copyRect.left = 0;

	if (width > 256)
	{
		copyRect.right = width;

		if (height > 256)
		{
			copyRect.bottom = height;
			BlitFn = BlitFn512x480;

			if (height < (SNES_HEIGHT_EXTENDED << 1))
			{
				copyRect.top     = kMarginDouble;
				copyRect.bottom += kMarginDouble;
			}
			else
			{
				copyRect.top     = kMarginDoubleExt;
				copyRect.bottom += kMarginDoubleExt;
			}
		}
		else
		{
			copyRect.bottom = height << 1;
			BlitFn = BlitFn512x240;

			if (height < SNES_HEIGHT_EXTENDED)
			{
				copyRect.top     = kMarginDouble;
				copyRect.bottom += kMarginDouble;
			}
			else
			{
				copyRect.top     = kMarginDoubleExt;
				copyRect.bottom += kMarginDoubleExt;
			}
		}
	}
	else
	{
		if (doubleSize)
		{
			copyRect.right  = width  << 1;
			copyRect.bottom = height << 1;

			if (height < SNES_HEIGHT_EXTENDED)
			{
				copyRect.top     = kMarginDouble;
				copyRect.bottom += kMarginDouble;
			}
			else
			{
				copyRect.top     = kMarginDoubleExt;
				copyRect.bottom += kMarginDoubleExt;
			}
		}
		else
		{
			copyRect.top    = 120;
			copyRect.left   = 128;
			copyRect.bottom = 120 + height;
			copyRect.right  = 128 + width;

			if (height < SNES_HEIGHT_EXTENDED)
			{
				copyRect.top    += kMarginSingle;
				copyRect.bottom += kMarginSingle;
			}
		}

		BlitFn = BlitFn256x240;
	}

	imageWidth[whichBuf]  = width;
	imageHeight[whichBuf] = height;

	if (multiprocessor)
	{
		MPWaitOnSemaphore(readySemaphore, kDurationForever);

		mpDataBuffer->blitFn    = BlitFn;
		mpDataBuffer->srcRect   = srcRect;
		mpDataBuffer->copyRect  = copyRect;
		mpDataBuffer->gfxBuffer = GFX.Screen;

		MPNotifyQueue(taskQueue, (void *) kMPBlitFrame, 0, 0);

		whichBuf = 1 - whichBuf;
		GFX.Screen = gfxScreen[whichBuf];
	}
	else
	{
		if (!fullscreen)
		{
			BEGIN_UPDATE_WINDOW_BACKBUFFER
			BlitFn((uint8 *) GFX.Screen, gWinScreenPtr, gMacRowBytes, &srcRect, &copyRect);
			END_UPDATE_WINDOW_BACKBUFFER
		}
		else
			BlitFn((uint8 *) GFX.Screen, gWinScreenPtr, gMacRowBytes, &srcRect, &copyRect);
	}
}

static void S9xPutImageOpenGL(int width, int height)
{
	if ((imageWidth[0] != width) || (imageHeight[0] != height) || (windowResizeCount > 0))
	{
		int	vh = (height > 256) ? height : (height << 1);

		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				double   fpw = (double) glScreenH / vh * 512.0;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);

				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
				glViewport((glScreenW - 512) >> 1, (glScreenH - vh) >> 1, 512, vh);
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			if (windowExtend)
				glViewport(0, ((kMacWindowHeight - vh) >> 1) * wh / kMacWindowHeight, ww, vh * wh / kMacWindowHeight);
			else
				glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		textureNum = (width <= 256) ? kGL256256 : ((height <= 256) ? kGL512256 : kGL512512);
		OpenGL.vertex[textureNum][5] = OpenGL.vertex[textureNum][7] = OpenGL.rangeExt ? height : (vh / 512.0);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
		glBindTexture(OpenGL.target, OpenGL.textures[textureNum]);

		imageWidth[0]  = width;
		imageHeight[0] = height;
	}

	glTexSubImage2D(OpenGL.target, 0, 0, 0, OpenGL.texW[textureNum], OpenGL.texH[textureNum], OpenGL.format, OpenGL.type, GFX.Screen);

	if (!screencurvature)
	{
		glBegin(GL_QUADS);

		glTexCoord2fv(&OpenGL.vertex[textureNum][6]);
		glVertex2f(-1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][4]);
		glVertex2f( 1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][2]);
		glVertex2f( 1.0,  1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][0]);
		glVertex2f(-1.0,  1.0);

		glEnd();
	}
	else
	{
		GLdouble *t, *s;
		int		tex;

		if ((height == SNES_HEIGHT) || (height == (SNES_HEIGHT << 1)))
			tex = kSC2xNormal;
		else
			tex = kSC2xExtend;

		t = scTexArray[tex];
		s = scScnArray;

		for (int i = 0; i < kSCMeshY; i++)
		{
			glTexCoordPointer(2, GL_double, 0, t);
			glVertexPointer(3, GL_double, 0, s);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (kSCMeshX + 1) * 2);

			t += (kSCMeshX + 1) * 2 * 2;
			s += (kSCMeshX + 1) * 2 * 3;
		}
	}

	glFinishObjectAPPLE(GL_TEXTURE, OpenGL.textures[textureNum]);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
}

static void S9xPutImageBlitGL(int width, int height)
{
	Rect	srcRect = { 0, 0, height, width }, copyRect;
	Blitter	blitFn;

	copyRect.top  = 0;
	copyRect.left = 0;

	if ((width <= 256) && ((imageWidth[whichBuf] != width) || (imageHeight[whichBuf] != height)))
	{
		if (eagleMode || supsaiMode || saiMode || epxMode || hq2xMode || hq3xMode || hq4xMode)
			BlitClearExtendArea((uint8 *) GFX.Screen, &srcRect);
		else
			BlitUpdateEntireDelta();
	}

	switch (nx)
	{
		default:
		case 2:
			if (width <= 256)
			{
				copyRect.right  = width  * 2;
				copyRect.bottom = height * 2;
				blitFn = hq2xMode ? BlitPixHQ2x16 : (epxMode ? BlitPixEPX16: (supsaiMode ? BlitPixSuper2xSaI16 : (saiMode ? BlitPix2xSaI16 : (eagleMode ? BlitPixEagle16 : BlitPixScaledTV16))));
			}
			else
			if (tvMode && (height <= 256))
			{
				copyRect.right  = width;
				copyRect.bottom = height * 2;
				blitFn = BlitPixHiResTV16;
			}
			else
			{
				copyRect.right  = width;
				copyRect.bottom = height;
				blitFn = BlitPixSmall16;
			}

			break;

		case 3:
			if (width <= 256)
			{
				copyRect.right  = width  * 3;
				copyRect.bottom = height * 3;
				blitFn = /*hq3xMode ? */ BlitPixHQ3x16;
			}
			else
			{
				copyRect.right  = width;
				copyRect.bottom = height;
				blitFn = BlitPixSmall16;
			}

			break;

		case 4:
			if (width <= 256)
			{
				copyRect.right  = width  * 4;
				copyRect.bottom = height * 4;
				blitFn =  /*hq4xMode ? */ BlitPixHQ4x16;
			}
			else
			{
				copyRect.right  = width  * 2;
				copyRect.bottom = height * 2;
				blitFn =  /*hq4xMode ? */ BlitPixHQ2x16;
			}

			break;
	}

	imageWidth[whichBuf]  = width;
	imageHeight[whichBuf] = height;

	if (multiprocessor)
	{
		MPWaitOnSemaphore(readySemaphore, kDurationForever);

		mpDataBuffer->blitFn      = blitFn;
		mpDataBuffer->dstRowBytes = (OpenGL.rangeExt ? copyRect.right : ((copyRect.right > 512) ? 1024 : 512)) * 2;
		mpDataBuffer->srcRect     = srcRect;
		mpDataBuffer->copyRect    = copyRect;
		mpDataBuffer->gfxBuffer   = GFX.Screen;

		MPNotifyQueue(taskQueue, (void *) kMPBlitFrame, 0, 0);

		whichBuf = 1 - whichBuf;
		GFX.Screen = gfxScreen[whichBuf];
	}
	else
	{
		blitFn((uint8 *) GFX.Screen, blitGLBuffer, (OpenGL.rangeExt ? copyRect.right : ((copyRect.right > 512) ? 1024 : 512)) * 2, &srcRect, &copyRect);
		if (!ciFilterEnable)
			S9xPutImageBlitGL2(copyRect.right, copyRect.bottom);
		else
			S9xPutImageBlitGL2CoreImage(copyRect.right, copyRect.bottom);
	}
}

static void S9xPutImageCoreImage(int width, int height)
{
#ifdef MAC_COREIMAGE_SUPPORT
	if ((imageWidth[0] != width) || (imageHeight[0] != height) || (windowResizeCount > 0))
	{
		int	vh = (height > 256) ? height : (height << 1);

		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				double   fpw = (double) glScreenH / vh * 512.0;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);

				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
				glViewport((glScreenW - 512) >> 1, (glScreenH - vh) >> 1, 512, vh);
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			if (windowExtend)
				glViewport(0, ((kMacWindowHeight - vh) >> 1) * wh / kMacWindowHeight, ww, vh * wh / kMacWindowHeight);
			else
				glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);

		if (cgGameImage)
			CGImageRelease(cgGameImage);
		cgGameImage = CreateGameScreenCGImage();

		imageWidth[0]  = width;
		imageHeight[0] = height;
	}

	CGRect	src;

	src = CGRectMake(0, 0, width, height);
	DrawWithCoreImageFilter(src, cgGameImage);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
#endif
}

static void S9xPutImageOverscanDirect(int width, int height)
{
	Rect	srcRect, copyRect;
	Blitter	BlitFn;
	int		extbtm;

	extbtm = (height > 256) ? (SNES_HEIGHT_EXTENDED << 1) : SNES_HEIGHT_EXTENDED;

	srcRect.left   = 0;
	srcRect.top    = 0;
	srcRect.right  = width;
	srcRect.bottom = extbtm;

	if ((imageWidth[whichBuf] != width) || (imageHeight[whichBuf] != height))
	{
		if ((height == SNES_HEIGHT) || (height == (SNES_HEIGHT << 1)))
		{
			const unsigned int	pitch = 1024;
			unsigned int		i;
			uint32				*extarea;

			extarea = (uint32 *) ((uint8 *) GFX.Screen + height * pitch);
			for (i = 0; i < (((extbtm - height) * pitch) >> 2); i++)
				extarea[i] = 0;
		}

		if (width <= 256)
		{
			if (eagleMode || supsaiMode || saiMode || epxMode || hq2xMode)
				BlitClearExtendArea((uint8 *) GFX.Screen, &srcRect);
			else
				BlitUpdateEntireDelta();
		}
	}

	copyRect.top  = 0;
	copyRect.left = 0;

	if (width > 256)
	{
		copyRect.right  = width;
		copyRect.bottom = SNES_HEIGHT_EXTENDED << 1;
		BlitFn = (height > 256) ? BlitFn512x480 : BlitFn512x240;
	}
	else
	{
		if (doubleSize)
		{
			copyRect.right  = width << 1;
			copyRect.bottom = SNES_HEIGHT_EXTENDED << 1;
		}
		else
		{
			copyRect.top    = 120;
			copyRect.left   = 128;
			copyRect.bottom = 120 + SNES_HEIGHT_EXTENDED;
			copyRect.right  = 128 + width;
		}

		BlitFn = BlitFn256x240;
	}

	imageWidth[whichBuf]  = width;
	imageHeight[whichBuf] = height;

	if (multiprocessor)
	{
		MPWaitOnSemaphore(readySemaphore, kDurationForever);

		mpDataBuffer->blitFn    = BlitFn;
		mpDataBuffer->srcRect   = srcRect;
		mpDataBuffer->copyRect  = copyRect;
		mpDataBuffer->gfxBuffer = GFX.Screen;

		MPNotifyQueue(taskQueue, (void *) kMPBlitFrame, 0, 0);

		whichBuf = 1 - whichBuf;
		GFX.Screen = gfxScreen[whichBuf];
	}
	else
	{
		if (!fullscreen)
		{
			BEGIN_UPDATE_WINDOW_BACKBUFFER
			BlitFn((uint8 *) GFX.Screen, gWinScreenPtr, gMacRowBytes, &srcRect, &copyRect);
			END_UPDATE_WINDOW_BACKBUFFER
		}
		else
			BlitFn((uint8 *) GFX.Screen, gWinScreenPtr, gMacRowBytes, &srcRect, &copyRect);
	}
}

static void S9xPutImageOverscanOpenGL(int width, int height)
{
	if ((imageWidth[0] != width) || (imageHeight[0] != height) || (windowResizeCount > 0))
	{
		if ((height == SNES_HEIGHT) || (height == (SNES_HEIGHT << 1)))
		{
			int		pitch    = width << 1;
			int		extbtm   = (height > 256) ? (SNES_HEIGHT_EXTENDED << 1) : SNES_HEIGHT_EXTENDED;
			uint32	*extarea = (uint32 *) ((uint8 *) GFX.Screen + height * pitch);

			for (int i = 0; i < (((extbtm - height) * pitch) >> 2); i++)
				extarea[i] = 0;
		}

		const int	vh = SNES_HEIGHT_EXTENDED << 1;

		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				double   fpw = (double) glScreenH / vh * 512.0;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);

				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
				glViewport((glScreenW - 512) >> 1, (glScreenH - vh) >> 1, 512, vh);
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		textureNum = (width <= 256) ? kGL256256 : ((height <= 256) ? kGL512256 : kGL512512);
		OpenGL.vertex[textureNum][5] = OpenGL.vertex[textureNum][7] = OpenGL.rangeExt ? ((height > 256) ? vh : SNES_HEIGHT_EXTENDED) : (vh / 512.0);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
		glBindTexture(OpenGL.target, OpenGL.textures[textureNum]);

		imageWidth[0]  = width;
		imageHeight[0] = height;
	}

	glTexSubImage2D(OpenGL.target, 0, 0, 0, OpenGL.texW[textureNum], OpenGL.texH[textureNum], OpenGL.format, OpenGL.type, GFX.Screen);

	if (!screencurvature)
	{
		glBegin(GL_QUADS);

		glTexCoord2fv(&OpenGL.vertex[textureNum][6]);
		glVertex2f(-1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][4]);
		glVertex2f( 1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][2]);
		glVertex2f( 1.0,  1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][0]);
		glVertex2f(-1.0,  1.0);

		glEnd();
	}
	else
	{
		GLdouble *t, *s;

		t = scTexArray[kSC2xExtend];
		s = scScnArray;

		for (int i = 0; i < kSCMeshY; i++)
		{
			glTexCoordPointer(2, GL_double, 0, t);
			glVertexPointer(3, GL_double, 0, s);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (kSCMeshX + 1) * 2);

			t += (kSCMeshX + 1) * 2 * 2;
			s += (kSCMeshX + 1) * 2 * 3;
		}
	}

	glFinishObjectAPPLE(GL_TEXTURE, OpenGL.textures[textureNum]);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
}

static void S9xPutImageOverscanBlitGL(int width, int height)
{
	Rect	srcRect, copyRect;
	Blitter	blitFn;
	int		extbtm;

	extbtm = (height > 256) ? (SNES_HEIGHT_EXTENDED << 1) : SNES_HEIGHT_EXTENDED;

	srcRect.left   = 0;
	srcRect.top    = 0;
	srcRect.right  = width;
	srcRect.bottom = extbtm;

	if ((imageWidth[whichBuf] != width) || (imageHeight[whichBuf] != height))
	{
		if ((height == SNES_HEIGHT) || (height == (SNES_HEIGHT << 1)))
		{
			const unsigned int	pitch = 1024;
			unsigned int		i;
			uint32				*extarea;

			extarea = (uint32 *) ((uint8 *) GFX.Screen + height * pitch);
			for (i = 0; i < (((extbtm - height) * pitch) >> 2); i++)
				extarea[i] = 0;
		}

		if (width <= 256)
		{
			if (eagleMode || supsaiMode || saiMode || epxMode || hq2xMode || hq3xMode || hq4xMode)
				BlitClearExtendArea((uint8 *) GFX.Screen, &srcRect);
			else
				BlitUpdateEntireDelta();
		}
	}

	copyRect.top  = 0;
	copyRect.left = 0;

	switch (nx)
	{
		default:
		case 2:
			if (width <= 256)
			{
				copyRect.right  = width * 2;
				copyRect.bottom = SNES_HEIGHT_EXTENDED * 2;
				blitFn = hq2xMode ? BlitPixHQ2x16 : (epxMode ? BlitPixEPX16: (supsaiMode ? BlitPixSuper2xSaI16 : (saiMode ? BlitPix2xSaI16 : (eagleMode ? BlitPixEagle16 : BlitPixScaledTV16))));
			}
			else
			if (tvMode && (height <= 256))
			{
				copyRect.right  = width;
				copyRect.bottom = SNES_HEIGHT_EXTENDED * 2;
				blitFn = BlitPixHiResTV16;
			}
			else
			{
				copyRect.right  = width;
				copyRect.bottom = extbtm;
				blitFn = BlitPixSmall16;
			}

			break;

		case 3:
			if (width <= 256)
			{
				copyRect.right  = width * 3;
				copyRect.bottom = SNES_HEIGHT_EXTENDED * 3;
				blitFn =  /*hq3xMode ? */ BlitPixHQ3x16;
			}
			else
			{
				copyRect.right  = width;
				copyRect.bottom = extbtm;
				blitFn = BlitPixSmall16;
			}

			break;

		case 4:
			if (width <= 256)
			{
				copyRect.right  = width * 4;
				copyRect.bottom = SNES_HEIGHT_EXTENDED * 4;
				blitFn =  /*hq4xMode ? */ BlitPixHQ4x16;
			}
			else
			{
				copyRect.right  = width * 2;
				copyRect.bottom = extbtm * 2;
				blitFn =  /*hq4xMode ? */ BlitPixHQ2x16;
			}

			break;
	}

	imageWidth[whichBuf]  = width;
	imageHeight[whichBuf] = height;

	if (multiprocessor)
	{
		MPWaitOnSemaphore(readySemaphore, kDurationForever);

		mpDataBuffer->blitFn      = blitFn;
		mpDataBuffer->dstRowBytes = (OpenGL.rangeExt ? copyRect.right : ((copyRect.right > 512) ? 1024 : 512)) * 2;
		mpDataBuffer->srcRect     = srcRect;
		mpDataBuffer->copyRect    = copyRect;
		mpDataBuffer->gfxBuffer   = GFX.Screen;

		MPNotifyQueue(taskQueue, (void *) kMPBlitFrame, 0, 0);

		whichBuf = 1 - whichBuf;
		GFX.Screen = gfxScreen[whichBuf];
	}
	else
	{
		blitFn((uint8 *) GFX.Screen, blitGLBuffer, (OpenGL.rangeExt ? copyRect.right : ((copyRect.right > 512) ? 1024 : 512)) * 2, &srcRect, &copyRect);
		if (!ciFilterEnable)
			S9xPutImageBlitGL2(copyRect.right, copyRect.bottom);
		else
			S9xPutImageBlitGL2CoreImage(copyRect.right, copyRect.bottom);
	}
}

static void S9xPutImageOverscanCoreImage(int width, int height)
{
#ifdef MAC_COREIMAGE_SUPPORT
	int	extbtm = (height > 256) ? (SNES_HEIGHT_EXTENDED << 1) : SNES_HEIGHT_EXTENDED;

	if ((imageWidth[0] != width) || (imageHeight[0] != height) || (windowResizeCount > 0))
	{
		if ((height == SNES_HEIGHT) || (height == (SNES_HEIGHT << 1)))
		{
			int		pitch    = width << 1;
			uint32	*extarea = (uint32 *) ((uint8 *) GFX.Screen + height * pitch);

			for (int i = 0; i < (((extbtm - height) * pitch) >> 2); i++)
				extarea[i] = 0;
		}

		const int	vh = SNES_HEIGHT_EXTENDED << 1;

		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				double   fpw = (double) glScreenH / vh * 512.0;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);

				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
				glViewport((glScreenW - 512) >> 1, (glScreenH - vh) >> 1, 512, vh);
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height - extbtm, height, -1, 1);

		if (cgGameImage)
			CGImageRelease(cgGameImage);
		cgGameImage = CreateGameScreenCGImage();

		imageWidth[0]  = width;
		imageHeight[0] = height;
	}

	CGRect	src;

	src = CGRectMake(0, 0, width, height);
	DrawWithCoreImageFilter(src, cgGameImage);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
#endif
}

static void S9xPutImageBlitGL2(int blit_width, int blit_height)
{
	if ((prevBlitWidth != blit_width) || (prevBlitHeight != blit_height) || (windowResizeCount > 0))
	{
		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				int		sh  = (blit_width > blit_height * 2) ? (blit_height * 2) : blit_height;
				double	fpw = (double) glScreenH / (double) sh * (double) blit_width;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);
				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
			{
				int		sw = SNES_WIDTH * nx,
						sh = ((blit_height % SNES_HEIGHT) ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT) * nx;
				glViewport((glScreenW - sw) >> 1, (glScreenH - sh) >> 1, sw, sh);
			}
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			if (windowExtend)
			{
				int	bh = (blit_height % SNES_HEIGHT) ? (SNES_HEIGHT_EXTENDED << 1) : (SNES_HEIGHT << 1);
				glViewport(0, ((kMacWindowHeight - bh) >> 1) * wh / kMacWindowHeight, ww, bh * wh / kMacWindowHeight);
			}
			else
				glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		switch (blit_width / SNES_WIDTH)
		{
			default:
			case 2:	textureNum = kGLBlit2x;	break;
			case 3:	textureNum = kGLBlit3x;	break;
			case 4:	textureNum = kGLBlit4x;	break;
		}

		if (OpenGL.rangeExt)
		{
			OpenGL.vertex[textureNum][2] = OpenGL.vertex[textureNum][4] = blit_width;
			OpenGL.vertex[textureNum][5] = OpenGL.vertex[textureNum][7] = blit_height;
			glPixelStorei(GL_UNPACK_ROW_LENGTH, blit_width);
		}
		else
		{
			int	sl = (blit_width > 512) ? 1024 : 512;
			OpenGL.vertex[textureNum][2] = OpenGL.vertex[textureNum][4] = blit_width  / (double) sl;
			OpenGL.vertex[textureNum][5] = OpenGL.vertex[textureNum][7] = blit_height / (double) sl;
			glPixelStorei(GL_UNPACK_ROW_LENGTH, sl);
		}

		glBindTexture(OpenGL.target, OpenGL.textures[textureNum]);

		prevBlitWidth  = blit_width;
		prevBlitHeight = blit_height;
	}

	glTexSubImage2D(OpenGL.target, 0, 0, 0, OpenGL.texW[textureNum], OpenGL.texH[textureNum], OpenGL.format, OpenGL.type, blitGLBuffer);

	if (!screencurvature)
	{
		glBegin(GL_QUADS);

		glTexCoord2fv(&OpenGL.vertex[textureNum][6]);
		glVertex2f(-1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][4]);
		glVertex2f( 1.0, -1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][2]);
		glVertex2f( 1.0,  1.0);
		glTexCoord2fv(&OpenGL.vertex[textureNum][0]);
		glVertex2f(-1.0,  1.0);

		glEnd();
	}
	else
	{
		GLdouble *t, *s;
		int		tex;

		if (blit_width > blit_height * 2)
		{
			if (blit_width / SNES_WIDTH != 3)
				tex = (blit_height % SNES_HEIGHT) ? kSC2xEHiRes : kSC2xNHiRes;
			else
				tex = (blit_height % SNES_HEIGHT) ? kSC3xEHiRes : kSC3xNHiRes;
		}
		else
		{
			if (blit_width / SNES_WIDTH != 3)
				tex = (blit_height % SNES_HEIGHT) ? kSC2xExtend : kSC2xNormal;
			else
				tex = (blit_height % SNES_HEIGHT) ? kSC3xExtend : kSC3xNormal;
		}

		t = scTexArray[tex];
		s = scScnArray;

		for (int i = 0; i < kSCMeshY; i++)
		{
			glTexCoordPointer(2, GL_double, 0, t);
			glVertexPointer(3, GL_double, 0, s);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (kSCMeshX + 1) * 2);

			t += (kSCMeshX + 1) * 2 * 2;
			s += (kSCMeshX + 1) * 2 * 3;
		}
	}

	glFinishObjectAPPLE(GL_TEXTURE, OpenGL.textures[textureNum]);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
}

static void S9xPutImageBlitGL2CoreImage(int blit_width, int blit_height)
{
#ifdef MAC_COREIMAGE_SUPPORT
	if ((prevBlitWidth != blit_width) || (prevBlitHeight != blit_height) || (windowResizeCount > 0))
	{
		if (fullscreen)
		{
			CGLSetCurrentContext(glContext);

			if (glstretch)
			{
				int		sh  = (blit_width > blit_height * 2) ? (blit_height * 2) : blit_height;
				double	fpw = (double) glScreenH / (double) sh * (double) blit_width;
				int		pw  = (int) (fpw + ((double) glScreenW - fpw) * (double) macAspectRatio / 100.0);
				glViewport((glScreenW - pw) >> 1, 0, pw, glScreenH);
			}
			else
			{
				int		sw = SNES_WIDTH * nx,
						sh = ((blit_height % SNES_HEIGHT) ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT) * nx;
				glViewport((glScreenW - sw) >> 1, (glScreenH - sh) >> 1, sw, sh);
			}
		}
		else
		{
			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;

			aglSetCurrentContext(agContext);
			aglUpdateContext(agContext);

			if (windowExtend)
			{
				int	bh = (blit_height % SNES_HEIGHT) ? (SNES_HEIGHT_EXTENDED << 1) : (SNES_HEIGHT << 1);
				glViewport(0, ((kMacWindowHeight - bh) >> 1) * wh / kMacWindowHeight, ww, bh * wh / kMacWindowHeight);
			}
			else
				glViewport(0, 0, ww, wh);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		windowResizeCount--;

		int	sl = OpenGL.rangeExt ? blit_width : ((blit_width > 512) ? 1024 : 512);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, sl);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, blit_width, 0, blit_height, -1, 1);

		if (cgBlitImage)
			CGImageRelease(cgBlitImage);
		cgBlitImage = CreateBlitScreenCGImage(blit_width, blit_height, sl << 1, blitGLBuffer);

		prevBlitWidth  = blit_width;
		prevBlitHeight = blit_height;
	}

	CGRect	src;

	src = CGRectMake(0, 0, blit_width, blit_height);
	DrawWithCoreImageFilter(src, cgBlitImage);

	if (fullscreen)
		CGLFlushDrawable(glContext);
	else
		aglSwapBuffers(agContext);
#endif
}

static void GLMakeScreenMesh(GLdouble *vertex3D, int meshx, int meshy)
{
	GLdouble *v;
	double   warp;

	v = vertex3D;
	warp = macCurvatureWarp * 0.001;

	for (int y = 0; y < meshy; y++)
	{
		for (int x = 0; x <= meshx; x++)
		{
			double	u1, v1, v2;

			u1 = -1.0 + 2.0 / (double) meshx * (double)  x;
			v1 = -1.0 + 2.0 / (double) meshy * (double)  y;
			v2 = -1.0 + 2.0 / (double) meshy * (double) (y + 1);

			*v++ = u1;
			*v++ = v2;
			*v++ = -1.0 - (u1 * u1 + v2 * v2) * warp;

			*v++ = u1;
			*v++ = v1;
			*v++ = -1.0 - (u1 * u1 + v1 * v1) * warp;
		}
	}
}

static void GLMakeTextureMesh(GLdouble *vertex2D, int meshx, int meshy, double lx, double ly)
{
	GLdouble *v;

	v = vertex2D;

	for (int y = meshy; y > 0; y--)
	{
		for (int x = 0; x <= meshx; x++)
		{
			double	u1, v1, v2;

			u1 = lx / (double) meshx * (double)  x;
			v1 = ly / (double) meshy * (double)  y;
			v2 = ly / (double) meshy * (double) (y - 1);

			*v++ = u1;
			*v++ = v2;

			*v++ = u1;
			*v++ = v1;
		}
	}
}

void S9xTextMode(void)
{
	return;
}

void S9xGraphicsMode(void)
{
	return;
}

void S9xSetPalette(void)
{
	return;
}

void SetInfoDlgColor(unsigned char r, unsigned char g, unsigned char b)
{
	#pragma unused (r, g, b)

	return;
}
