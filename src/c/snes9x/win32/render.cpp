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



/* Render.CPP
 * ----------
 * Video output filters for the Windows port.
 */

#include "../port.h"
#include "wsnes9x.h"
#include "../snes9x.h"
#include <memory.h>
#include "render.h"
#include "directx.h"
#include "../ppu.h"
#if defined(USE_GLIDE) || defined(USE_OPENGL)
#include "../3d.h"
#include "../gfx.h"
#include <math.h>
#include "auxmath.h"
#endif
#include "../gfx.h"

// Private Prototypes, should not be called directly
void RenderPlain (SSurface Src, SSurface Dst, RECT *);
void RenderForced1X (SSurface Src, SSurface Dst, RECT *);
void RenderSimple2X (SSurface Src, SSurface Dst, RECT *);
void RenderFakeTV (SSurface Src, SSurface Dst, RECT *);
void RenderSuperEagle (SSurface Src, SSurface Dst, RECT *);
void Render2xSaI (SSurface Src, SSurface Dst, RECT *);
void RenderSuper2xSaI (SSurface Src, SSurface Dst, RECT *);
void RenderTVMode (SSurface Src, SSurface Dst, RECT *);
template<int T> void RenderHQ2X (SSurface Src, SSurface Dst, RECT *rect);
template<int T> void RenderHQ3X (SSurface Src, SSurface Dst, RECT *rect);
void RenderLQ3XB (SSurface Src, SSurface Dst, RECT *rect);
void RenderEPXA (SSurface Src, SSurface Dst, RECT *);
void RenderEPXB (SSurface Src, SSurface Dst, RECT *);
void RenderEPXC (SSurface Src, SSurface Dst, RECT *);
void RenderEPX3 (SSurface Src, SSurface Dst, RECT *);
void RenderSimple3X (SSurface Src, SSurface Dst, RECT *);
void RenderTVMode3X (SSurface Src, SSurface Dst, RECT *);
void RenderDotMatrix3X (SSurface Src, SSurface Dst, RECT *);
#ifdef USE_GLIDE
void RenderGlide (SSurface Src, SSurface Dst, RECT *);
#endif
#ifdef USE_OPENGL
void RenderOpenGL (SSurface Src, SSurface Dst, RECT *);
#endif
// Contains the pointer to the now active render method
TRenderMethod RenderMethod = RenderPlain;
TRenderMethod RenderMethodHiRes = RenderPlain;

// Used as change log
static uint8 ChangeLog1 [EXT_PITCH * MAX_SNES_HEIGHT];
static uint8 ChangeLog2 [EXT_PITCH * MAX_SNES_HEIGHT];
static uint8 ChangeLog3 [EXT_PITCH * MAX_SNES_HEIGHT];

uint8 *ChangeLog [3] = {
    ChangeLog1, ChangeLog2, ChangeLog3
};

START_EXTERN_C
uint8 snes9x_clear_change_log = 0;
END_EXTERN_C

TRenderMethod FilterToMethod(RenderFilter filterID)
{
	switch(filterID)
	{
        default:
        case FILTER_NONE:       return RenderPlain;
        case FILTER_SIMPLE1X:   return RenderForced1X;
        case FILTER_SIMPLE2X:   return RenderSimple2X;
        case FILTER_SCANLINES:  return RenderFakeTV;
        case FILTER_TVMODE:     return RenderTVMode;
        case FILTER_SUPEREAGLE: return RenderSuperEagle;
        case FILTER_SUPER2XSAI: return RenderSuper2xSaI;
        case FILTER_2XSAI:      return Render2xSaI;
        case FILTER_HQ2X:       return RenderHQ2X<FILTER_HQ2X>;
        case FILTER_HQ2XS:      return RenderHQ2X<FILTER_HQ2XS>;
        case FILTER_HQ2XBOLD:   return RenderHQ2X<FILTER_HQ2XBOLD>;
        case FILTER_EPXA:       return RenderEPXA;
        case FILTER_EPXB:       return RenderEPXB;
        case FILTER_EPXC:       return RenderEPXC;
        case FILTER_SIMPLE3X:   return RenderSimple3X;
        case FILTER_TVMODE3X:   return RenderTVMode3X;
        case FILTER_DOTMATRIX3X:return RenderDotMatrix3X;
        case FILTER_HQ3X:       return RenderHQ3X<FILTER_HQ3X>;
        case FILTER_HQ3XS:      return RenderHQ3X<FILTER_HQ3XS>;
        case FILTER_HQ3XBOLD:   return RenderHQ3X<FILTER_HQ3XBOLD>;
        case FILTER_LQ3XBOLD:   return RenderLQ3XB;
        case FILTER_EPX3:       return RenderEPX3;
#ifdef USE_GLIDE
        case FILTER_GLIDE:      return RenderGlide;
#endif
#ifdef USE_OPENGL
        case FILTER_OPENGL:     return RenderOpenGL;
#endif
	}
}

const char* GetFilterName(RenderFilter filterID)
{
	switch(filterID)
	{
		default: return "Unknown";
		case FILTER_NONE: return "None";
		case FILTER_SIMPLE1X: return "Forced 1X";
		case FILTER_SIMPLE2X: return "Simple 2X";
		case FILTER_SCANLINES: return "Scanlines";
		case FILTER_TVMODE: return "TV Mode";
		case FILTER_SUPEREAGLE: return "SuperEagle";
		case FILTER_SUPER2XSAI: return "Super2xSaI";
		case FILTER_2XSAI: return "2xSaI";
		case FILTER_HQ2X: return "hq2x";
		case FILTER_HQ2XS: return "hq2xS";
		case FILTER_HQ2XBOLD: return "hq2xBold";
		case FILTER_EPXA: return "EPX A";
		case FILTER_EPXB: return "EPX B";
		case FILTER_EPXC: return "EPX C";
		case FILTER_SIMPLE3X: return "Simple 3X";
		case FILTER_TVMODE3X: return "TV Mode 3X";
		case FILTER_DOTMATRIX3X: return "Dot Matrix 3X";
		case FILTER_HQ3X: return "hq3x";
		case FILTER_HQ3XS: return "hq3xS";
		case FILTER_HQ3XBOLD: return "hq3xBold";
		case FILTER_LQ3XBOLD: return "lq3xBold";
		case FILTER_EPX3: return "EPX3";
	#ifdef USE_GLIDE
		case FILTER_GLIDE: return "3D/FX bi-linear";
	#endif
	#ifdef USE_OPENGL
		case FILTER_OPENGL: return "OpenGL bi-linear";
	#endif
	}
}

int GetFilterScale(RenderFilter filterID)
{
	switch(filterID)
	{
		case FILTER_NONE:
		case FILTER_SIMPLE1X:
	#ifdef USE_GLIDE
		case FILTER_GLIDE:
	#endif
	#ifdef USE_OPENGL
		case FILTER_OPENGL:
	#endif
			return 1;

		default:
			return 2;

		case FILTER_SIMPLE3X:
		case FILTER_TVMODE3X:
		case FILTER_DOTMATRIX3X:
		case FILTER_HQ3X:
		case FILTER_HQ3XS:
		case FILTER_HQ3XBOLD:
		case FILTER_LQ3XBOLD:
		case FILTER_EPX3:
			return 3;
	}
}

bool GetFilterHiResSupport(RenderFilter filterID)
{
	switch(filterID)
	{
		case FILTER_NONE:
		case FILTER_SIMPLE1X:
		case FILTER_SIMPLE2X:
		case FILTER_SCANLINES:
		case FILTER_TVMODE:
		case FILTER_SIMPLE3X:
	#ifdef USE_GLIDE
		case FILTER_GLIDE:
	#endif
	#ifdef USE_OPENGL
		case FILTER_OPENGL:
	#endif
			return true;

		default:
			return false;
	}
}

inline static bool GetFilter32BitSupport(RenderFilter filterID)
{
	switch(filterID)
	{
		case FILTER_NONE:
		case FILTER_SIMPLE1X:
		case FILTER_SIMPLE2X:
		case FILTER_SIMPLE3X:
		case FILTER_EPXA:
		case FILTER_EPXB:
		case FILTER_EPXC:
		case FILTER_EPX3:
		case FILTER_SCANLINES:
		case FILTER_TVMODE3X:
		case FILTER_DOTMATRIX3X:
			return true;

		default:
			return false;
	}
}

void SelectRenderMethod()
{
    TRenderMethod OldRenderMethod = RenderMethod;
    TRenderMethod OldRenderMethodHiRes = RenderMethodHiRes;

	RenderMethod = FilterToMethod(GUI.Scale);
	RenderMethodHiRes = FilterToMethod(GUI.ScaleHiRes);

	if (GUI.ScreenCleared || OldRenderMethod != RenderMethod || OldRenderMethodHiRes != RenderMethodHiRes)
        snes9x_clear_change_log = GUI.NumFlipFrames;

    GUI.ScreenCleared = false;

	GUI.DepthConverted = !GUI.NeedDepthConvert;
	if(GUI.ScreenDepth == 32 &&
	   ((GetFilter32BitSupport(GUI.Scale)      && (IPPU.RenderedScreenHeight <= SNES_HEIGHT_EXTENDED && IPPU.RenderedScreenWidth < 512)) ||
	    (GetFilter32BitSupport(GUI.ScaleHiRes) && (IPPU.RenderedScreenHeight > SNES_HEIGHT_EXTENDED || IPPU.RenderedScreenWidth == 512))))
	{
		// filter supports converting
		GUI.DepthConverted = true;
	}
}


// Needed by Render2xSaI
void SuperEagle(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
void _2xSaI(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
void Super2xSaI(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
bool8 mmx_checked = false;
bool8 mmx_enabled = true;
extern bool8 cpu_mmx;


#define R5G6B5 // windows port uses RGB565

#ifdef R5G6B5
	#define	Mask_2	0x07E0	// 00000 111111 00000
	#define	Mask13	0xF81F	// 11111 000000 11111
	#define	Mask_1	0x001F	// 00000 000000 11111
	#define	Mask_3	0xF800	// 11111 000000 00000
	#define CONVERT_16_TO_32(pixel) \
        (((((pixel) >> 11)        ) << /*RedShift+3*/  19) | \
         ((((pixel) >> 6)   & 0x1f) << /*GreenShift+3*/11) | \
          (((pixel)         & 0x1f) << /*BlueShift+3*/ 3))
	#define NUMBITS (16)
#else
	#define	Mask_2	0x03E0	// 00000 11111 00000
	#define	Mask13	0x7C1F	// 11111 00000 11111
	#define	Mask_1	0x001F	// 00000 00000 11111
	#define	Mask_3	0x7C00	// 11111 00000 00000
	#define CONVERT_16_TO_32(pixel) \
        (((((pixel) >> 10)        ) << /*RedShift+3*/  19) | \
         ((((pixel) >> 5)   & 0x1f) << /*GreenShift+3*/11) | \
          (((pixel)         & 0x1f) << /*BlueShift+3*/ 3))
	#define NUMBITS (15)
#endif

static int	RGBtoYUV[1<<NUMBITS];
static uint16 RGBtoBright[1<<NUMBITS];

#define Interp05(c1, c2) \
	(((c1) == (c2)) ? (c1) : \
	((((((c1) & Mask_2)      + ((c2) & Mask_2)) >> 1) & Mask_2) + \
	 (((((c1) & Mask13)      + ((c2) & Mask13)) >> 1) & Mask13)))
#define Interp01(c1, c2) \
	((((c1) == (c2)) ? (c1) : \
	(((((((c1) & Mask_2) *  3) + ((c2) & Mask_2)) >> 2) & Mask_2) + \
	 ((((((c1) & Mask13) *  3) + ((c2) & Mask13)) >> 2) & Mask13))))
#define Halve(c1) \
	((((((c1) & Mask_2)) >> 1) & Mask_2) + \
	 (((((c1) & Mask13)) >> 1) & Mask13))
#define ThreeQuarters(c1) \
	((((((c1) & Mask_2)*3) >> 2) & Mask_2) + \
	 (((((c1) & Mask13)*3) >> 2) & Mask13))

#ifdef LSB_FIRST
	#define TWO_PIX(left,right) ((left) | ((right) << 16))
	#define THREE_PIX(left,middle,right) uint48((left) | ((middle) << 16), (right))
	#define TWO_PIX_32(left,right) (CONVERT_16_TO_32(left) | ((uint64)CONVERT_16_TO_32(right) << 32))
	#define THREE_PIX_32(left,middle,right) uint96(CONVERT_16_TO_32(left) | ((uint64)CONVERT_16_TO_32(middle) << 32), CONVERT_16_TO_32(right))
#else
	#define TWO_PIX(left,right) ((right) | ((left) << 16))
	#define THREE_PIX(left,middle,right) uint48((middle) | ((left) << 16), (right)) // is this right?
//	#define THREE_PIX(left,middle,right) uint48((right) | ((middle) << 16), (left)) // or this?
#endif



// stretches a single line
inline void DoubleLine16( uint16 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		*lpDst++ = *lpSrc;
		*lpDst++ = *lpSrc++;
    }
}
inline void TripleLine16( uint16 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		*lpDst++ = *lpSrc;
		*lpDst++ = *lpSrc;
		*lpDst++ = *lpSrc++;
    }
}
inline void ThreeHalfLine16( uint16 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width-=2){
		*lpDst++ = *lpSrc;
		*lpDst++ = Interp05(*lpSrc, *(lpSrc+1));
		*lpDst++ = *(lpSrc+1);
		lpSrc+=2;
    }
}
inline void HalfLine16( uint16 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		*lpDst++ = Interp05(*lpSrc, *(lpSrc+1));
		lpSrc+=2;
    }
}
inline void DoubleLine32( uint32 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		lpSrc++;
    }
}
inline void SingleLine32( uint32 *lpDst, uint16 *lpSrc, unsigned int Width){
	while(Width--){
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		lpSrc++;
	}
}
inline void HalfLine32( uint32 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		const uint16 color = Interp05(*lpSrc, *(lpSrc+1));
		*lpDst++ = CONVERT_16_TO_32(color);
		lpSrc+=2;
    }
}
inline void TripleLine32( uint32 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width--){
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		lpSrc++;
    }
}
inline void ThreeHalfLine32( uint32 *lpDst, uint16 *lpSrc, unsigned int Width){
    while(Width-=2){
		*lpDst++ = CONVERT_16_TO_32(*lpSrc);
		const uint16 color = Interp05(*lpSrc, *(lpSrc+1));
		*lpDst++ = CONVERT_16_TO_32(color);
		*lpDst++ = CONVERT_16_TO_32(*(lpSrc+1));
		lpSrc+=2;
    }
}


inline void SetRect(RECT* rect, int width, int height, int scale)
{
	rect->left = 0;
	rect->right = width * scale;
	rect->top = 0;
	rect->bottom = (height - (GUI.HeightExtend?0:15)) * scale;
}


// No enlargement, just render to the screen
void RenderPlain (SSurface Src, SSurface Dst, RECT *rect)
{
	SetRect(rect, Src.Width, Src.Height, 1);
	if(Src.Height > SNES_HEIGHT_EXTENDED)
		rect->bottom -= (GUI.HeightExtend?0:15);
	const uint32 srcHeight = (rect->bottom - rect->top)/1;

	uint16 *lpSrc = reinterpret_cast<uint16 *>(Src.Surface);
	const unsigned int srcPitch = Src.Pitch >> 1;

	if(GUI.ScreenDepth == 16)
	{
		const unsigned int dstPitch = Dst.Pitch >> 1;
		uint16 *lpDst = reinterpret_cast<uint16 *>(Dst.Surface) + rect->top * dstPitch + rect->left;

		for (unsigned int H = 0; H != srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
			memcpy (lpDst, lpSrc, Src.Width << 1);
	}
	else if(GUI.ScreenDepth == 32)
	{
		const unsigned int dstPitch = Dst.Pitch >> 2;
		uint32 *lpDst = reinterpret_cast<uint32 *>(Dst.Surface) + rect->top * dstPitch + rect->left;

		for (unsigned int H = 0; H != srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
			SingleLine32(lpDst, lpSrc, Src.Width);
	}
}

void RenderForced1X( SSurface Src, SSurface Dst, RECT *rect)
{
    uint16 *lpSrc;
    unsigned int H;

	SetRect(rect, 256, 239, 1);
	const uint32 srcHeight = (rect->bottom - rect->top)/1;

	const unsigned int srcPitch = Src.Pitch >> 1;
    lpSrc = reinterpret_cast<uint16 *>(Src.Surface);

	if(GUI.ScreenDepth == 16)
	{
		const unsigned int dstPitch = Dst.Pitch >> 1;
		uint16 *lpDst = reinterpret_cast<uint16 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H != srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
					memcpy (lpDst, lpSrc, Src.Width << 1);
			else
				for (H = 0; H < srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
					HalfLine16 (lpDst, lpSrc, Src.Width);
		else
			if(Src.Width != 512)
				for (H = 0; H != Src.Height; H+=2, lpDst += dstPitch, lpSrc += srcPitch<<1)
					memcpy (lpDst, lpSrc, Src.Width << 1);
			else
				for (H = 0; H < Src.Height >> 1; H++, lpDst += dstPitch, lpSrc += srcPitch<<1)
					HalfLine16 (lpDst, lpSrc, Src.Width);
	}
	else if(GUI.ScreenDepth == 32)
	{
		const unsigned int dstPitch = Dst.Pitch >> 2;
		uint32 *lpDst = reinterpret_cast<uint32 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H != srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
					SingleLine32 (lpDst, lpSrc, Src.Width);
			else
				for (H = 0; H < srcHeight; H++, lpDst += dstPitch, lpSrc += srcPitch)
					HalfLine32 (lpDst, lpSrc, Src.Width);
		else
			if(Src.Width != 512)
				for (H = 0; H != Src.Height; H+=2, lpDst += dstPitch, lpSrc += srcPitch<<1)
					SingleLine32 (lpDst, lpSrc, Src.Width);
			else
				for (H = 0; H < Src.Height >> 1; H++, lpDst += dstPitch, lpSrc += srcPitch<<1)
					HalfLine32 (lpDst, lpSrc, Src.Width);
	}
}

// Enlarge 2x using Fake TV mode when the Snes9x is not rendering in HiRes
// FakeTV mode = a black line between each other line (scanlines)
void RenderFakeTV( SSurface Src, SSurface Dst, RECT *rect)
{
    uint16 *lpSrc;
    unsigned int H;

	SetRect(rect, 256, 239, 2);
	const uint32 srcHeight = (rect->bottom - rect->top)/2;

	const unsigned int srcPitch = Src.Pitch >> 1;
    lpSrc = reinterpret_cast<uint16 *>(Src.Surface);

	if(GUI.ScreenDepth == 16)
	{
		const unsigned int dstPitch = Dst.Pitch >> 1;
		uint16 *lpDst = reinterpret_cast<uint16 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if(Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					DoubleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*2), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					memcpy (lpDst, lpSrc, Src.Width << 1), lpDst += dstPitch,
					memset (lpDst, 0, 512*2), lpDst += dstPitch;
		else
			if(Src.Width != 512)
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch << 1)
					DoubleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*2), lpDst += dstPitch;
			else
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch << 1)
					memcpy (lpDst, lpSrc, Src.Width << 1), lpDst += dstPitch,
					memset (lpDst, 0, 512*2), lpDst += dstPitch;
	}
	else if(GUI.ScreenDepth == 32)
	{
		const unsigned int dstPitch = Dst.Pitch >> 2;
		uint32 *lpDst = reinterpret_cast<uint32 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if(Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					DoubleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*4), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					SingleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*4), lpDst += dstPitch;
		else
			if(Src.Width != 512)
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch << 1)
					DoubleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*4), lpDst += dstPitch;
			else
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch << 1)
					SingleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					memset (lpDst, 0, 512*4), lpDst += dstPitch;
	}
}

void RenderSimple2X( SSurface Src, SSurface Dst, RECT *rect)
{
	// just copy if it's high res in both dimensions
    if(Src.Height > SNES_HEIGHT_EXTENDED && Src.Width == 512)
	    {RenderPlain (Src, Dst, rect); return;}

    uint16 *lpSrc;
    unsigned int H;

	SetRect(rect, 256, 239, 2);
	const uint32 srcHeight = (rect->bottom - rect->top)/2;

	const unsigned int srcPitch = Src.Pitch >> 1;
    lpSrc = reinterpret_cast<uint16 *>(Src.Surface);

	if(GUI.ScreenDepth == 16)
	{
		const unsigned int dstPitch = Dst.Pitch >> 1;
		uint16 *lpDst = reinterpret_cast<uint16 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					DoubleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					DoubleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					memcpy (lpDst, lpSrc, Src.Width << 1), lpDst += dstPitch,
					memcpy (lpDst, lpSrc, Src.Width << 1), lpDst += dstPitch;
		else
			for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
				DoubleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
	}
	else if(GUI.ScreenDepth == 32)
	{
		const unsigned int dstPitch = Dst.Pitch >> 2;
		uint32 *lpDst = reinterpret_cast<uint32 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					DoubleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					DoubleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					SingleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					SingleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
		else
			for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
				DoubleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
	}
}

// As the name suggests, I (Jerremy) took this procedure from the MAME32, I hope that
// Michael Soderstrom and Chris Kirmse don't mind this.
static bool MAME32_DetectMMX()
{
#if defined(_M_IX86)

    bool result = false;
    int cpu_properties;

#ifdef __MINGW32__
	__asm__ __volatile__
	(
		"movl $1, %%eax\n\t"
		"cpuid\n\t"
		"andl $0x800000, %%edx\n\t"
		"movl %%edx, %0"
		: "=q" (cpu_properties)
		:
	);

	if (cpu_properties)
		result = true;

	return result;

#else

#define ASM_CPUID __asm _emit 0xf __asm _emit 0xa2

    __try
    {
        __asm
        {
            mov eax, 1
            ASM_CPUID
            mov cpu_properties, edx
        }
        /* if bit 23 is set, MMX is available */
        if (cpu_properties & 0x800000)
            result = true;
    }
    __except(1)
    {
        return false;
    }
#endif

    return result;

#else
    return false;
#endif
}

void RenderSuperEagle (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple2X (Src, Dst, rect); return;}

    unsigned char *lpSrc, *lpDst;

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if (mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

	SetRect(rect, Src.Width, Src.Height, 2);

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    SuperEagle (lpSrc, Src.Pitch,
                ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames],
                lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void Render2xSaI (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {
        RenderSimple2X (Src, Dst, rect);
        return;
    }

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if( mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

	SetRect(rect, Src.Width, Src.Height, 2);

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    _2xSaI (lpSrc, Src.Pitch, ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames],
            lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void RenderSuper2xSaI (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple2X (Src, Dst, rect); return;}

    unsigned char *lpSrc, *lpDst;

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if( mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

	SetRect(rect, Src.Width, Src.Height, 2);

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    Super2xSaI (lpSrc, Src.Pitch,
                ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames],
                lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void RenderTVMode ( SSurface Src, SSurface Dst, RECT *rect)
{
	// XXX: this filter's hi-res support for double-height modes is NYI
    if (Src.Height > SNES_HEIGHT_EXTENDED)
    {
        snes9x_clear_change_log = GUI.NumFlipFrames;
        RenderSimple2X (Src, Dst, rect);
        return;
    }

    uint8 *nextLine, *finish;
    uint32 colorMask = ~(RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 16));
    uint32 lowPixelMask = RGB_LOW_BITS_MASK;
    uint8 *srcPtr = Src.Surface;
    uint8 *dstPtr = Dst.Surface;
    uint32 srcPitch = Src.Pitch;
    uint32 dstPitch = Dst.Pitch;
    int width = Src.Width;
    int height = Src.Height;
    uint8 *deltaPtr = ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames];

	SetRect(rect, 256, height, 2);

    dstPtr += rect->top * Dst.Pitch + rect->left * 2;
    nextLine = dstPtr + dstPitch;

    if (width == 256)
    {
		do
		{
			uint32 *bP = (uint32 *) srcPtr;
			uint32 *xP = (uint32 *) deltaPtr;
			uint32 *dP = (uint32 *) dstPtr;
			uint32 *nL = (uint32 *) nextLine;
			uint32 currentPixel;
			uint32 nextPixel;
			uint32 currentDelta;
			uint32 nextDelta;

			finish = (uint8 *) bP + ((width + 2) << 1);
			nextPixel = *bP++;
			nextDelta = *xP++;

			do
			{
				currentPixel = nextPixel;
				currentDelta = nextDelta;
				nextPixel = *bP++;
				nextDelta = *xP++;

				if (snes9x_clear_change_log ||
							nextPixel != nextDelta || currentPixel != currentDelta)
				{
					uint32 colorA, colorB, product, darkened;

					*(xP - 2) = currentPixel;
		#ifdef LSB_FIRST
					colorA = currentPixel & 0xffff;
					colorB = (currentPixel & 0xffff0000) >> 16;
		#else
					colorA = (currentPixel & 0xffff0000) >> 16;
					colorB = currentPixel & 0xffff;
		#endif

					*(dP) = product = TWO_PIX(colorA, (((colorA & colorMask) >> 1) +
											           ((colorB & colorMask) >> 1) +
											            (colorA & colorB & lowPixelMask)));

					darkened  = (product = ((product & colorMask) >> 1));
					darkened += (product = ((product & colorMask) >> 1));
					darkened += (product & colorMask) >> 1;
					*(nL) = darkened;

		#ifdef LSB_FIRST
					colorA = nextPixel & 0xffff;
		#else
					colorA = (nextPixel & 0xffff0000) >> 16;
		#endif
					*(dP + 1) = product = TWO_PIX(colorB, (((colorA & colorMask) >> 1) +
												           ((colorB & colorMask) >> 1) +
												            (colorA & colorB & lowPixelMask)));
					darkened  = (product = ((product & colorMask) >> 1));
					darkened += (product = ((product & colorMask) >> 1));
					darkened += (product & colorMask) >> 1;
					*(nL + 1) = darkened;
				}

				dP += 2;
				nL += 2;
			} while ((uint8 *) bP < finish);

			deltaPtr += srcPitch;
			srcPtr += srcPitch;
			dstPtr += dstPitch * 2;
			nextLine += dstPitch * 2;
		}
		while (--height);

        if (snes9x_clear_change_log)
            snes9x_clear_change_log--;
    }
    else
    {
        snes9x_clear_change_log = GUI.NumFlipFrames;

		do
		{
			uint32 *bP = (uint32 *) srcPtr;
			uint32 *xP = (uint32 *) deltaPtr;
			uint32 *dP = (uint32 *) dstPtr;
			uint32 *nL = (uint32 *) nextLine;
			uint32 currentPixel;
			uint32 nextPixel;
			uint32 currentDelta;
			uint32 nextDelta;

			finish = (uint8 *) bP + ((width + 2) << 1);
			nextPixel = *bP++;
			nextDelta = *xP++;

			do
			{
				currentPixel = nextPixel;
				currentDelta = nextDelta;
				nextPixel = *bP++;
				nextDelta = *xP++;

				if (snes9x_clear_change_log ||
							nextPixel != nextDelta || currentPixel != currentDelta)
				{
					uint32 colorA, colorB, product, darkened;

					*(xP - 1) = currentPixel;
		#ifdef LSB_FIRST
					colorA = currentPixel & 0xffff;
					colorB = (currentPixel & 0xffff0000) >> 16;
		#else
					colorA = (currentPixel & 0xffff0000) >> 16;
					colorB = currentPixel & 0xffff;
		#endif
					product = TWO_PIX(colorA, (((colorA & colorMask) >> 1) +
											((colorB & colorMask) >> 1) +
												(colorA & colorB & lowPixelMask)));

					*(dP) = currentPixel;
					darkened  = (product = ((product & colorMask) >> 1));
					darkened += (product = ((product & colorMask) >> 1));
					darkened += (product & colorMask) >> 1;
					*(nL) = darkened;
				}

				dP += 1;
				nL += 1;
			} while ((uint8 *) bP < finish);

			deltaPtr += srcPitch;
			srcPtr += srcPitch;
			dstPtr += dstPitch * 2;
			nextLine += dstPitch * 2;
		}
		while (--height);
    }
}


void RenderSimple3X( SSurface Src, SSurface Dst, RECT *rect)
{
    uint16 *lpSrc;
    unsigned int H;

	SetRect(rect, 256, 239, 3);
	const uint32 srcHeight = (rect->bottom - rect->top)/3;

	const unsigned int srcPitch = Src.Pitch >> 1;
    lpSrc = reinterpret_cast<uint16 *>(Src.Surface);

	if(GUI.ScreenDepth == 16)
	{
		const unsigned int dstPitch = Dst.Pitch >> 1;
		uint16 *lpDst = reinterpret_cast<uint16 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
		else
			if(Src.Width != 512)
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch, lpSrc += srcPitch,
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch, lpSrc += srcPitch,
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine16 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
	}
	else if(GUI.ScreenDepth == 32)
	{
		const unsigned int dstPitch = Dst.Pitch >> 2;
		uint32 *lpDst = reinterpret_cast<uint32 *>(Dst.Surface) + rect->top * dstPitch + rect->left;
		if (Src.Height <= SNES_HEIGHT_EXTENDED)
			if(Src.Width != 512)
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < srcHeight; H++, lpSrc += srcPitch)
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
		else
			if(Src.Width != 512)
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch, lpSrc += srcPitch,
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					TripleLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
			else
				for (H = 0; H < Src.Height >> 1; H++, lpSrc += srcPitch)
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch, lpSrc += srcPitch,
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch,
					ThreeHalfLine32 (lpDst, lpSrc, Src.Width), lpDst += dstPitch;
	}
}



#ifdef LSB_FIRST
#pragma pack(push, 1)
struct uint48
{
	uint32 L;
	uint16 R;
	inline uint48(uint32 left, uint16 right)
	{
		L = left;
		R = right;
	}
};
struct uint96
{
	uint64 L;
	uint32 R;
	inline uint96(uint64 left, uint32 right)
	{
		L = left;
		R = right;
	}
};
#pragma pack(pop)
#else
#error NYI for !LSB_FIRST
#endif

// assert that sizeof(uint48) == 6 bytes
//static int compiler_error_sizeof_uint48_notequal_6  [(sizeof(uint48)== 6)?1:0] = {0};
//static int compiler_error_sizeof_uint96_notequal_12 [(sizeof(uint96)==12)?1:0] = {0};


// shared EPX-type loop macros
// All of these parameters will be constant values,
// so I hope the compiler is smart enough to optimize away "if(0) ..."

#define DrawRows(scale,diags)            \
	{                                    \
		h = srcHeight - 1;               \
		DoRow(0,1,scale,diags);          \
		for (h = srcHeight - 2; h; h--)  \
			DoRow(1,1,scale,diags);      \
		DoRow(1,0,scale,diags);          \
	}

#define DoRow(topValid,botValid,scale,diags)              \
	{                                                     \
		w = srcWidth - 1;                                 \
		InitLine(topValid,botValid,scale,diags);          \
		DrawPix(0,topValid,0,botValid);                   \
		for (w = srcWidth - 2; w; w--)                    \
		{                                                 \
			NextPixel(topValid,botValid,1,diags);         \
			DrawPix(topValid,topValid,botValid,botValid); \
		}                                                 \
		NextPixel(topValid,botValid,0,diags);             \
		DrawPix(topValid,0,botValid,0);                   \
		srcPtr += srcPitch;                               \
		dstPtr += dstPitch * scale;                       \
	}

#define InitLine(topValid, botValid, scale, diags)                \
	{                                                             \
		if(topValid) uP  = (uint16 *) (srcPtr - srcPitch);        \
		if(botValid) lP  = (uint16 *) (srcPtr + srcPitch);        \
					 sP  = (uint16 *) srcPtr;                     \
					 dP1 = (destType *) dstPtr;                   \
					 dP2 = (destType *) (dstPtr + dstPitch);      \
		if(scale>=3) dP3 = (destType *) (dstPtr + (dstPitch<<1)); \
		if(topValid) colorD = *uP++;                              \
		if(botValid) colorB = *lP++;                              \
					colorX = *sP++;                               \
					colorC = *sP;                                 \
		if(diags) if(topValid) colorH = *uP;                      \
		if(diags) if(botValid) colorG = *lP;                      \
	}

#define NextPixel(topValid, botValid, rightValid, diags)      \
	{                                                         \
		colorA = colorX;                                      \
		colorX = colorC;                                      \
		if(rightValid) colorC = *++sP;                        \
		if(diags) {                                           \
			if(botValid){                                     \
				colorF = colorB;                              \
				colorB = colorG;                              \
				if(rightValid) colorG = *++lP;                \
			}                                                 \
			if(topValid){                                     \
				colorE = colorD;                              \
				colorD = colorH;                              \
				if(rightValid) colorH = *++uP;                \
			}                                                 \
		} else {                                              \
			if(botValid) colorB = *lP++;                      \
			if(topValid) colorD = *uP++;                      \
		}                                                     \
	}

#define DrawInit(scale,uintDest)                                                            \
	uint8 *srcPtr = Src.Surface, *dstPtr = Dst.Surface;                                     \
	const uint32 srcPitch = Src.Pitch, dstPitch = Dst.Pitch;                                \
	SetRect(rect, 256, Src.Height, scale);                                                  \
	dstPtr += rect->top * Dst.Pitch + rect->left * 2;                                       \
	const uint32 srcHeight = (rect->bottom - rect->top)/scale;                              \
	const uint32 srcWidth = (rect->right - rect->left)/scale;                               \
	uint16	colorX, colorA, colorB, colorC, colorD, colorE=0, colorF=0, colorG=0, colorH=0; \
	uint16	*sP, *uP, *lP;                                                                  \
	typedef uintDest destType;                                                              \
	destType *dP1, *dP2, *dP3=0;                                                            \
	int		w, h;


// code for rendering the image enlarged 2X with EPX
void RenderEPXA (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple2X (Src, Dst, rect); return;}

	// all we have to do now is define what to do at each pixel
	// (it's a 2x filter, so for each pixel we draw 2x2 = 4 pixels)
	// based on the current pixel and the surrounding 4 pixels

	//   D
	// A X C
	//   B
	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */                     \
	{                                    /* on10 on11 */                     \
		if ((((on00||on10)?colorA:colorX) != ((on01||on11)?colorC:colorX))   \
		 && (((on10||on11)?colorB:colorX) != ((on00||on01)?colorD:colorX)))  \
		{                                                                    \
			*dP1++ = _TWO_PIX((on00 && colorD == colorA) ? colorD : colorX,    /* we set 2 pixels at a time (the pointer size is the size of (scale=2) pixels)*/\
						      (on01 && colorC == colorD) ? colorC : colorX); \
			*dP2++ = _TWO_PIX((on10 && colorA == colorB) ? colorA : colorX,    /* also set the 2 pixels below those*/\
						      (on11 && colorB == colorC) ? colorB : colorX); \
		} else {                                                             \
			*dP1++ = _TWO_PIX(colorX, colorX);                               \
			*dP2++ = _TWO_PIX(colorX, colorX);                               \
		}                                                                    \
	}

	// this renderer supports 32-bit or 16-bit rendering (see GetFilter32BitSupport())
	// so we must switch based on depth to handle both modes.
	// we could choose to only support 16-bit, but that would impose a performance penalty
	// for color conversion from the increasingly-commonly-used 32-bit depth
	if(GUI.ScreenDepth == 32)
	{
#define _TWO_PIX TWO_PIX_32 // define _TWO_PIX to combine two 32-bit pixels at a time
		DrawInit(2,uint64); // initialize with the scale and an int as big as 2 pixels
		DrawRows(2,0); // scale is 2x, and diags (8 surrounding pixels) is off since we only need 4 surrounding
#undef _TWO_PIX
	}
	else // 16-bit, same deal but with smaller pointer size and different 2-pixel combiner
	{
#define _TWO_PIX TWO_PIX // define _TWO_PIX to combine two 16-bit pixels at a time
		DrawInit(2,uint32); // initialize with the scale and an int as big as 2 pixels
		DrawRows(2,0); // scale is 2x, and diags (8 surrounding pixels) is off since we only need 4 surrounding
#undef _TWO_PIX
	}


	#undef DrawPix
}

// code for improved 2X EPX, which tends to do better with diagonal edges than regular EPX
void RenderEPXB (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple2X (Src, Dst, rect); return;}

	// E D H
	// A X C
	// F B G
	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */                                                                                                       \
	{                                    /* on10 on11 */                                                                                                       \
		if ((((on00||on10)?colorA:colorX) != ((on01||on11)?colorC:colorX))                                                                                     \
		&& (((on10||on11)?colorB:colorX) != ((on00||on01)?colorD:colorX))                                                                                      \
		&& ((!on00||!on01||!on10|!on11) ||                                                                                                                     \
			((colorX == colorA) || (colorX == colorB) || (colorX == colorC) || (colorX == colorD) || /* diagonal    */                                         \
			 (((colorE != colorG) || (colorX == colorF) || (colorX == colorH)) &&                    /*  edge       */                                         \
			  ((colorF != colorH) || (colorX == colorE) || (colorX == colorG))))))                   /*   smoothing */                                         \
		{                                                                                                                                                      \
			*dP1++ = _TWO_PIX((on00 && colorD == colorA && (colorX != colorE || colorX != colorG || colorD != colorH || colorA != colorF)) ? colorD : colorX,  \
							  (on01 && colorC == colorD && (colorX != colorH || colorX != colorF || colorC != colorG || colorD != colorE)) ? colorC : colorX); \
			*dP2++ = _TWO_PIX((on10 && colorA == colorB && (colorX != colorF || colorX != colorH || colorA != colorE || colorB != colorG)) ? colorA : colorX,  \
							  (on11 && colorB == colorC && (colorX != colorG || colorX != colorE || colorB != colorF || colorC != colorH)) ? colorB : colorX); \
		} else {                                                                                                                                               \
			*dP1++ = _TWO_PIX(colorX, colorX);                                                                                                                 \
			*dP2++ = _TWO_PIX(colorX, colorX);                                                                                                                 \
		}                                                                                                                                                      \
	}

	// again, this supports 32-bit or 16-bit rendering
	if(GUI.ScreenDepth == 32)
	{
#define _TWO_PIX TWO_PIX_32
		DrawInit(2,uint64);
		DrawRows(2,1); // 2x scale, and diags is on since we do use all 8 surrounding pixels
#undef _TWO_PIX
	}
	else
	{
#define _TWO_PIX TWO_PIX
		DrawInit(2,uint32);
		DrawRows(2,1); // 2x scale, and diags is on since we do use all 8 surrounding pixels
#undef _TWO_PIX
	}

	#undef DrawPix
}

void RenderEPX3 (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple3X (Src, Dst, rect); return;}

	// E D H
	// A X C
	// F B G

	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */ \
	{                                    /* on10 on11 */ \
		if ((((on00||on10)?colorA:colorX) != ((on01||on11)?colorC:colorX)) \
		 && (((on10||on11)?colorB:colorX) != ((on00||on01)?colorD:colorX))) \
		{ \
			const bool XnE = colorX != colorE; \
			const bool XnF = colorX != colorF; \
			const bool XnG = colorX != colorG; \
			const bool XnH = colorX != colorH; \
			const bool DA = on00 && colorD == colorA && (XnE || XnG || colorD != colorH || colorA != colorF); \
			const bool AB = on10 && colorA == colorB && (XnF || XnH || colorA != colorE || colorB != colorG); \
			const bool BC = on11 && colorB == colorC && (XnG || XnE || colorB != colorF || colorC != colorH); \
			const bool CD = on01 && colorC == colorD && (XnH || XnF || colorC != colorG || colorD != colorE); \
			if (!on00||!on01||!on10||!on11 || ((colorA != colorC) && (colorB != colorD) && \
				((colorX == colorA) || (colorX==colorB) || (colorX==colorC) || (colorX==colorD) || (colorX==colorE) || (colorX==colorF) || (colorX==colorG) || (colorX==colorH)))) \
			{ \
				*dP1++ = _THREE_PIX(on00 && DA ? colorA : colorX, on00&&on01 && ((CD && XnE) || (DA && XnH)) ? colorD : colorX, on01 && CD ? colorC : colorX); \
				*dP2++ = _THREE_PIX(on00&&on10 && ((DA && XnF) || (AB && XnE)) ? colorA : colorX, colorX, on01&&on11 && ((BC && XnH) || (CD && XnG)) ? colorC : colorX); \
				*dP3++ = _THREE_PIX(on10 && AB ? colorA : colorX, on10&&on11 && ((AB && XnG) || (BC && XnF)) ? colorB : colorX, on11 && BC ? colorC : colorX); \
			} else { \
				*dP1++ = _THREE_PIX(on00 && DA && (colorX!=colorB&&colorX!=colorC) ? colorA : colorX, colorX, on01 && CD && (colorX!=colorA&&colorX!=colorB) ? colorC : colorX); \
				*dP2++ = _THREE_PIX(colorX,colorX,colorX); \
				*dP3++ = _THREE_PIX(on10 && AB && (colorX!=colorC&&colorX!=colorD) ? colorA : colorX, colorX, on11 && BC && (colorX!=colorD&&colorX!=colorA) ? colorC : colorX); \
			} \
		} else { \
			*dP1++ = _THREE_PIX(colorX, colorX, colorX); \
			*dP2++ = _THREE_PIX(colorX, colorX, colorX); \
			*dP3++ = _THREE_PIX(colorX, colorX, colorX); \
		} \
	}

	if(GUI.ScreenDepth == 32)
	{
#define _THREE_PIX THREE_PIX_32
		DrawInit(3,uint96); // initialize with the scale and an int as big as 3 pixels
		DrawRows(3,1);
#undef _THREE_PIX
	}
	else
	{
#define _THREE_PIX THREE_PIX
		DrawInit(3,uint48);
		DrawRows(3,1);
#undef _THREE_PIX
	}

	#undef DrawPix
}

#define Interp44(c1, c2, c3, c4) \
	((((((c1) & Mask_2) * 5 + ((c2) & Mask_2) + ((c3) & Mask_2) + ((c4) & Mask_2)) >> 3) & Mask_2) + \
	 (((((c1) & Mask13) * 5 + ((c2) & Mask13) + ((c3) & Mask13) + ((c4) & Mask13)) >> 3) & Mask13))

#define Interp11(c1, c2) \
	((((c1) == (c2)) ? (c1) : \
	(((((((c1) & Mask_2) *  3) + ((c2) & Mask_2) * 5) >> 3) & Mask_2) + \
	 ((((((c1) & Mask13) *  3) + ((c2) & Mask13) * 5) >> 3) & Mask13))))

// EPX3 scaled down to 2X
void RenderEPXC (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple2X (Src, Dst, rect); return;}

	// E D H
	// A X C
	// F B G
	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */ \
	{                                    /* on10 on11 */ \
		if ((((on00||on10)?colorA:colorX) != ((on01||on11)?colorC:colorX)) \
		 && (((on10||on11)?colorB:colorX) != ((on00||on01)?colorD:colorX))) \
		{ \
			const bool XnE = colorX != colorE; \
			const bool XnF = colorX != colorF; \
			const bool XnG = colorX != colorG; \
			const bool XnH = colorX != colorH; \
			const bool DA = on00 && colorD == colorA && (XnE || XnG || colorD != colorH || colorA != colorF); \
			const bool AB = on10 && colorA == colorB && (XnF || XnH || colorA != colorE || colorB != colorG); \
			const bool BC = on11 && colorB == colorC && (XnG || XnE || colorB != colorF || colorC != colorH); \
			const bool CD = on01 && colorC == colorD && (XnH || XnF || colorC != colorG || colorD != colorE); \
			if (!on00||!on01||!on10||!on11 || ((colorA != colorC) && (colorB != colorD) && \
				((colorX == colorA) || (colorX==colorB) || (colorX==colorC) || (colorX==colorD) || (colorX==colorE) || (colorX==colorF) || (colorX==colorG) || (colorX==colorH)))) \
			{ \
				const uint16 colorAA = on00&&on10 && ((DA && XnF) || (AB && XnE)) ? colorA : colorX; \
				const uint16 colorBB = on10&&on11 && ((AB && XnG) || (BC && XnF)) ? colorB : colorX; \
				const uint16 colorCC = on01&&on11 && ((BC && XnH) || (CD && XnG)) ? colorC : colorX; \
				const uint16 colorDD = on00&&on01 && ((CD && XnE) || (DA && XnH)) ? colorD : colorX; \
				*dP1++ = _TWO_PIX(Interp44(on00 && DA ? colorA : colorX, colorDD, colorAA, colorX), Interp44(on01 && CD ? colorC : colorX, colorBB, colorCC, colorX)); \
				*dP2++ = _TWO_PIX(Interp44(on10 && AB ? colorA : colorX, colorAA, colorBB, colorX), Interp44(on11 && BC ? colorC : colorX, colorCC, colorDD, colorX)); \
			} else { \
				*dP1++ = _TWO_PIX(Interp01(colorX, on00 && DA && (colorX!=colorB&&colorX!=colorC) ? colorA : colorX), Interp01(colorX, on01 && CD && (colorX!=colorA&&colorX!=colorB) ? colorC : colorX)); \
				*dP2++ = _TWO_PIX(Interp01(colorX, on10 && AB && (colorX!=colorC&&colorX!=colorD) ? colorA : colorX), Interp01(colorX, on11 && BC && (colorX!=colorD&&colorX!=colorA) ? colorC : colorX)); \
			} \
		} else { \
			*dP1++ = _TWO_PIX(colorX, colorX); \
			*dP2++ = _TWO_PIX(colorX, colorX); \
		} \
	}

	if(GUI.ScreenDepth == 32)
	{
#define _TWO_PIX TWO_PIX_32
		DrawInit(2,uint64);
		DrawRows(2,1); // 2x scale, and diags is on since we do use all 8 surrounding pixels
#undef _TWO_PIX
	}
	else
	{
#define _TWO_PIX TWO_PIX
		DrawInit(2,uint32);
		DrawRows(2,1); // 2x scale, and diags is on since we do use all 8 surrounding pixels
#undef _TWO_PIX
	}

	#undef DrawPix
}

void RenderTVMode3X (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple3X (Src, Dst, rect); return;}

	// E D H
	// A X C
	// F B G

	#define Interp05TQ(c1,c2) ThreeQuarters(Interp05(c1,c2))

	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */ \
	{                                    /* on10 on11 */ \
		if(!on00) {colorE = 0; if(!on01) {colorD = 0; colorX = Halve(colorX); }} \
		if(!on01) {colorH = 0; if(!on11) {colorC = 0; colorX = Halve(colorX); }} \
		if(!on10) {colorF = 0; if(!on00) {colorA = 0; colorX = Halve(colorX); }} \
		if(!on11) {colorG = 0; if(!on10) {colorB = 0; colorX = Halve(colorX); }} \
		if ((colorA == colorC) && (colorB == colorD) && (colorX == colorA) && (colorF == colorG) && (colorE == colorH) && (colorX == colorF) && (colorF == colorE)) \
		{ \
			const uint16 colorX2 = ThreeQuarters(colorX); \
			*dP1++ = _THREE_PIX(colorX, colorX, colorX); \
			*dP2++ = _THREE_PIX(colorX, colorX, colorX); \
			*dP3++ = _THREE_PIX(colorX2, colorX2, colorX2); \
		} \
		else \
		{ \
			*dP1++ = _THREE_PIX(Interp05((colorE&Mask_1)|(colorD&(Mask_3|Mask_2)), (colorA&Mask_1)|(colorX&(Mask_3|Mask_2))),  \
								Interp05(colorD,colorX),                                                                         \
								Interp05((colorD&(Mask_2|Mask_1))|(colorH&Mask_3), (colorX&(Mask_2|Mask_1))|(colorC&Mask_3))); \
			*dP2++ = _THREE_PIX(((colorA&Mask_1)|(colorX&(Mask_3|Mask_2))),  \
								  colorX,                                    \
								((colorX&(Mask_2|Mask_1))|(colorC&Mask_3))); \
			*dP3++ = _THREE_PIX(Interp05TQ((colorA&Mask_1)|(colorX&(Mask_3|Mask_2)), (colorF&Mask_1)|(colorB&(Mask_3|Mask_2))),  \
								Interp05TQ(colorB,colorX),                                                                         \
								Interp05TQ((colorX&(Mask_2|Mask_1))|(colorC&Mask_3), (colorB&(Mask_2|Mask_1))|(colorG&Mask_3))); \
		} \
	}

	if(GUI.ScreenDepth == 32)
	{
#define _THREE_PIX THREE_PIX_32
		DrawInit(3,uint96);
		DrawRows(3,1);
#undef _THREE_PIX
	}
	else
	{
#define _THREE_PIX THREE_PIX
		DrawInit(3,uint48);
		DrawRows(3,1);
#undef _THREE_PIX
	}

	#undef DrawPix
}

void RenderDotMatrix3X (SSurface Src, SSurface Dst, RECT *rect)
{
    if(Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
	    {RenderSimple3X (Src, Dst, rect); return;}

	#define DrawPix(on00,on01,on10,on11) /* on00 on01 */  \
	{                                    /* on10 on11 */  \
		const uint16 colorXA = COLOR_ADD(colorX,colorX);  \
		const uint16 colorXS = COLOR_SUB(colorXA,colorX); \
		const uint16 colorX2 = COLOR_SUB(colorX,colorXS); \
		*dP1++ = _THREE_PIX(colorX2, colorX,  colorX2);   \
		*dP2++ = _THREE_PIX(colorX,  colorXA, colorX);    \
		*dP3++ = _THREE_PIX(colorX2, colorX,  colorX2);   \
	}

	if(GUI.ScreenDepth == 32)
	{
#define _THREE_PIX THREE_PIX_32
		DrawInit(3,uint96);
		DrawRows(3,0);
#undef _THREE_PIX
	}
	else
	{
#define _THREE_PIX THREE_PIX
		DrawInit(3,uint48);
		DrawRows(3,0);
#undef _THREE_PIX
	}

	#undef DrawPix
}

#undef DoRow
#undef InitLine
#undef NextPixel
#undef DrawRows
#undef DrawInit





////////////////////////////////////////////////////////////////////////////////
// HQ2X stuff follows)
////////////////////////////////////////////////////////////////////////////////



#define	Ymask	0xFF0000
#define	Umask	0x00FF00
#define	Vmask	0x0000FF
#define	trY		0x300000
#define	trU		0x000700
#define	trV		0x000006




#define Interp02(c1, c2, c3) \
	((((((c1) & Mask_2) *  2 + ((c2) & Mask_2)     + ((c3) & Mask_2)    ) >> 2) & Mask_2) + \
	(((((c1) & Mask13) *  2 + ((c2) & Mask13)     + ((c3) & Mask13)    ) >> 2) & Mask13))

#define Interp06(c1, c2, c3) \
	((((((c1) & Mask_2) *  5 + ((c2) & Mask_2) * 2 + ((c3) & Mask_2)    ) >> 3) & Mask_2) + \
	(((((c1) & Mask13) *  5 + ((c2) & Mask13) * 2 + ((c3) & Mask13)    ) >> 3) & Mask13))

#define Interp07(c1, c2, c3) \
	((((((c1) & Mask_2) *  6 + ((c2) & Mask_2)     + ((c3) & Mask_2)    ) >> 3) & Mask_2) + \
	(((((c1) & Mask13) *  6 + ((c2) & Mask13)     + ((c3) & Mask13)    ) >> 3) & Mask13))

#define Interp09(c1, c2, c3) \
	((((((c1) & Mask_2) *  2 + ((c2) & Mask_2) * 3 + ((c3) & Mask_2) * 3) >> 3) & Mask_2) + \
	(((((c1) & Mask13) *  2 + ((c2) & Mask13) * 3 + ((c3) & Mask13) * 3) >> 3) & Mask13))

#define Interp10(c1, c2, c3) \
	((((((c1) & Mask_2) * 14 + ((c2) & Mask_2)     + ((c3) & Mask_2)    ) >> 4) & Mask_2) + \
	(((((c1) & Mask13) * 14 + ((c2) & Mask13)     + ((c3) & Mask13)    ) >> 4) & Mask13))

#define PIXEL00_0		*(dp) = w5
#define PIXEL00_10		*(dp) = Interp01(w5, w1)
#define PIXEL00_11		*(dp) = Interp01(w5, w4)
#define PIXEL00_12		*(dp) = Interp01(w5, w2)
#define PIXEL00_20		*(dp) = Interp02(w5, w4, w2)
#define PIXEL00_21		*(dp) = Interp02(w5, w1, w2)
#define PIXEL00_22		*(dp) = Interp02(w5, w1, w4)
#define PIXEL00_60		*(dp) = Interp06(w5, w2, w4)
#define PIXEL00_61		*(dp) = Interp06(w5, w4, w2)
#define PIXEL00_70		*(dp) = Interp07(w5, w4, w2)
#define PIXEL00_90		*(dp) = Interp09(w5, w4, w2)
#define PIXEL00_100		*(dp) = Interp10(w5, w4, w2)

#define PIXEL01_0		*(dp + 1) = w5
#define PIXEL01_10		*(dp + 1) = Interp01(w5, w3)
#define PIXEL01_11		*(dp + 1) = Interp01(w5, w2)
#define PIXEL01_12		*(dp + 1) = Interp01(w5, w6)
#define PIXEL01_20		*(dp + 1) = Interp02(w5, w2, w6)
#define PIXEL01_21		*(dp + 1) = Interp02(w5, w3, w6)
#define PIXEL01_22		*(dp + 1) = Interp02(w5, w3, w2)
#define PIXEL01_60		*(dp + 1) = Interp06(w5, w6, w2)
#define PIXEL01_61		*(dp + 1) = Interp06(w5, w2, w6)
#define PIXEL01_70		*(dp + 1) = Interp07(w5, w2, w6)
#define PIXEL01_90		*(dp + 1) = Interp09(w5, w2, w6)
#define PIXEL01_100		*(dp + 1) = Interp10(w5, w2, w6)

#define PIXEL10_0		*(dp + dst1line) = w5
#define PIXEL10_10		*(dp + dst1line) = Interp01(w5, w7)
#define PIXEL10_11		*(dp + dst1line) = Interp01(w5, w8)
#define PIXEL10_12		*(dp + dst1line) = Interp01(w5, w4)
#define PIXEL10_20		*(dp + dst1line) = Interp02(w5, w8, w4)
#define PIXEL10_21		*(dp + dst1line) = Interp02(w5, w7, w4)
#define PIXEL10_22		*(dp + dst1line) = Interp02(w5, w7, w8)
#define PIXEL10_60		*(dp + dst1line) = Interp06(w5, w4, w8)
#define PIXEL10_61		*(dp + dst1line) = Interp06(w5, w8, w4)
#define PIXEL10_70		*(dp + dst1line) = Interp07(w5, w8, w4)
#define PIXEL10_90		*(dp + dst1line) = Interp09(w5, w8, w4)
#define PIXEL10_100		*(dp + dst1line) = Interp10(w5, w8, w4)

#define PIXEL11_0		*(dp + dst1line + 1) = w5
#define PIXEL11_10		*(dp + dst1line + 1) = Interp01(w5, w9)
#define PIXEL11_11		*(dp + dst1line + 1) = Interp01(w5, w6)
#define PIXEL11_12		*(dp + dst1line + 1) = Interp01(w5, w8)
#define PIXEL11_20		*(dp + dst1line + 1) = Interp02(w5, w6, w8)
#define PIXEL11_21		*(dp + dst1line + 1) = Interp02(w5, w9, w8)
#define PIXEL11_22		*(dp + dst1line + 1) = Interp02(w5, w9, w6)
#define PIXEL11_60		*(dp + dst1line + 1) = Interp06(w5, w8, w6)
#define PIXEL11_61		*(dp + dst1line + 1) = Interp06(w5, w6, w8)
#define PIXEL11_70		*(dp + dst1line + 1) = Interp07(w5, w6, w8)
#define PIXEL11_90		*(dp + dst1line + 1) = Interp09(w5, w6, w8)
#define PIXEL11_100		*(dp + dst1line + 1) = Interp10(w5, w6, w8)

#define Absolute(c) \
(!(c & (1 << 31)) ? c : (~c + 1))


static inline bool Diff(int c1, int c2)
{
	if (Absolute((c1 & Ymask) - (c2 & Ymask)) > trY) return true;
	if (Absolute((c1 & Umask) - (c2 & Umask)) > trU) return true;
	if (Absolute((c1 & Vmask) - (c2 & Vmask)) > trV) return true;

	return false;
}

void InitLUTs(void)
{
	int	c, r, g, b, y, u, v;

	for (c = 0 ; c < (1<<NUMBITS) ; c++)
  	{
#ifdef R5G6B5
		b = (int)((c & 0x1F)) << 3;
		g = (int)((c & 0x7E0)) >> 3;
		r = (int)((c & 0xF800)) >> 8;
#else
		b = (int)((c & 0x1F)) << 3;
		g = (int)((c & 0x3E0)) >> 2;
		r = (int)((c & 0x7C00)) >> 7;
#endif
		RGBtoBright[c] = r+r+r + g+g+g + b+b;

		y = (int)( 0.256788f*r + 0.504129f*g + 0.097906f*b + 0.5f) + 16;
		u = (int)(-0.148223f*r - 0.290993f*g + 0.439216f*b + 0.5f) + 128;
		v = (int)( 0.439216f*r - 0.367788f*g - 0.071427f*b + 0.5f) + 128;

		RGBtoYUV[c] = (y << 16) + (u << 8) + v;

//		y = (r + g + b) >> 2;
//    	u = 128 + ((r - b) >> 2);
//    	v = 128 + ((-r + 2 * g - b) >> 3);
//
//		RGBtoYUVLQ[c] = (y << 16) + (u << 8) + v;
	}
}

#define HQ2XCASES \
	case 0: case 1: case 4: case 32: case 128: case 5: case 132: case 160: case 33: case 129: case 36: case 133: case 164: case 161: case 37: case 165: PIXEL00_20; PIXEL01_20; PIXEL10_20; PIXEL11_20; break; \
	case 2: case 34: case 130: case 162: PIXEL00_22; PIXEL01_21; PIXEL10_20; PIXEL11_20; break; \
	case 16: case 17: case 48: case 49: PIXEL00_20; PIXEL01_22; PIXEL10_20; PIXEL11_21; break; \
	case 64: case 65: case 68: case 69: PIXEL00_20; PIXEL01_20; PIXEL10_21; PIXEL11_22; break; \
	case 8: case 12: case 136: case 140: PIXEL00_21; PIXEL01_20; PIXEL10_22; PIXEL11_20; break; \
	case 3: case 35: case 131: case 163: PIXEL00_11; PIXEL01_21; PIXEL10_20; PIXEL11_20; break; \
	case 6: case 38: case 134: case 166: PIXEL00_22; PIXEL01_12; PIXEL10_20; PIXEL11_20; break; \
	case 20: case 21: case 52: case 53: PIXEL00_20; PIXEL01_11; PIXEL10_20; PIXEL11_21; break; \
	case 144: case 145: case 176: case 177: PIXEL00_20; PIXEL01_22; PIXEL10_20; PIXEL11_12; break; \
	case 192: case 193: case 196: case 197: PIXEL00_20; PIXEL01_20; PIXEL10_21; PIXEL11_11; break; \
	case 96: case 97: case 100: case 101: PIXEL00_20; PIXEL01_20; PIXEL10_12; PIXEL11_22; break; \
	case 40: case 44: case 168: case 172: PIXEL00_21; PIXEL01_20; PIXEL10_11; PIXEL11_20; break; \
	case 9: case 13: case 137: case 141: PIXEL00_12; PIXEL01_20; PIXEL10_22; PIXEL11_20; break; \
	case 18: case 50: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_20; PIXEL10_20; PIXEL11_21; break; \
	case 80: case 81: PIXEL00_20; PIXEL01_22; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_20; break; \
	case 72: case 76: PIXEL00_21; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_20; PIXEL11_22; break; \
	case 10: case 138: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_20; PIXEL01_21; PIXEL10_22; PIXEL11_20; break; \
	case 66: PIXEL00_22; PIXEL01_21; PIXEL10_21; PIXEL11_22; break; \
	case 24: PIXEL00_21; PIXEL01_22; PIXEL10_22; PIXEL11_21; break; \
	case 7: case 39: case 135: PIXEL00_11; PIXEL01_12; PIXEL10_20; PIXEL11_20; break; \
	case 148: case 149: case 180: PIXEL00_20; PIXEL01_11; PIXEL10_20; PIXEL11_12; break; \
	case 224: case 228: case 225: PIXEL00_20; PIXEL01_20; PIXEL10_12; PIXEL11_11; break; \
	case 41: case 169: case 45: PIXEL00_12; PIXEL01_20; PIXEL10_11; PIXEL11_20; break; \
	case 22: case 54: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_20; PIXEL11_21; break; \
	case 208: case 209: PIXEL00_20; PIXEL01_22; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 104: case 108: PIXEL00_21; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_22; break; \
	case 11: case 139: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_21; PIXEL10_22; PIXEL11_20; break; \
	case 19: case 51: if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL00_11, PIXEL01_10; else PIXEL00_60, PIXEL01_90; PIXEL10_20; PIXEL11_21; break; \
	case 146: case 178: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10, PIXEL11_12; else PIXEL01_90, PIXEL11_61; PIXEL10_20; break; \
	case 84: case 85: PIXEL00_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL01_11, PIXEL11_10; else PIXEL01_60, PIXEL11_90; PIXEL10_21; break; \
	case 112: case 113: PIXEL00_20; PIXEL01_22; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL10_12, PIXEL11_10; else PIXEL10_61, PIXEL11_90; break; \
	case 200: case 204: PIXEL00_21; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10, PIXEL11_11; else PIXEL10_90, PIXEL11_60; break; \
	case 73: case 77: if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL00_12, PIXEL10_10; else PIXEL00_61, PIXEL10_90; PIXEL01_20; PIXEL11_22; break; \
	case 42: case 170: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10, PIXEL10_11; else PIXEL00_90, PIXEL10_60; PIXEL01_21; PIXEL11_20; break; \
	case 14: case 142: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10, PIXEL01_12; else PIXEL00_90, PIXEL01_61; PIXEL10_22; PIXEL11_20; break; \
	case 67: PIXEL00_11; PIXEL01_21; PIXEL10_21; PIXEL11_22; break; \
	case 70: PIXEL00_22; PIXEL01_12; PIXEL10_21; PIXEL11_22; break; \
	case 28: PIXEL00_21; PIXEL01_11; PIXEL10_22; PIXEL11_21; break; \
	case 152: PIXEL00_21; PIXEL01_22; PIXEL10_22; PIXEL11_12; break; \
	case 194: PIXEL00_22; PIXEL01_21; PIXEL10_21; PIXEL11_11; break; \
	case 98: PIXEL00_22; PIXEL01_21; PIXEL10_12; PIXEL11_22; break; \
	case 56: PIXEL00_21; PIXEL01_22; PIXEL10_11; PIXEL11_21; break; \
	case 25: PIXEL00_12; PIXEL01_22; PIXEL10_22; PIXEL11_21; break; \
	case 26: case 31: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_22; PIXEL11_21; break; \
	case 82: case 214: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 88: case 248: PIXEL00_21; PIXEL01_22; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 74: case 107: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_21; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_22; break; \
	case 27: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_10; PIXEL10_22; PIXEL11_21; break; \
	case 86: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_21; PIXEL11_10; break; \
	case 216: PIXEL00_21; PIXEL01_22; PIXEL10_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 106: PIXEL00_10; PIXEL01_21; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_22; break; \
	case 30: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_22; PIXEL11_21; break; \
	case 210: PIXEL00_22; PIXEL01_10; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 120: PIXEL00_21; PIXEL01_22; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_10; break; \
	case 75: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_21; PIXEL10_10; PIXEL11_22; break; \
	case 29: PIXEL00_12; PIXEL01_11; PIXEL10_22; PIXEL11_21; break; \
	case 198: PIXEL00_22; PIXEL01_12; PIXEL10_21; PIXEL11_11; break; \
	case 184: PIXEL00_21; PIXEL01_22; PIXEL10_11; PIXEL11_12; break; \
	case 99: PIXEL00_11; PIXEL01_21; PIXEL10_12; PIXEL11_22; break; \
	case 57: PIXEL00_12; PIXEL01_22; PIXEL10_11; PIXEL11_21; break; \
	case 71: PIXEL00_11; PIXEL01_12; PIXEL10_21; PIXEL11_22; break; \
	case 156: PIXEL00_21; PIXEL01_11; PIXEL10_22; PIXEL11_12; break; \
	case 226: PIXEL00_22; PIXEL01_21; PIXEL10_12; PIXEL11_11; break; \
	case 60: PIXEL00_21; PIXEL01_11; PIXEL10_11; PIXEL11_21; break; \
	case 195: PIXEL00_11; PIXEL01_21; PIXEL10_21; PIXEL11_11; break; \
	case 102: PIXEL00_22; PIXEL01_12; PIXEL10_12; PIXEL11_22; break; \
	case 153: PIXEL00_12; PIXEL01_22; PIXEL10_22; PIXEL11_12; break; \
	case 58: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_11; PIXEL11_21; break; \
	case 83: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 92: PIXEL00_21; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 202: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; PIXEL01_21; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; PIXEL11_11; break; \
	case 78: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; PIXEL11_22; break; \
	case 154: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_22; PIXEL11_12; break; \
	case 114: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 89: PIXEL00_12; PIXEL01_22; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 90: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 55: case 23: if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL00_11, PIXEL01_0; else PIXEL00_60, PIXEL01_90; PIXEL10_20; PIXEL11_21; break; \
	case 182: case 150: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0, PIXEL11_12; else PIXEL01_90, PIXEL11_61; PIXEL10_20; break; \
	case 213: case 212: PIXEL00_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL01_11, PIXEL11_0; else PIXEL01_60, PIXEL11_90; PIXEL10_21; break; \
	case 241: case 240: PIXEL00_20; PIXEL01_22; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL10_12, PIXEL11_0; else PIXEL10_61, PIXEL11_90; break; \
	case 236: case 232: PIXEL00_21; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0, PIXEL11_11; else PIXEL10_90, PIXEL11_60; break; \
	case 109: case 105: if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL00_12, PIXEL10_0; else PIXEL00_61, PIXEL10_90; PIXEL01_20; PIXEL11_22; break; \
	case 171: case 43: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0, PIXEL10_11; else PIXEL00_90, PIXEL10_60; PIXEL01_21; PIXEL11_20; break; \
	case 143: case 15: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0, PIXEL01_12; else PIXEL00_90, PIXEL01_61; PIXEL10_22; PIXEL11_20; break; \
	case 124: PIXEL00_21; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_10; break; \
	case 203: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_21; PIXEL10_10; PIXEL11_11; break; \
	case 62: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_11; PIXEL11_21; break; \
	case 211: PIXEL00_11; PIXEL01_10; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 118: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_12; PIXEL11_10; break; \
	case 217: PIXEL00_12; PIXEL01_22; PIXEL10_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 110: PIXEL00_10; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_22; break; \
	case 155: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_10; PIXEL10_22; PIXEL11_12; break; \
	case 188: PIXEL00_21; PIXEL01_11; PIXEL10_11; PIXEL11_12; break; \
	case 185: PIXEL00_12; PIXEL01_22; PIXEL10_11; PIXEL11_12; break; \
	case 61: PIXEL00_12; PIXEL01_11; PIXEL10_11; PIXEL11_21; break; \
	case 157: PIXEL00_12; PIXEL01_11; PIXEL10_22; PIXEL11_12; break; \
	case 103: PIXEL00_11; PIXEL01_12; PIXEL10_12; PIXEL11_22; break; \
	case 227: PIXEL00_11; PIXEL01_21; PIXEL10_12; PIXEL11_11; break; \
	case 230: PIXEL00_22; PIXEL01_12; PIXEL10_12; PIXEL11_11; break; \
	case 199: PIXEL00_11; PIXEL01_12; PIXEL10_21; PIXEL11_11; break; \
	case 220: PIXEL00_21; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 158: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_22; PIXEL11_12; break; \
	case 234: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; PIXEL01_21; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_11; break; \
	case 242: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 59: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_11; PIXEL11_21; break; \
	case 121: PIXEL00_12; PIXEL01_22; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 87: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 79: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; PIXEL11_22; break; \
	case 122: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 94: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 218: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 91: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 229: PIXEL00_20; PIXEL01_20; PIXEL10_12; PIXEL11_11; break; \
	case 167: PIXEL00_11; PIXEL01_12; PIXEL10_20; PIXEL11_20; break; \
	case 173: PIXEL00_12; PIXEL01_20; PIXEL10_11; PIXEL11_20; break; \
	case 181: PIXEL00_20; PIXEL01_11; PIXEL10_20; PIXEL11_12; break; \
	case 186: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_11; PIXEL11_12; break; \
	case 115: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 93: PIXEL00_12; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 206: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; PIXEL11_11; break; \
	case 205: case 201: PIXEL00_12; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_10; else PIXEL10_70; PIXEL11_11; break; \
	case 174: case 46: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_10; else PIXEL00_70; PIXEL01_12; PIXEL10_11; PIXEL11_20; break; \
	case 179: case 147: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_10; else PIXEL01_70; PIXEL10_20; PIXEL11_12; break; \
	case 117: case 116: PIXEL00_20; PIXEL01_11; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_10; else PIXEL11_70; break; \
	case 189: PIXEL00_12; PIXEL01_11; PIXEL10_11; PIXEL11_12; break; \
	case 231: PIXEL00_11; PIXEL01_12; PIXEL10_12; PIXEL11_11; break; \
	case 126: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_10; break; \
	case 219: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_10; PIXEL10_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 125: if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL00_12, PIXEL10_0; else PIXEL00_61, PIXEL10_90; PIXEL01_11; PIXEL11_10; break; \
	case 221: PIXEL00_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL01_11, PIXEL11_0; else PIXEL01_60, PIXEL11_90; PIXEL10_10; break; \
	case 207: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0, PIXEL01_12; else PIXEL00_90, PIXEL01_61; PIXEL10_10; PIXEL11_11; break; \
	case 238: PIXEL00_10; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0, PIXEL11_11; else PIXEL10_90, PIXEL11_60; break; \
	case 190: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0, PIXEL11_12; else PIXEL01_90, PIXEL11_61; PIXEL10_11; break; \
	case 187: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0, PIXEL10_11; else PIXEL00_90, PIXEL10_60; PIXEL01_10; PIXEL11_12; break; \
	case 243: PIXEL00_11; PIXEL01_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL10_12, PIXEL11_0; else PIXEL10_61, PIXEL11_90; break; \
	case 119: if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL00_11, PIXEL01_0; else PIXEL00_60, PIXEL01_90; PIXEL10_12; PIXEL11_10; break; \
	case 237: case 233: PIXEL00_12; PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; PIXEL11_11; break; \
	case 175: case 47: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; PIXEL01_12; PIXEL10_11; PIXEL11_20; break; \
	case 183: case 151: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_20; PIXEL11_12; break; \
	case 245: case 244: PIXEL00_20; PIXEL01_11; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 250: PIXEL00_10; PIXEL01_10; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 123: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_10; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_10; break; \
	case 95: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_10; PIXEL11_10; break; \
	case 222: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 252: PIXEL00_21; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 249: PIXEL00_12; PIXEL01_22; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 235: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_21; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; PIXEL11_11; break; \
	case 111: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_22; break; \
	case 63: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_11; PIXEL11_21; break; \
	case 159: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_22; PIXEL11_12; break; \
	case 215: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_21; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 246: PIXEL00_22; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 254: PIXEL00_10; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 253: PIXEL00_12; PIXEL01_11; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 251: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; PIXEL01_10; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 239: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; PIXEL01_12; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; PIXEL11_11; break; \
	case 127: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_20; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_20; PIXEL11_10; break; \
	case 191: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_11; PIXEL11_12; break; \
	case 223: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_20; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_10; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_20; break; \
	case 247: PIXEL00_11; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; PIXEL10_12; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break; \
	case 255: if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) PIXEL00_0; else PIXEL00_100; if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) PIXEL01_0; else PIXEL01_100; if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) PIXEL10_0; else PIXEL10_100; if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) PIXEL11_0; else PIXEL11_100; break;

template<int GuiScale>
void RenderHQ2X (SSurface Src, SSurface Dst, RECT *rect)
{
    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {
		RenderSimple2X (Src, Dst, rect);
        return;
    }

    uint8 *srcPtr = Src.Surface;
    uint8 *dstPtr = Dst.Surface;
    uint32 srcPitch = Src.Pitch;
    uint32 dstPitch = Dst.Pitch;
    int width = Src.Width;
    int height = Src.Height;

	SetRect(rect, 256, height, 2);

    dstPtr += rect->top * Dst.Pitch + rect->left * 2;
	int	w1, w2, w3, w4, w5, w6, w7, w8, w9;

	uint32	src1line = srcPitch >> 1;
	uint32	dst1line = dstPitch >> 1;
	uint16	*sp = (uint16 *) srcPtr;
	uint16	*dp = (uint16 *) dstPtr;

	const int* RGBtoYUVtable = RGBtoYUV;

	uint32  pattern;
	int		l, y;

	while (height--)
	{
		sp--;

		w1 = *(sp - src1line);
		w4 = *(sp);
		w7 = *(sp + src1line);

		sp++;

		w2 = *(sp - src1line);
		w5 = *(sp);
		w8 = *(sp + src1line);

		for (l = width; l; l--)
		{
			sp++;

			w3 = *(sp - src1line);
			w6 = *(sp);
			w9 = *(sp + src1line);

			pattern = 0;

			switch(GuiScale)
			{
			case FILTER_HQ2XBOLD: {
				const uint16 avg = (RGBtoBright[w1] + RGBtoBright[w2] + RGBtoBright[w3] + RGBtoBright[w4] + RGBtoBright[w5] + RGBtoBright[w6] + RGBtoBright[w7] + RGBtoBright[w8] + RGBtoBright[w9]) / 9;
				const bool diff5 = RGBtoBright[w5] > avg;
				if ((w1 != w5) && ((RGBtoBright[w1] > avg) != diff5)) pattern |= (1 << 0);
				if ((w2 != w5) && ((RGBtoBright[w2] > avg) != diff5)) pattern |= (1 << 1);
				if ((w3 != w5) && ((RGBtoBright[w3] > avg) != diff5)) pattern |= (1 << 2);
				if ((w4 != w5) && ((RGBtoBright[w4] > avg) != diff5)) pattern |= (1 << 3);
				if ((w6 != w5) && ((RGBtoBright[w6] > avg) != diff5)) pattern |= (1 << 4);
				if ((w7 != w5) && ((RGBtoBright[w7] > avg) != diff5)) pattern |= (1 << 5);
				if ((w8 != w5) && ((RGBtoBright[w8] > avg) != diff5)) pattern |= (1 << 6);
				if ((w9 != w5) && ((RGBtoBright[w9] > avg) != diff5)) pattern |= (1 << 7);
              }  break;

			case FILTER_HQ2XS: {
				bool nosame = true;
				if(w1 == w5 || w3 == w5 || w7 == w5 || w9 == w5)
					nosame = false;

				if(nosame)
				{
					const uint16 avg = (RGBtoBright[w1] + RGBtoBright[w2] + RGBtoBright[w3] + RGBtoBright[w4] + RGBtoBright[w5] + RGBtoBright[w6] + RGBtoBright[w7] + RGBtoBright[w8] + RGBtoBright[w9]) / 9;
					const bool diff5 = RGBtoBright[w5] > avg;
					if((RGBtoBright[w1] > avg) != diff5) pattern |= (1 << 0);
					if((RGBtoBright[w2] > avg) != diff5) pattern |= (1 << 1);
					if((RGBtoBright[w3] > avg) != diff5) pattern |= (1 << 2);
					if((RGBtoBright[w4] > avg) != diff5) pattern |= (1 << 3);
					if((RGBtoBright[w6] > avg) != diff5) pattern |= (1 << 4);
					if((RGBtoBright[w7] > avg) != diff5) pattern |= (1 << 5);
					if((RGBtoBright[w8] > avg) != diff5) pattern |= (1 << 6);
					if((RGBtoBright[w9] > avg) != diff5) pattern |= (1 << 7);
				}
				else
				{
					y = RGBtoYUV[w5];
					if ((w1 != w5) && (Diff(y, RGBtoYUV[w1]))) pattern |= (1 << 0);
					if ((w2 != w5) && (Diff(y, RGBtoYUV[w2]))) pattern |= (1 << 1);
					if ((w3 != w5) && (Diff(y, RGBtoYUV[w3]))) pattern |= (1 << 2);
					if ((w4 != w5) && (Diff(y, RGBtoYUV[w4]))) pattern |= (1 << 3);
					if ((w6 != w5) && (Diff(y, RGBtoYUV[w6]))) pattern |= (1 << 4);
					if ((w7 != w5) && (Diff(y, RGBtoYUV[w7]))) pattern |= (1 << 5);
					if ((w8 != w5) && (Diff(y, RGBtoYUV[w8]))) pattern |= (1 << 6);
					if ((w9 != w5) && (Diff(y, RGBtoYUV[w9]))) pattern |= (1 << 7);
				}
              }  break;
	        default:
            case FILTER_HQ2X:
				y = RGBtoYUVtable[w5];
				if ((w1 != w5) && (Diff(y, RGBtoYUVtable[w1]))) pattern |= (1 << 0);
				if ((w2 != w5) && (Diff(y, RGBtoYUVtable[w2]))) pattern |= (1 << 1);
				if ((w3 != w5) && (Diff(y, RGBtoYUVtable[w3]))) pattern |= (1 << 2);
				if ((w4 != w5) && (Diff(y, RGBtoYUVtable[w4]))) pattern |= (1 << 3);
				if ((w6 != w5) && (Diff(y, RGBtoYUVtable[w6]))) pattern |= (1 << 4);
				if ((w7 != w5) && (Diff(y, RGBtoYUVtable[w7]))) pattern |= (1 << 5);
				if ((w8 != w5) && (Diff(y, RGBtoYUVtable[w8]))) pattern |= (1 << 6);
				if ((w9 != w5) && (Diff(y, RGBtoYUVtable[w9]))) pattern |= (1 << 7);
                break;
			}

			switch (pattern)
			{
				HQ2XCASES
			}

			w1 = w2; w4 = w5; w7 = w8;
			w2 = w3; w5 = w6; w8 = w9;

			dp += 2;
		}

		dp += ((dst1line - width) << 1);
		sp +=  (src1line - width);
	}
}














#define PIXEL00_1M  *(dp) = Interp01(w5, w1);
#define PIXEL00_1U  *(dp) = Interp01(w5, w2);
#define PIXEL00_1L  *(dp) = Interp01(w5, w4);
#define PIXEL00_2   *(dp) = Interp02(w5, w4, w2);
#define PIXEL00_4   *(dp) = Interp04(w5, w4, w2);
#define PIXEL00_5   *(dp) = Interp05(w4, w2);
#define PIXEL00_C   *(dp) = w5;

#define PIXEL01_1   *(dp + 1) = Interp01(w5, w2);
#define PIXEL01_3   *(dp + 1) = Interp03(w5, w2);
#define PIXEL01_6   *(dp + 1) = Interp01(w2, w5);
#define PIXEL01_C   *(dp + 1) = w5;

#define PIXEL02_1M  *(dp + 2) = Interp01(w5, w3);
#define PIXEL02_1U  *(dp + 2) = Interp01(w5, w2);
#define PIXEL02_1R  *(dp + 2) = Interp01(w5, w6);
#define PIXEL02_2   *(dp + 2) = Interp02(w5, w2, w6);
#define PIXEL02_4   *(dp + 2) = Interp04(w5, w2, w6);
#define PIXEL02_5   *(dp + 2) = Interp05(w2, w6);
#define PIXEL02_C   *(dp + 2) = w5;

#define PIXEL10_1   *(dp + dst1line) = Interp01(w5, w4);
#define PIXEL10_3   *(dp + dst1line) = Interp03(w5, w4);
#define PIXEL10_6   *(dp + dst1line) = Interp01(w4, w5);
#define PIXEL10_C   *(dp + dst1line) = w5;

#define PIXEL11     *(dp + dst1line + 1) = w5;

#define PIXEL12_1   *(dp + dst1line + 2) = Interp01(w5, w6);
#define PIXEL12_3   *(dp + dst1line + 2) = Interp03(w5, w6);
#define PIXEL12_6   *(dp + dst1line + 2) = Interp01(w6, w5);
#define PIXEL12_C   *(dp + dst1line + 2) = w5;

#define PIXEL20_1M  *(dp + dst1line + dst1line) = Interp01(w5, w7);
#define PIXEL20_1D  *(dp + dst1line + dst1line) = Interp01(w5, w8);
#define PIXEL20_1L  *(dp + dst1line + dst1line) = Interp01(w5, w4);
#define PIXEL20_2   *(dp + dst1line + dst1line) = Interp02(w5, w8, w4);
#define PIXEL20_4   *(dp + dst1line + dst1line) = Interp04(w5, w8, w4);
#define PIXEL20_5   *(dp + dst1line + dst1line) = Interp05(w8, w4);
#define PIXEL20_C   *(dp + dst1line + dst1line) = w5;

#define PIXEL21_1   *(dp + dst1line + dst1line + 1) = Interp01(w5, w8);
#define PIXEL21_3   *(dp + dst1line + dst1line + 1) = Interp03(w5, w8);
#define PIXEL21_6   *(dp + dst1line + dst1line + 1) = Interp01(w8, w5);
#define PIXEL21_C   *(dp + dst1line + dst1line + 1) = w5;

#define PIXEL22_1M  *(dp + dst1line + dst1line + 2) = Interp01(w5, w9);
#define PIXEL22_1D  *(dp + dst1line + dst1line + 2) = Interp01(w5, w8);
#define PIXEL22_1R  *(dp + dst1line + dst1line + 2) = Interp01(w5, w6);
#define PIXEL22_2   *(dp + dst1line + dst1line + 2) = Interp02(w5, w6, w8);
#define PIXEL22_4   *(dp + dst1line + dst1line + 2) = Interp04(w5, w6, w8);
#define PIXEL22_5   *(dp + dst1line + dst1line + 2) = Interp05(w6, w8);
#define PIXEL22_C   *(dp + dst1line + dst1line + 2) = w5;

#define Interp03(c1, c2) \
	(((c1 == c2) ? c1 : \
	((((((c1) & Mask_2) *  7 + ((c2) & Mask_2)) >> 3) & Mask_2) + \
	(((((c1) & Mask13) *  7 + ((c2) & Mask13)) >> 3) & Mask13))))

#define Interp04(c1, c2, c3) \
	((((((c1) & Mask_2) *  2 + ((c2) & Mask_2) * 7 + ((c3) & Mask_2) * 7) >> 4) & Mask_2) + \
	(((((c1) & Mask13) *  2 + ((c2) & Mask13) * 7 + ((c3) & Mask13) * 7) >> 4) & Mask13))

#define HQ3XCASES \
	case 0: case 1: case 4: case 32: case 128: case 5: case 132: case 160: case 33: case 129: case 36: case 133: case 164: case 161: case 37: case 165: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 2: case 34: case 130: case 162: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 16: case 17: case 48: case 49: { PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 64: case 65: case 68: case 69: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 8: case 12: case 136: case 140: { PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 3: case 35: case 131: case 163: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 6: case 38: case 134: case 166: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 20: case 21: case 52: case 53: { PIXEL00_2 PIXEL01_1 PIXEL02_1U PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 144: case 145: case 176: case 177: { PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1D break; }\
	case 192: case 193: case 196: case 197: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 96: case 97: case 100: case 101: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 40: case 44: case 168: case 172: { PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1D PIXEL21_1 PIXEL22_2 break; }\
	case 9: case 13: case 137: case 141: { PIXEL00_1U PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 18: case 50: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_1M PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 80: case 81: { PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_1M } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 72: case 76: { PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_1M PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 10: case 138: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 66: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 24: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 7: case 39: case 135: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 148: case 149: case 180: { PIXEL00_2 PIXEL01_1 PIXEL02_1U PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1D break; }\
	case 224: case 228: case 225: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 41: case 169: case 45: { PIXEL00_1U PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1D PIXEL21_1 PIXEL22_2 break; }\
	case 22: case 54: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 208: case 209: { PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 104: case 108: { PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 11: case 139: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 19: case 51: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL12_C } else { PIXEL00_2 PIXEL01_6 PIXEL02_5 PIXEL12_1 } PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 146: case 178: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_1M PIXEL12_C PIXEL22_1D } else { PIXEL01_1 PIXEL02_5 PIXEL12_6 PIXEL22_2 } PIXEL00_1M PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 break; }\
	case 84: case 85: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL02_1U PIXEL12_C PIXEL21_C PIXEL22_1M } else { PIXEL02_2 PIXEL12_6 PIXEL21_1 PIXEL22_5 } PIXEL00_2 PIXEL01_1 PIXEL10_1 PIXEL11 PIXEL20_1M break; }\
	case 112: case 113: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL20_1L PIXEL21_C PIXEL22_1M } else { PIXEL12_1 PIXEL20_2 PIXEL21_6 PIXEL22_5 } PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 break; }\
	case 200: case 204: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_1M PIXEL21_C PIXEL22_1R } else { PIXEL10_1 PIXEL20_5 PIXEL21_6 PIXEL22_2 } PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 break; }\
	case 73: case 77: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL00_1U PIXEL10_C PIXEL20_1M PIXEL21_C } else { PIXEL00_2 PIXEL10_6 PIXEL20_5 PIXEL21_1 } PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 PIXEL22_1M break; }\
	case 42: case 170: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M PIXEL01_C PIXEL10_C PIXEL20_1D } else { PIXEL00_5 PIXEL01_1 PIXEL10_6 PIXEL20_2 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL21_1 PIXEL22_2 break; }\
	case 14: case 142: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_C } else { PIXEL00_5 PIXEL01_6 PIXEL02_2 PIXEL10_1 } PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 67: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 70: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 28: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 152: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 194: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 98: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 56: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 25: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 26: case 31: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL10_C } else { PIXEL00_4 PIXEL10_3 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C PIXEL12_C } else { PIXEL02_4 PIXEL12_3 } PIXEL11 PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 82: case 214: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C } else { PIXEL01_3 PIXEL02_4 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL21_C PIXEL22_C } else { PIXEL21_3 PIXEL22_4 } break; }\
	case 88: case 248: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C } else { PIXEL10_3 PIXEL20_4 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL22_C } else { PIXEL12_3 PIXEL22_4 } break; }\
	case 74: case 107: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C } else { PIXEL00_4 PIXEL01_3 } PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C PIXEL21_C } else { PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 27: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 86: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_1 PIXEL11 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 216: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 106: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 30: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_C PIXEL11 PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 210: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 120: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 75: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 29: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1M break; }\
	case 198: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 184: { PIXEL00_1M PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 99: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 57: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 71: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 156: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 226: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 60: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 195: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 102: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 153: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 58: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 83: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 92: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 202: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 78: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C PIXEL22_1M break; }\
	case 154: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 114: { PIXEL00_1M PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 89: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 90: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 55: case 23: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL00_1L PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL00_2 PIXEL01_6 PIXEL02_5 PIXEL12_1 } PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 PIXEL22_1M break; }\
	case 182: case 150: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C PIXEL22_1D } else { PIXEL01_1 PIXEL02_5 PIXEL12_6 PIXEL22_2 } PIXEL00_1M PIXEL10_1 PIXEL11 PIXEL20_2 PIXEL21_1 break; }\
	case 213: case 212: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL02_1U PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL02_2 PIXEL12_6 PIXEL21_1 PIXEL22_5 } PIXEL00_2 PIXEL01_1 PIXEL10_1 PIXEL11 PIXEL20_1M break; }\
	case 241: case 240: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL20_1L PIXEL21_C PIXEL22_C } else { PIXEL12_1 PIXEL20_2 PIXEL21_6 PIXEL22_5 } PIXEL00_2 PIXEL01_1 PIXEL02_1M PIXEL10_1 PIXEL11 break; }\
	case 236: case 232: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C PIXEL22_1R } else { PIXEL10_1 PIXEL20_5 PIXEL21_6 PIXEL22_2 } PIXEL00_1M PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 break; }\
	case 109: case 105: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL00_1U PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL00_2 PIXEL10_6 PIXEL20_5 PIXEL21_1 } PIXEL01_1 PIXEL02_2 PIXEL11 PIXEL12_1 PIXEL22_1M break; }\
	case 171: case 43: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C PIXEL20_1D } else { PIXEL00_5 PIXEL01_1 PIXEL10_6 PIXEL20_2 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL21_1 PIXEL22_2 break; }\
	case 143: case 15: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL02_1R PIXEL10_C } else { PIXEL00_5 PIXEL01_6 PIXEL02_2 PIXEL10_1 } PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_1 PIXEL22_2 break; }\
	case 124: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 203: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 62: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_C PIXEL11 PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 211: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 118: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_1 PIXEL11 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 217: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 110: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 155: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 188: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 185: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 61: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 157: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 103: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 227: { PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 230: { PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 199: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 220: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 158: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_C PIXEL11 PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 234: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C PIXEL02_1M PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1R break; }\
	case 242: { PIXEL00_1M PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL20_1L if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 59: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 121: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 87: { PIXEL00_1L if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_1 PIXEL11 PIXEL20_1M PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 79: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1R PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C PIXEL22_1M break; }\
	case 122: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 94: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL10_C PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 218: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_C PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 91: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 229: { PIXEL00_2 PIXEL01_1 PIXEL02_2 PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 167: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_2 PIXEL21_1 PIXEL22_2 break; }\
	case 173: { PIXEL00_1U PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1D PIXEL21_1 PIXEL22_2 break; }\
	case 181: { PIXEL00_2 PIXEL01_1 PIXEL02_1U PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1D break; }\
	case 186: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 115: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 93: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 206: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 205: case 201: { PIXEL00_1U PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_1M } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 174: case 46: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_1M } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1D PIXEL21_1 PIXEL22_2 break; }\
	case 179: case 147: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_1M } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1D break; }\
	case 117: case 116: { PIXEL00_2 PIXEL01_1 PIXEL02_1U PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_1M } else { PIXEL22_2 } break; }\
	case 189: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 231: { PIXEL00_1L PIXEL01_C PIXEL02_1R PIXEL10_1 PIXEL11 PIXEL12_1 PIXEL20_1L PIXEL21_C PIXEL22_1R break; }\
	case 126: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_4 PIXEL12_3 } PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 219: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_4 PIXEL01_3 PIXEL10_3 } PIXEL02_1M PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_4 } break; }\
	case 125: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL00_1U PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL00_2 PIXEL10_6 PIXEL20_5 PIXEL21_1 } PIXEL01_1 PIXEL02_1U PIXEL11 PIXEL12_C PIXEL22_1M break; }\
	case 221: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL02_1U PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL02_2 PIXEL12_6 PIXEL21_1 PIXEL22_5 } PIXEL00_1U PIXEL01_1 PIXEL10_C PIXEL11 PIXEL20_1M break; }\
	case 207: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL02_1R PIXEL10_C } else { PIXEL00_5 PIXEL01_6 PIXEL02_2 PIXEL10_1 } PIXEL11 PIXEL12_1 PIXEL20_1M PIXEL21_C PIXEL22_1R break; }\
	case 238: { if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C PIXEL22_1R } else { PIXEL10_1 PIXEL20_5 PIXEL21_6 PIXEL22_2 } PIXEL00_1M PIXEL01_C PIXEL02_1R PIXEL11 PIXEL12_1 break; }\
	case 190: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C PIXEL22_1D } else { PIXEL01_1 PIXEL02_5 PIXEL12_6 PIXEL22_2 } PIXEL00_1M PIXEL10_C PIXEL11 PIXEL20_1D PIXEL21_1 break; }\
	case 187: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C PIXEL20_1D } else { PIXEL00_5 PIXEL01_1 PIXEL10_6 PIXEL20_2 } PIXEL02_1M PIXEL11 PIXEL12_C PIXEL21_1 PIXEL22_1D break; }\
	case 243: { if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL20_1L PIXEL21_C PIXEL22_C } else { PIXEL12_1 PIXEL20_2 PIXEL21_6 PIXEL22_5 } PIXEL00_1L PIXEL01_C PIXEL02_1M PIXEL10_1 PIXEL11 break; }\
	case 119: { if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL00_1L PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL00_2 PIXEL01_6 PIXEL02_5 PIXEL12_1 } PIXEL10_1 PIXEL11 PIXEL20_1L PIXEL21_C PIXEL22_1M break; }\
	case 237: case 233: { PIXEL00_1U PIXEL01_1 PIXEL02_2 PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 175: case 47: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 PIXEL20_1D PIXEL21_1 PIXEL22_2 break; }\
	case 183: case 151: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_2 PIXEL21_1 PIXEL22_1D break; }\
	case 245: case 244: { PIXEL00_2 PIXEL01_1 PIXEL02_1U PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }\
	case 250: { PIXEL00_1M PIXEL01_C PIXEL02_1M PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C } else { PIXEL10_3 PIXEL20_4 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL22_C } else { PIXEL12_3 PIXEL22_4 } break; }\
	case 123: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C } else { PIXEL00_4 PIXEL01_3 } PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C PIXEL21_C } else { PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 95: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL10_C } else { PIXEL00_4 PIXEL10_3 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C PIXEL12_C } else { PIXEL02_4 PIXEL12_3 } PIXEL11 PIXEL20_1M PIXEL21_C PIXEL22_1M break; }\
	case 222: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C } else { PIXEL01_3 PIXEL02_4 } PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL21_C PIXEL22_C } else { PIXEL21_3 PIXEL22_4 } break; }\
	case 252: { PIXEL00_1M PIXEL01_1 PIXEL02_1U PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C } else { PIXEL10_3 PIXEL20_4 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }\
	case 249: { PIXEL00_1U PIXEL01_1 PIXEL02_1M PIXEL10_C PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL22_C } else { PIXEL12_3 PIXEL22_4 } break; }\
	case 235: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C } else { PIXEL00_4 PIXEL01_3 } PIXEL02_1M PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 111: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C PIXEL21_C } else { PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 63: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C PIXEL12_C } else { PIXEL02_4 PIXEL12_3 } PIXEL10_C PIXEL11 PIXEL20_1D PIXEL21_1 PIXEL22_1M break; }\
	case 159: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL10_C } else { PIXEL00_4 PIXEL10_3 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL11 PIXEL12_C PIXEL20_1M PIXEL21_1 PIXEL22_1D break; }\
	case 215: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL21_C PIXEL22_C } else { PIXEL21_3 PIXEL22_4 } break; }\
	case 246: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C } else { PIXEL01_3 PIXEL02_4 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }\
	case 254: { PIXEL00_1M if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C } else { PIXEL01_3 PIXEL02_4 } PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C } else { PIXEL10_3 PIXEL20_4 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL21_C PIXEL22_C } else { PIXEL12_3 PIXEL21_3 PIXEL22_2 } break; }\
	case 253: { PIXEL00_1U PIXEL01_1 PIXEL02_1U PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }\
	case 251: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C } else { PIXEL00_4 PIXEL01_3 } PIXEL02_1M PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL10_C PIXEL20_C PIXEL21_C } else { PIXEL10_3 PIXEL20_2 PIXEL21_3 } if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL12_C PIXEL22_C } else { PIXEL12_3 PIXEL22_4 } break; }\
	case 239: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C PIXEL02_1R PIXEL10_C PIXEL11 PIXEL12_1 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C PIXEL22_1R break; }\
	case 127: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL01_C PIXEL10_C } else { PIXEL00_2 PIXEL01_3 PIXEL10_3 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C PIXEL12_C } else { PIXEL02_4 PIXEL12_3 } PIXEL11 if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C PIXEL21_C } else { PIXEL20_4 PIXEL21_3 } PIXEL22_1M break; }\
	case 191: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C PIXEL20_1D PIXEL21_1 PIXEL22_1D break; }\
	case 223: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C PIXEL10_C } else { PIXEL00_4 PIXEL10_3 } if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL01_C PIXEL02_C PIXEL12_C } else { PIXEL01_3 PIXEL02_2 PIXEL12_3 } PIXEL11 PIXEL20_1M if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL21_C PIXEL22_C } else { PIXEL21_3 PIXEL22_4 } break; }\
	case 247: { PIXEL00_1L PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL10_1 PIXEL11 PIXEL12_C PIXEL20_1L PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }\
	case 255: { if (Diff(RGBtoYUVtable[w4], RGBtoYUVtable[w2])) { PIXEL00_C } else { PIXEL00_2 } PIXEL01_C if (Diff(RGBtoYUVtable[w2], RGBtoYUVtable[w6])) { PIXEL02_C } else { PIXEL02_2 } PIXEL10_C PIXEL11 PIXEL12_C if (Diff(RGBtoYUVtable[w8], RGBtoYUVtable[w4])) { PIXEL20_C } else { PIXEL20_2 } PIXEL21_C if (Diff(RGBtoYUVtable[w6], RGBtoYUVtable[w8])) { PIXEL22_C } else { PIXEL22_2 } break; }


template<int GuiScale>
void RenderHQ3X (SSurface Src, SSurface Dst, RECT *rect)
{
    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {
		RenderSimple3X (Src, Dst, rect);
        return;
    }

    uint8 *srcPtr = Src.Surface;
    uint8 *dstPtr = Dst.Surface;
    uint32 srcPitch = Src.Pitch;
    uint32 dstPitch = Dst.Pitch;
    int width = Src.Width;
    int height = Src.Height;

	SetRect(rect, 256, height, 3);

    dstPtr += rect->top * Dst.Pitch + rect->left * 2;
	int	w1, w2, w3, w4, w5, w6, w7, w8, w9;

	uint32	src1line = srcPitch >> 1;
	uint32	dst1line = dstPitch >> 1;
	uint16	*sp = (uint16 *) srcPtr;
	uint16	*dp = (uint16 *) dstPtr;

	const int* RGBtoYUVtable = RGBtoYUV;

	uint32  pattern;
	int		l, y;

	while (height--)
	{
		sp--;

		w1 = *(sp - src1line);
		w4 = *(sp);
		w7 = *(sp + src1line);

		sp++;

		w2 = *(sp - src1line);
		w5 = *(sp);
		w8 = *(sp + src1line);

		for (l = width; l; l--)
		{
			sp++;

			w3 = *(sp - src1line);
			w6 = *(sp);
			w9 = *(sp + src1line);

			pattern = 0;

			switch(GuiScale)
			{
			case FILTER_HQ3XBOLD: {
				const uint16 avg = (RGBtoBright[w1] + RGBtoBright[w2] + RGBtoBright[w3] + RGBtoBright[w4] + RGBtoBright[w5] + RGBtoBright[w6] + RGBtoBright[w7] + RGBtoBright[w8] + RGBtoBright[w9]) / 9;
				const bool diff5 = RGBtoBright[w5] > avg;
				if ((w1 != w5) && ((RGBtoBright[w1] > avg) != diff5)) pattern |= (1 << 0);
				if ((w2 != w5) && ((RGBtoBright[w2] > avg) != diff5)) pattern |= (1 << 1);
				if ((w3 != w5) && ((RGBtoBright[w3] > avg) != diff5)) pattern |= (1 << 2);
				if ((w4 != w5) && ((RGBtoBright[w4] > avg) != diff5)) pattern |= (1 << 3);
				if ((w6 != w5) && ((RGBtoBright[w6] > avg) != diff5)) pattern |= (1 << 4);
				if ((w7 != w5) && ((RGBtoBright[w7] > avg) != diff5)) pattern |= (1 << 5);
				if ((w8 != w5) && ((RGBtoBright[w8] > avg) != diff5)) pattern |= (1 << 6);
				if ((w9 != w5) && ((RGBtoBright[w9] > avg) != diff5)) pattern |= (1 << 7);
              }  break;

			case FILTER_HQ3XS: {
				bool nosame = true;
				if(w1 == w5 || w3 == w5 || w7 == w5 || w9 == w5)
					nosame = false;

				if(nosame)
				{
					const uint16 avg = (RGBtoBright[w1] + RGBtoBright[w2] + RGBtoBright[w3] + RGBtoBright[w4] + RGBtoBright[w5] + RGBtoBright[w6] + RGBtoBright[w7] + RGBtoBright[w8] + RGBtoBright[w9]) / 9;
					const bool diff5 = RGBtoBright[w5] > avg;
					if((RGBtoBright[w1] > avg) != diff5) pattern |= (1 << 0);
					if((RGBtoBright[w2] > avg) != diff5) pattern |= (1 << 1);
					if((RGBtoBright[w3] > avg) != diff5) pattern |= (1 << 2);
					if((RGBtoBright[w4] > avg) != diff5) pattern |= (1 << 3);
					if((RGBtoBright[w6] > avg) != diff5) pattern |= (1 << 4);
					if((RGBtoBright[w7] > avg) != diff5) pattern |= (1 << 5);
					if((RGBtoBright[w8] > avg) != diff5) pattern |= (1 << 6);
					if((RGBtoBright[w9] > avg) != diff5) pattern |= (1 << 7);
				}
				else
				{
					y = RGBtoYUV[w5];
					if ((w1 != w5) && (Diff(y, RGBtoYUV[w1]))) pattern |= (1 << 0);
					if ((w2 != w5) && (Diff(y, RGBtoYUV[w2]))) pattern |= (1 << 1);
					if ((w3 != w5) && (Diff(y, RGBtoYUV[w3]))) pattern |= (1 << 2);
					if ((w4 != w5) && (Diff(y, RGBtoYUV[w4]))) pattern |= (1 << 3);
					if ((w6 != w5) && (Diff(y, RGBtoYUV[w6]))) pattern |= (1 << 4);
					if ((w7 != w5) && (Diff(y, RGBtoYUV[w7]))) pattern |= (1 << 5);
					if ((w8 != w5) && (Diff(y, RGBtoYUV[w8]))) pattern |= (1 << 6);
					if ((w9 != w5) && (Diff(y, RGBtoYUV[w9]))) pattern |= (1 << 7);
				}
              }  break;
	        default:
            case FILTER_HQ3X:
				y = RGBtoYUVtable[w5];
				if ((w1 != w5) && (Diff(y, RGBtoYUVtable[w1]))) pattern |= (1 << 0);
				if ((w2 != w5) && (Diff(y, RGBtoYUVtable[w2]))) pattern |= (1 << 1);
				if ((w3 != w5) && (Diff(y, RGBtoYUVtable[w3]))) pattern |= (1 << 2);
				if ((w4 != w5) && (Diff(y, RGBtoYUVtable[w4]))) pattern |= (1 << 3);
				if ((w6 != w5) && (Diff(y, RGBtoYUVtable[w6]))) pattern |= (1 << 4);
				if ((w7 != w5) && (Diff(y, RGBtoYUVtable[w7]))) pattern |= (1 << 5);
				if ((w8 != w5) && (Diff(y, RGBtoYUVtable[w8]))) pattern |= (1 << 6);
				if ((w9 != w5) && (Diff(y, RGBtoYUVtable[w9]))) pattern |= (1 << 7);
                break;
			}

			switch (pattern)
			{
				HQ3XCASES
			}

			w1 = w2; w4 = w5; w7 = w8;
			w2 = w3; w5 = w6; w8 = w9;

			dp += 3;
		}

		dp += ((dst1line - width) * 3);
		sp +=  (src1line - width);
	}
}


#undef Interp02
#undef Interp06
#undef Interp07
#undef Interp10
#undef Interp03
#undef Interp01
#undef Interp09
#undef Interp04
#define Interp02(c1, c2, c3) (c1) // choose majority
#define Interp06(c1, c2, c3) (c1)
#define Interp07(c1, c2, c3) (c1)
#define Interp10(c1, c2, c3) (c1)
#define Interp03(c1, c2) (c1)
#define Interp01(c1, c2) (c1)
#define Interp09(c1, c2, c3) Interp05(c2,c3) // 50/50 split of majority
#define Interp04(c1, c2, c3) Interp05(c2,c3)

// same as RenderHQ3XB except mostly choosing colors instead of blending them
void RenderLQ3XB (SSurface Src, SSurface Dst, RECT *rect)
{
    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {
		RenderSimple3X (Src, Dst, rect);
        return;
    }

    uint8 *srcPtr = Src.Surface;
    uint8 *dstPtr = Dst.Surface;
    uint32 srcPitch = Src.Pitch;
    uint32 dstPitch = Dst.Pitch;
    int width = Src.Width;
    int height = Src.Height;

	SetRect(rect, 256, height, 3);

    dstPtr += rect->top * Dst.Pitch + rect->left * 2;
	int	w1, w2, w3, w4, w5, w6, w7, w8, w9;

	uint32	src1line = srcPitch >> 1;
	uint32	dst1line = dstPitch >> 1;
	uint16	*sp = (uint16 *) srcPtr;
	uint16	*dp = (uint16 *) dstPtr;

	const int* RGBtoYUVtable = RGBtoYUV;

	uint32  pattern;
	int		l;//, y;

	while (height--)
	{
		sp--;

		w1 = *(sp - src1line);
		w4 = *(sp);
		w7 = *(sp + src1line);

		sp++;

		w2 = *(sp - src1line);
		w5 = *(sp);
		w8 = *(sp + src1line);

		for (l = width; l; l--)
		{
			sp++;

			w3 = *(sp - src1line);
			w6 = *(sp);
			w9 = *(sp + src1line);

			pattern = 0;

			{
				const uint16 avg = (RGBtoBright[w1] + RGBtoBright[w2] + RGBtoBright[w3] + RGBtoBright[w4] + RGBtoBright[w5] + RGBtoBright[w6] + RGBtoBright[w7] + RGBtoBright[w8] + RGBtoBright[w9]) / 9;
				const bool diff5 = RGBtoBright[w5] > avg;
				if ((w1 != w5) && ((RGBtoBright[w1] > avg) != diff5)) pattern |= (1 << 0);
				if ((w2 != w5) && ((RGBtoBright[w2] > avg) != diff5)) pattern |= (1 << 1);
				if ((w3 != w5) && ((RGBtoBright[w3] > avg) != diff5)) pattern |= (1 << 2);
				if ((w4 != w5) && ((RGBtoBright[w4] > avg) != diff5)) pattern |= (1 << 3);
				if ((w6 != w5) && ((RGBtoBright[w6] > avg) != diff5)) pattern |= (1 << 4);
				if ((w7 != w5) && ((RGBtoBright[w7] > avg) != diff5)) pattern |= (1 << 5);
				if ((w8 != w5) && ((RGBtoBright[w8] > avg) != diff5)) pattern |= (1 << 6);
				if ((w9 != w5) && ((RGBtoBright[w9] > avg) != diff5)) pattern |= (1 << 7);
			}

			switch (pattern)
			{
				HQ3XCASES
			}

			w1 = w2; w4 = w5; w7 = w8;
			w2 = w3; w5 = w6; w8 = w9;

			dp += 3;
		}

		dp += ((dst1line - width) * 3);
		sp +=  (src1line - width);
	}
}


#undef Interp02
#undef Interp06
#undef Interp07
#undef Interp10
#undef Interp03
#undef Interp01
#undef Interp09
#undef Interp04







////////////////////////////////////////////////////////////////////////////////
// Glide and OpenGL stuff follows (basically deprecated)
////////////////////////////////////////////////////////////////////////////////




#ifdef USE_GLIDE
GlideData Glide;

#define TEXTURE_SIZE 256

static uint16 *texture_download_buffer = NULL;
static GrTexInfo texture_info;
static int texture_mem_start [4];

static void S9xGlideCalcSquare (int snes_width, int snes_height,
				int x, int y, int width, int height,
				GrVertex *square)
{
    double snes_aspect_ratio = snes_width / (double) snes_height;
    double voodoo_aspect_ratio = Glide.voodoo_width / (double) Glide.voodoo_height;
    double virtual_height;
    double virtual_width;
    double virtual_x;
    double virtual_y;

#if 0
    if (snes_aspect_ratio < voodoo_aspect_ratio)
    {
	virtual_height = (double) Glide.voodoo_height;
	virtual_width = virtual_height * snes_aspect_ratio;
	virtual_x = ((double) Glide.voodoo_width - virtual_width) / 2.0;
	virtual_y = 0.0;
    }
    else
    {
	virtual_width = Glide.voodoo_width;
	virtual_height = virtual_width / snes_aspect_ratio;
	virtual_x = 0;
	virtual_y = (Glide.voodoo_height - virtual_height) / 2.0;
    }
#endif

    virtual_x = 0;
    virtual_y = 0;
    virtual_width = Glide.voodoo_width;
    virtual_height = Glide.voodoo_height;

    for (int i = 0; i < 4; i++)
    {
	square [i].oow = 1.0;
	square [i].ooz = 65535.0;
	square [i].tmuvtx [0].oow = 1.0;
    }

    square [0].tmuvtx [0].sow = 0.0;
    square [0].tmuvtx [0].tow = 0.0;
    square [1].tmuvtx [0].sow = (double) width;
    square [1].tmuvtx [0].tow = 0.0;
    square [2].tmuvtx [0].sow = (double) width;
    square [2].tmuvtx [0].tow = (double) height;
    square [3].tmuvtx [0].sow = 0.0;
    square [3].tmuvtx [0].tow = (double) height;

    double width_percent;
    double height_percent;
    double width_percent2;
    double height_percent2;

    if ((width_percent = virtual_width * TEXTURE_SIZE / snes_width) > virtual_width)
	width_percent = virtual_width;
    if ((height_percent = virtual_height * TEXTURE_SIZE / snes_height) > virtual_height)
	height_percent = virtual_height;

    width_percent2 = width_percent;
    height_percent2 = height_percent;
#if 1
    if (virtual_x + (x + 1) * width_percent2 > virtual_width)
        width_percent2 -= ((virtual_x + (x + 1) * width_percent2) - virtual_width) / (x + 1);
    if (virtual_y + (y + 1) * height_percent2 > virtual_height)
        height_percent2 -= ((virtual_y + (y + 1) * height_percent2) - virtual_height) / (y + 1);
#endif
    square [0].x = virtual_x + x * width_percent;
    square [0].y = virtual_y + y * height_percent;
    square [1].x = virtual_x + (x + 1) * width_percent2;
    square [1].y = virtual_y + y * height_percent;
    square [2].x = virtual_x + (x + 1) * width_percent2;
    square [2].y = virtual_y + (y + 1) * height_percent2;
    square [3].x = virtual_x + x * width_percent;
    square [3].y = virtual_y + (y + 1) * height_percent2;
}
typedef void (FX_CALL *tpColorCombine) (
               GrCombineFunction_t function, GrCombineFactor_t factor,
               GrCombineLocal_t local, GrCombineOther_t other,
               FxBool invert );
static tpColorCombine pgrColorCombine = NULL;

typedef void (FX_CALL *tpTexCombine) (
             GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor,
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert,
             FxBool alpha_invert
             );
static tpTexCombine pgrTexCombine = NULL;

typedef void (FX_CALL *tpTexMipMapMode) (
                 GrChipID_t     tmu,
                 GrMipMapMode_t mode,
                 FxBool         lodBlend );
static tpTexMipMapMode pgrTexMipMapMode = NULL;

typedef void (FX_CALL *tpTexClampMode) (
               GrChipID_t tmu,
               GrTextureClampMode_t s_clampmode,
               GrTextureClampMode_t t_clampmode
               );
static tpTexClampMode pgrTexClampMode = NULL;

typedef void (FX_CALL *tpTexFilterMode) (
                GrChipID_t tmu,
                GrTextureFilterMode_t minfilter_mode,
                GrTextureFilterMode_t magfilter_mode
                );
static tpTexFilterMode pgrTexFilterMode = NULL;

typedef FxU32 (FX_CALL *tpTexTextureMemRequired) (
                FxU32     evenOdd,
                GrTexInfo *info   );

static tpTexTextureMemRequired pgrTexTextureMemRequired = NULL;

typedef FxU32 (FX_CALL *tpTexMinAddress) (
                GrChipID_t);
static tpTexMinAddress pgrTexMinAddress = NULL;

typedef void (FX_CALL *tpGlideInit) ();

static tpGlideInit pgrGlideInit = NULL;

typedef FxBool (FX_CALL *tpQueryHardware) (
                 GrHwConfiguration *hwconfig);
static tpQueryHardware pgrSstQueryHardware = NULL;

typedef void (FX_CALL *tpSelect) (int);
static tpSelect pgrSstSelect = NULL;

typedef FxBool (FX_CALL *tpWinOpen) (
          FxU32                hWnd,
          GrScreenResolution_t screen_resolution,
          GrScreenRefresh_t    refresh_rate,
          GrColorFormat_t      color_format,
          GrOriginLocation_t   origin_location,
          int                  nColBuffers,
          int                  nAuxBuffers);
static tpWinOpen pgrSstWinOpen = NULL;

typedef void (FX_CALL *tpBufferClear) (
               GrColor_t color, GrAlpha_t alpha, FxU16 depth);
static tpBufferClear pgrBufferClear = NULL;

typedef FxU32 (FX_CALL *tpScreenWidth) ();
static tpScreenWidth pgrSstScreenWidth = NULL;

typedef FxU32 (FX_CALL *tpScreenHeight) ();
static tpScreenHeight  pgrSstScreenHeight = NULL;

typedef void (FX_CALL *tpGlideShutdown) ();
static tpGlideShutdown pgrGlideShutdown = NULL;

typedef void (FX_CALL *tpTexDownloadMipMapLevel) (
               GrChipID_t        tmu,
               FxU32             startAddress,
               GrLOD_t           thisLod,
               GrLOD_t           largeLod,
               GrAspectRatio_t   aspectRatio,
               GrTextureFormat_t format,
               FxU32             evenOdd,
               void              *data );

static tpTexDownloadMipMapLevel pgrTexDownloadMipMapLevel = NULL;

typedef void (FX_CALL *tpTexSource) (
               GrChipID_t tmu,
               FxU32      startAddress,
               FxU32      evenOdd,
               GrTexInfo  *info );
static tpTexSource pgrTexSource = NULL;

typedef void (FX_CALL *tpDrawTriangle) (
               const GrVertex *a,
               const GrVertex *b,
               const GrVertex *c );
static tpDrawTriangle pgrDrawTriangle = NULL;

typedef void (FX_CALL *tpBufferSwap) (
               int swap_interval);
static tpBufferSwap pgrBufferSwap = NULL;

static void S9xGlideInitTextures ()
{
    (*pgrColorCombine) (GR_COMBINE_FUNCTION_SCALE_OTHER,
                        GR_COMBINE_FACTOR_ONE,
                        GR_COMBINE_LOCAL_NONE,
                        GR_COMBINE_OTHER_TEXTURE,
                        FXFALSE);

    (*pgrTexCombine) (GR_TMU0,
                      GR_COMBINE_FUNCTION_LOCAL,
                      GR_COMBINE_FACTOR_NONE,
                      GR_COMBINE_FUNCTION_NONE,
                      GR_COMBINE_FACTOR_NONE,
                      FXFALSE,
                      FXFALSE);

    (*pgrTexMipMapMode) (GR_TMU0,
                         GR_MIPMAP_DISABLE,
                         FXFALSE);

    (*pgrTexClampMode) (GR_TMU0,
                        GR_TEXTURECLAMP_CLAMP,
                        GR_TEXTURECLAMP_CLAMP);

    (*pgrTexFilterMode) (GR_TMU0, GR_TEXTUREFILTER_BILINEAR,
                         GR_TEXTUREFILTER_BILINEAR);

    texture_info.smallLod = GR_LOD_256;
    texture_info.largeLod = GR_LOD_256;
    texture_info.aspectRatio = GR_ASPECT_1x1;
    texture_info.format = GR_TEXFMT_RGB_565;

    Glide.texture.smallLod = GR_LOD_8;
    Glide.texture.largeLod = GR_LOD_8;
    Glide.texture.aspectRatio = GR_ASPECT_1x1;
    Glide.texture.format = GR_TEXFMT_RGB_565;
    Glide.texture_mem_size = (*pgrTexTextureMemRequired) (GR_MIPMAPLEVELMASK_BOTH,
						      &Glide.texture);
    Glide.texture_mem_start = (*pgrTexMinAddress) (GR_TMU0);

    int texture_mem_size = (*pgrTexTextureMemRequired) (GR_MIPMAPLEVELMASK_BOTH,
						    &texture_info);
    int address = (*pgrTexMinAddress) (GR_TMU0);

    // At maximum SNES resolution (512x478) four seperate Voodoo textures will
    // be needed since the maximum texture size on Voodoo cards is 256x256.

    for (int t = 0; t < 4; t++)
    {
	texture_mem_start [t] = address;
	address += texture_mem_size;
    }

    texture_download_buffer = (uint16 *) malloc (texture_mem_size);

    for (int i = 0; i < 4; i++)
    {
	Glide.sq [i].oow = 1.0;
	Glide.sq [i].ooz = 65535.0;
	Glide.sq [i].tmuvtx [0].oow = 1.0;
    }

    Glide.x_offset = 0.0;
    Glide.y_offset = 0.0;
    // XXX: Do this when the SNES screen resolution is known.
    Glide.x_scale = (double) (Glide.voodoo_width * 8.0 / 256);
    Glide.y_scale = (double) (Glide.voodoo_height * 8.0 / 224);
}

static HMODULE GlideDLL = NULL;

bool8 S9xVoodooInitialise ()
{
    static bool8 init = FALSE;

    if (!init)
    {
        init = TRUE;
        if (!(GlideDLL = LoadLibrary ("glide2x.dll")))
        {
            MessageBox (GUI.hWnd, "\
Snes9x can't find the \"glide2x.dll\" dynamic-link library.\n\n\
Are you sure you actually have a Voodoo 3dfx graphics card inside your PC?",
                        "Snes9x - Missing Library(glide2x.dll) Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }

        pgrColorCombine = (tpColorCombine) GetProcAddress (GlideDLL, "_grColorCombine@20");
        pgrTexCombine = (tpTexCombine) GetProcAddress (GlideDLL, "_grTexCombine@28");
        pgrTexMipMapMode = (tpTexMipMapMode) GetProcAddress (GlideDLL, "_grTexMipMapMode@12");
        pgrTexClampMode = (tpTexClampMode) GetProcAddress (GlideDLL, "_grTexClampMode@12");
        pgrTexFilterMode = (tpTexFilterMode) GetProcAddress (GlideDLL, "_grTexFilterMode@12");
        pgrTexTextureMemRequired = (tpTexTextureMemRequired) GetProcAddress (GlideDLL, "_grTexTextureMemRequired@8");
        pgrTexMinAddress = (tpTexMinAddress) GetProcAddress (GlideDLL, "_grTexMinAddress@4");
        pgrGlideInit = (tpGlideInit) GetProcAddress (GlideDLL, "_grGlideInit@0");
        pgrSstQueryHardware = (tpQueryHardware) GetProcAddress (GlideDLL, "_grSstQueryHardware@4");
        pgrSstSelect = (tpSelect) GetProcAddress (GlideDLL, "_grSstSelect@4");
        pgrSstWinOpen = (tpWinOpen) GetProcAddress (GlideDLL, "_grSstWinOpen@28");
        pgrBufferClear = (tpBufferClear) GetProcAddress (GlideDLL, "_grBufferClear@12");
        pgrSstScreenWidth = (tpScreenWidth) GetProcAddress (GlideDLL, "_grSstScreenWidth@0");
        pgrSstScreenHeight = (tpScreenHeight) GetProcAddress (GlideDLL, "_grSstScreenHeight@0");
        pgrGlideShutdown = (tpGlideShutdown) GetProcAddress (GlideDLL, "_grGlideShutdown@0");
        pgrTexDownloadMipMapLevel = (tpTexDownloadMipMapLevel) GetProcAddress (GlideDLL, "_grTexDownloadMipMapLevel@32");
        pgrTexSource = (tpTexSource) GetProcAddress (GlideDLL, "_grTexSource@16");
        pgrDrawTriangle = (tpDrawTriangle) GetProcAddress (GlideDLL, "_grDrawTriangle@12");
        pgrBufferSwap = (tpBufferSwap) GetProcAddress (GlideDLL, "_grBufferSwap@4");
        //pgr = (tp) GetProcAddress (GlideDLL, "gr");

        if (!pgrColorCombine ||
            !pgrTexCombine ||
            !pgrTexMipMapMode ||
            !pgrTexClampMode ||
            !pgrTexFilterMode ||
            !pgrTexTextureMemRequired ||
            !pgrTexMinAddress ||
            !pgrGlideInit ||
            !pgrSstQueryHardware ||
            !pgrSstSelect ||
            !pgrSstWinOpen ||
            !pgrBufferClear ||
            !pgrSstScreenWidth ||
            !pgrSstScreenHeight ||
            !pgrGlideShutdown ||
            !pgrTexDownloadMipMapLevel ||
            !pgrTexSource ||
            !pgrDrawTriangle ||
            !pgrBufferSwap)
        {
            MessageBox (GUI.hWnd, "\
Snes9x found the \"glide2x.dll\" library it was looking for\n\
to enable 3dfx support, but it seems to be missing several\n\
important functions.\n\n\
3dfx support cannot be enabled, sorry.",
                        "Snes9x - Missing Symbols in Library (glide2x.dll) Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }
        (*pgrGlideInit) ();

        GrHwConfiguration hwconfig;
        Glide.voodoo_present = (*pgrSstQueryHardware) (&hwconfig);
        if (!Glide.voodoo_present)
        {
            MessageBox (GUI.hWnd, "\
Snes9x found the \"glide2x.dll\" library it was looking for\n\
to enable 3dfx support, but the library refused to initialise\n\
correctly.\n\n\
3dfx support cannot be enabled, sorry.",
                        "Snes9x - GLIDE Initialisation Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }
    }

    return (Glide.voodoo_present);
}

bool8 S9xGlideInit ()
{
    static GrHwConfiguration hwconfig;
    // XXX:
    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    GrScreenRefresh_t refresh_rate = GR_REFRESH_75Hz;

    if ((*pgrSstQueryHardware) (&hwconfig))
    {
        FxU32 hwnd = (FxU32) GUI.hWnd;

        if (hwconfig.SSTs [0].type == GR_SSTTYPE_SST96 ||
            hwconfig.SSTs [0].type >GR_SSTTYPE_Voodoo2)
        {
            resolution = GR_RESOLUTION_NONE;
        }
	(*pgrSstSelect) (0);

	if ((*pgrSstWinOpen) (hwnd, resolution, refresh_rate,
                              GR_COLORFORMAT_ABGR,
                              GR_ORIGIN_UPPER_LEFT, 2, 1))
	{
	    (*pgrBufferClear) (0, 0, GR_ZDEPTHVALUE_FARTHEST);
	    Glide.voodoo_width = (double) (*pgrSstScreenWidth) ();
	    Glide.voodoo_height = (double) (*pgrSstScreenHeight) ();
	    S9xGlideInitTextures ();
	    return (TRUE);
	}
    }

    Glide.voodoo_present = FALSE;
    return (FALSE);
}

void S9xGlideDeinit ()
{
    free ((char *) texture_download_buffer);
	texture_download_buffer = NULL;
    (*pgrGlideShutdown) ();
}

void S9xGlidePutImage (int snes_width, int snes_height)
{
    GrVertex square [4];

    Glide.voodoo_width = (double) (*pgrSstScreenWidth) ();
    Glide.voodoo_height = (double) (*pgrSstScreenHeight) ();

    //(*pgrBufferClear) (0, 0, GR_ZDEPTHVALUE_FARTHEST);

    if (snes_width <= TEXTURE_SIZE && snes_height <= TEXTURE_SIZE)
    {
	// Special case 256x224 and 256x239 SNES resolutions.
	texture_info.data = (void *) GFX.Screen;

	(*pgrTexDownloadMipMapLevel) (GR_TMU0, texture_mem_start [0],
				  GR_LOD_256, GR_LOD_256, GR_ASPECT_1x1,
				  GR_TEXFMT_RGB_565,
				  GR_MIPMAPLEVELMASK_BOTH,
				  (void *) GFX.Screen);

	(*pgrTexSource) (GR_TMU0, texture_mem_start [0],
		     GR_MIPMAPLEVELMASK_BOTH, &texture_info);

	S9xGlideCalcSquare (snes_width, snes_height, 0, 0,
			    snes_width, snes_height, square);
	(*pgrDrawTriangle) (&square [0], &square [3], &square [2]);
	(*pgrDrawTriangle) (&square [0], &square [1], &square [2]);
    }
    else
    {
	// Have to chop the rendered SNES screen into a maximum 256x256 size
	// chunks because that's all the Voodoo card can cope with.
	int t = 0;
	int vert_squares = (snes_height + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;
	int horiz_squares = (snes_width + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;

	texture_info.data = (void *) texture_download_buffer;

	for (int y = 0; y < vert_squares; y++)
	{
	    for (int x = 0; x < horiz_squares; x++, t++)
	    {
		int square_height = y != vert_squares - 1 ? TEXTURE_SIZE :
				    snes_height - (TEXTURE_SIZE * y);
		int square_width = x != horiz_squares - 1 ? TEXTURE_SIZE :
				   snes_width - (TEXTURE_SIZE * x);
		int offset = x * TEXTURE_SIZE * sizeof (uint16) +
			     y * TEXTURE_SIZE * GFX.Pitch;

		for (int l = 0; l < square_height; l++)
		{
		    memmove ((void *) (texture_download_buffer + l * 256),
			     GFX.Screen + l * GFX.RealPitch + offset,
			     sizeof (uint16) * 256);
		}
		(*pgrTexDownloadMipMapLevel) (GR_TMU0, texture_mem_start [t],
					  GR_LOD_256, GR_LOD_256,
					  GR_ASPECT_1x1,
					  GR_TEXFMT_RGB_565,
					  GR_MIPMAPLEVELMASK_BOTH,
					  (void *) texture_download_buffer);

		(*pgrTexSource) (GR_TMU0, texture_mem_start [t],
                                 GR_MIPMAPLEVELMASK_BOTH, &texture_info);

		S9xGlideCalcSquare (snes_width, snes_height, x, y,
				    square_width, square_height,
				    square);
		(*pgrDrawTriangle) (&square [0], &square [3], &square [2]);
		(*pgrDrawTriangle) (&square [0], &square [1], &square [2]);
	    }
	}
    }

    (*pgrBufferSwap) (1);
}

bool8 S9xGlideEnable (bool8 enable)
{
    static uint32 previous_graphic_format = RGB565;
    extern uint32 current_graphic_format;

    if (Settings.GlideEnable != enable)
    {
        WINDOWPLACEMENT place;
        place.length = sizeof (WINDOWPLACEMENT);
        GetWindowPlacement (GUI.hWnd, &place);

	if (enable)
	{
	    if (S9xGlideInit ())
	    {
		previous_graphic_format = current_graphic_format;
		S9xGraphicsDeinit ();
		S9xSetRenderPixelFormat (RGB565);
		S9xInitUpdate();
		S9xGraphicsInit ();
		Settings.GlideEnable = TRUE;
                if (place.showCmd == SW_SHOWNORMAL)
                {
                    place.showCmd = SW_SHOWMAXIMIZED;
                    SetWindowPlacement (GUI.hWnd, &place);
                }
	    }
	}
	else
	{
	    Settings.GlideEnable = FALSE;
	    S9xGlideDeinit ();
	    S9xSetRenderPixelFormat (previous_graphic_format);
	    S9xGraphicsDeinit ();
		S9xInitUpdate();
	    S9xGraphicsInit ();
            if (place.showCmd == SW_SHOWMAXIMIZED && !VOODOO_MODE)
            {
                place.showCmd = SW_SHOWNORMAL;
                SetWindowPlacement (GUI.hWnd, &place);
            }
	}
    }
    return (TRUE);
}
#endif

void RenderGlide (SSurface Src, SSurface Dst, RECT *rect)
{
    //rect->left = 0;
    //rect->right = DirectX.Width;
    //rect->top = 0;
    //rect->bottom = DirectX.Height;

#ifdef USE_GLIDE
    if (Glide.voodoo_present && !Settings.GlideEnable)
        S9xGlideEnable (TRUE);

    if (Settings.GlideEnable && Glide.voodoo_present)
        S9xGlidePutImage (Src.Width, Src.Height);
#endif
}

#ifdef USE_OPENGL
#define RED(color) ((color&0x7C00) >> 7)
#define GREEN(color) ((color&0x03E0) >> 2)
#define BLUE(color) ((color&0x001F) << 3 )

BYTE basetexbuffer [512 * 512 * 3];

GLdouble	xrot;			// X Rotation
GLdouble	yrot;			// Y Rotation
GLdouble	zrot;			// Z Rotation
#endif

void RenderOpenGL (SSurface Src, SSurface Dst, RECT *rect)
{
    //rect->left = 0;
    //rect->right = DirectX.Width;
    //rect->top = 0;
    //rect->bottom = DirectX.Height;

#ifdef USE_OPENGL
    int width, height;

    RECT r;
    GetClientRect (GUI.hWnd, &r);

	OffsetRect(&r, -r.left, -r.top);

	if(GUI.AspectRatio)
	{
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		int oldWidth = SNES_WIDTH;
		int oldHeight = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
		int newWidth, newHeight;

		if(oldWidth * height > oldHeight * width)
		{
			newWidth = oldWidth*width/oldWidth;
			newHeight = oldHeight*width/oldWidth;
		}
		else
		{
			newWidth = oldWidth*height/oldHeight;
			newHeight = oldHeight*height/oldHeight;
		}
		int xOffset = (width - newWidth)/2;
		int yOffset = (height - newHeight)/2;

		r.right = r.left + newWidth;
		r.bottom = r.top + newHeight;

		OffsetRect(&r, xOffset, yOffset);
	}

    width = r.right - r.left;
    height = r.bottom - r.top;

    glViewport (r.left, r.top, width, height);

///	glViewport (r.left, r.top, r.right, r.bottom);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    glFrustum (-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
    glMatrixMode (GL_MODELVIEW);

//    extern GLuint texture[1];
	    uint8 *data = (uint8 *) Src.Surface;

    uint32 swidth = Src.Width > 256 ? 512 : 256;
    int startl = 0;

    int texture_size;

    if (OpenGL.num_textures == 2 && Src.Width == 256)
        texture_size = 256;
    else
        texture_size = OpenGL.texture_size;

	uint32 srcheight = Src.Height - (GUI.HeightExtend?0:15);
    uint32 pheight = (srcheight) % texture_size;
    if (pheight == 0)
        pheight = texture_size;

    if (OpenGL.draw_cube)
    {
        xrot += 0.3f;
        yrot += 0.2f;
        zrot += 0.4f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
	else
	{
		if(GUI.AspectRatio /*|| !GUI.Stretch*/)
	        glClear(GL_COLOR_BUFFER_BIT);
	}

    for (int starty = (srcheight) - pheight; starty >= 0;
	startl += pheight, pheight = texture_size, starty -= texture_size)
    {
        for (int startx = 0; startx < (int) swidth; startx += texture_size)
        {
            uint32 pwidth;
            if ((pwidth = Src.Width - startx) > (uint32) texture_size)
                pwidth = texture_size;

            double fx1 = (double) startx / Src.Width;
            double fy1 = (double) starty / (srcheight);
            double fx2 = fx1 + (double) pwidth / Src.Width;
            double fy2 = fy1 + (double) (pheight+1) / (srcheight);

            if (!OpenGL.packed_pixels_extension_present)
            {
                int i = 0;
                for (uint32 y = 0; y < pheight; y++)
                {
                    uint16 *ptr = (uint16 *) (GFX.Screen + (y + startl) * GFX.RealPPL) + startx;
                    for (uint32 x = 0; x < pwidth; x++)
                    {
                        int color = *ptr++;
                        basetexbuffer [i  ] = RED(color);
                        basetexbuffer [i+1] = GREEN(color);
                        basetexbuffer [i+2] = BLUE(color);
                        i += 3;
                    }
                }
                data = basetexbuffer;
            }
			else if (Src.Width > (unsigned int)texture_size)
			{
				for (uint32 y = 0; y < pheight; y++)
				{
					/*memmove*/ MoveMemory (basetexbuffer + sizeof (uint16) * texture_size * y,
						(GFX.Screen + (y + startl) * GFX.RealPPL) + startx,
						sizeof (uint16) * texture_size);
				}
				data = basetexbuffer;
			}

			if (OpenGL.num_textures == 2 && Src.Width == 256)
				glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
			else
				glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);

			glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, pwidth, pheight,
				OpenGL.format, OpenGL.type, data);

			double maxx = (double) (pwidth - 1) / texture_size;
			double maxy = (double) (pheight - 1) / texture_size;

			if (OpenGL.draw_cube)
			{
				glLoadIdentity();
				glTranslatef (0.0f, 0.0f, -3.5f);

				glRotatef (xrot, 1.0f, 0.0f, 0.0f);
				glRotatef (yrot, 0.0f, 1.0f, 0.0f);
				glRotatef (zrot, 0.0f, 0.0f, 1.0f);

				glBegin(GL_QUADS);
				{
					// Front Face
					glNormal3f (0.0f, 0.0f, 1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
					// Back Face
					glNormal3f (0.0f, 0.0f, -1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
					// Top Face
					glNormal3f (0.0f, 1.0f, 0.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f, 1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					// Bottom Face
					glNormal3f (0.0f, -1.0f, 0.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f,-1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					// Right face
					glNormal3f (1.0f, 0.0f, 0.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					// Left Face
					glNormal3f (-1.0f, 0.0f, 0.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
				}
				glEnd();
			}
			else
			{
				glLoadIdentity ();
				glTranslatef (-0.5, -0.5, -1.0);
				glDisable (GL_DEPTH_TEST);

				glBegin(GL_TRIANGLE_STRIP);
				{
					glTexCoord2f (0.0, maxy);
					glVertex3f (fx1, fy1, 0.0);

					glTexCoord2f (maxx, maxy);
					glVertex3f (fx2, fy1, 0.0);

					glTexCoord2f (0.0, 0.0);
					glVertex3f (fx1, fy2, 0.0);

					glTexCoord2f (maxx, 0.0);
					glVertex3f (fx2, fy2, 0.0);
				}
				glEnd();
			}
        }
    }

    SwapBuffers (GUI.hDC);
#endif
}



