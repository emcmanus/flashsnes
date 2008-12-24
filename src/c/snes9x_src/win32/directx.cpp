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



// DirectX.cpp: implementation of the CDirectX class.
//
//////////////////////////////////////////////////////////////////////

#include "wsnes9x.h"
#include "../snes9x.h"
#include "../soundux.h"
#include "directx.h"

#ifdef FMOD_SUPPORT
#include "fmod.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDirectX::CDirectX()
{
    lpDD = NULL;
    lpDDClipper = NULL;
    lpDDPalette = NULL;

    lpDDSPrimary2 = NULL;
    lpDDSOffScreen2 = NULL;

    Width = Height = -1;
    Depth = -1;
    DoubleBuffered = false;

    lpDS = NULL;
    lpDSB = NULL;
    lpDSBPrimary = NULL;

    DSAvailable = false;
}

CDirectX::~CDirectX()
{
    DeInitialize();
}

bool CDirectX::InitDirectX ()
{
	static bool initialized = false;
	if(initialized)
		return true;

    dErr = DirectDrawCreate (NULL, &lpDD, NULL);
    if(FAILED(dErr))
        return false;

    dErr = lpDD -> CreateClipper (0, &lpDDClipper, NULL);
    if(FAILED(dErr))
        return false;

    dErr = lpDDClipper->SetHWnd (0, GUI.hWnd);
    if(FAILED(dErr))
        return false;

	initialized = true;

    return (true);
}

bool CDirectX::InitDirectSound ()
{
    DSAvailable = true;

#ifdef FMOD_SUPPORT
    if (Settings.SoundDriver >= WIN_FMOD_DIRECT_SOUND_DRIVER)
    {
        FSOUND_GetVersion ();

        switch (Settings.SoundDriver)
        {
            default:
            case WIN_FMOD_DIRECT_SOUND_DRIVER:
                FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
                break;
            case WIN_FMOD_WAVE_SOUND_DRIVER:
                FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
                break;
            case WIN_FMOD_A3D_SOUND_DRIVER:
                FSOUND_SetOutput(FSOUND_OUTPUT_A3D);
                break;
        }

        FSOUND_GetOutput ();
        for (int i = 0; i < FSOUND_GetNumDrivers (); i++)
            FSOUND_GetDriverName (i);
        FSOUND_SetDriver(0);
    }
    else
    {
#endif
        if (!lpDS)
        {
            dErr = DirectSoundCreate (NULL, &lpDS, NULL);
            if (dErr != DS_OK)
            {
                DSAvailable = false;
                MessageBox (GUI.hWnd, TEXT("\
Unable to initialise DirectSound. You will not be able to hear any\n\
sound effects or music while playing.\n\n\
It is usually caused by not having DirectX installed, another\n\
application that has already opened DirectSound in exclusive\n\
mode or the Windows WAVE device has been opened."),
                            TEXT("Snes9X - Unable to Open DirectSound"),
                            MB_OK | MB_ICONWARNING);
                return (false);
            }
        }
		dErr = lpDS->SetCooperativeLevel (GUI.hWnd, DSSCL_PRIORITY | DSSCL_EXCLUSIVE);
        if (!SUCCEEDED(dErr))
        {
			dErr = lpDS->SetCooperativeLevel (GUI.hWnd, DSSCL_PRIORITY);
			if (!SUCCEEDED(dErr))
			{
				if (!SUCCEEDED(lpDS -> SetCooperativeLevel (GUI.hWnd, DSSCL_NORMAL)))
				{
					lpDS -> Release();
					lpDS = NULL;

					DSAvailable = false;
				}
				if (DSAvailable)
					MessageBox (GUI.hWnd, TEXT("\
	Unable to set DirectSound's  priority cooperative level.\n\
	Another application is dicating the sound playback rate,\n\
	sample size and mono/stereo setting."),
						TEXT("Snes9X - Unable to Set DirectSound priority"),
								MB_OK | MB_ICONWARNING);
				else
					MessageBox (GUI.hWnd, TEXT("\
	Unable to set any DirectSound cooperative level. You will\n\
	not be able to hear any sound effects or music while playing.\n\n\
	It is usually caused by another application that has already\n\
	opened DirectSound in exclusive mode."),
						TEXT("Snes9X - Unable to DirectSound"),
								MB_OK | MB_ICONWARNING);
			}
		}
#ifdef FMOD_SUPPORT
    }
#endif

    return (true);
}

void CDirectX::DeInitialize()
{
    if (lpDD != NULL)
    {
        if (lpDDSPrimary2 != NULL)
        {
            lpDDSPrimary2->Release();
            lpDDSPrimary2 = NULL;
        }
        if (lpDDSOffScreen2 != NULL)
        {
            lpDDSOffScreen2->PageUnlock(0);
            lpDDSOffScreen2->Release();
            lpDDSOffScreen2 = NULL;
        }
        if (lpDDClipper != NULL)
        {
            lpDDClipper->Release();
            lpDDClipper = NULL;
        }
        if (lpDDPalette != NULL)
        {
            lpDDPalette->Release();
            lpDDPalette = NULL;
        }
        lpDD->Release();
        lpDD = NULL;
    }

    if( lpDS != NULL)
    {
        if( lpDSB != NULL)
        {
            lpDSB->Stop ();
            lpDSB->Release();
            lpDSB = NULL;
        }
        if( lpDSBPrimary != NULL)
        {
            lpDSBPrimary->Stop ();
            lpDSBPrimary->Release();
            lpDSBPrimary = NULL;
        }
        lpDS->SetCooperativeLevel (GUI.hWnd, DSSCL_NORMAL);
        lpDS->Release ();
        lpDS = NULL;
    }
}

bool CDirectX::SetDisplayMode(
		int pWidth, int pHeight, int pScale,
		char pDepth, int pRefreshRate, bool pWindowed, bool pDoubleBuffered)
{
	if(pScale < 2) pScale = 2;

    static bool BLOCK = false;
    DDSURFACEDESC ddsd;
    PALETTEENTRY PaletteEntries [256];

    if (BLOCK)
        return (false);

    BLOCK = true;

    if (pWindowed)
        pDoubleBuffered = false;

    if (pDepth == 0)
        pDepth = Depth;

    if (lpDDSPrimary2 != NULL)
    {
        lpDDSPrimary2->Release();
        lpDDSPrimary2 = NULL;
    }
    if (lpDDSOffScreen2 != NULL)
    {
        lpDDSOffScreen2->PageUnlock(0);
        lpDDSOffScreen2->Release();
        lpDDSOffScreen2 = NULL;
    }
    if( lpDDPalette != NULL)
    {
        lpDDPalette->Release();
        lpDDPalette = NULL;
    }

    lpDD->FlipToGDISurface();

    if (pWindowed)
    {
        lpDD->RestoreDisplayMode();

        SetWindowLong( GUI.hWnd, GWL_STYLE, WS_POPUPWINDOW|WS_CAPTION|
                       WS_THICKFRAME|WS_VISIBLE|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

		// disabled because it messes up window maximization
        //if (!VOODOO_MODE)
        //    SetWindowPos( GUI.hWnd, HWND_TOP, 0, 0, 0, 0,
        //                  SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);
        //else
        //    SetWindowPos( GUI.hWnd, HWND_TOP, 0, 0, 0, 0,
        //                  SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);

        ZeroMemory (&ddsd, sizeof (ddsd));

        ddsd.dwSize = sizeof (ddsd);
        ddsd.dwFlags = DDSD_PIXELFORMAT;
        dErr = lpDD->GetDisplayMode (&ddsd);
	    if (FAILED(dErr))
            pDepth = 8;
        else
        {
            if (ddsd.ddpfPixelFormat.dwFlags&DDPF_RGB)
                pDepth = (char) ddsd.ddpfPixelFormat.dwRGBBitCount;
            else
                pDepth = 8;
        }
        if (pDepth == 8)
            dErr = lpDD->SetCooperativeLevel (GUI.hWnd, DDSCL_FULLSCREEN|
                                              DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);
        else
            dErr = lpDD->SetCooperativeLevel (GUI.hWnd, DDSCL_NORMAL|DDSCL_ALLOWREBOOT);
    }
    else
    {
        SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUP|WS_VISIBLE);

        if (!VOODOO_MODE)
        {
            SetWindowPos (GUI.hWnd, HWND_TOP, 0, 0, 0, 0, SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);
            dErr = lpDD->SetCooperativeLevel (GUI.hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT);
			// XXX: TODO: use pRefreshRate!
            dErr = lpDD->SetDisplayMode (pWidth, pHeight, pDepth);
        }
        else
        {
            SetWindowPos (GUI.hWnd, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);
            dErr = lpDD->SetCooperativeLevel (GUI.hWnd, DDSCL_NORMAL|DDSCL_ALLOWREBOOT);
        }
    }

	if (FAILED(dErr))
    {
        BLOCK = false;
        return false;
    }

    ZeroMemory (&ddsd, sizeof (ddsd));
    ddsd.dwSize = sizeof (ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if(GUI.VideoMemory)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | (GUI.LocalVidMem ? DDSCAPS_LOCALVIDMEM : DDSCAPS_NONLOCALVIDMEM);
	}
	else
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	}
    ddsd.dwWidth = SNES_WIDTH * pScale;
    ddsd.dwHeight = SNES_HEIGHT_EXTENDED * pScale;

    LPDIRECTDRAWSURFACE lpDDSOffScreen;
    if (FAILED(lpDD->CreateSurface (&ddsd, &lpDDSOffScreen, NULL)))
    {
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | (GUI.LocalVidMem ? DDSCAPS_NONLOCALVIDMEM : DDSCAPS_LOCALVIDMEM);
		if(!GUI.VideoMemory || FAILED(lpDD->CreateSurface (&ddsd, &lpDDSOffScreen, NULL)))
		{
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if(!GUI.VideoMemory || FAILED(lpDD->CreateSurface (&ddsd, &lpDDSOffScreen, NULL)))
			{
				BLOCK = false;
				return (false);
			}
		}
    }

    if (FAILED (lpDDSOffScreen->QueryInterface (IID_IDirectDrawSurface2,
                                                (void **)&lpDDSOffScreen2)))
    {
        lpDDSOffScreen->Release();
        BLOCK = false;
        return (false);
    }
    lpDDSOffScreen2->PageLock(0);
    lpDDSOffScreen->Release();

    ZeroMemory (&ddsd, sizeof (ddsd));
    if (pDoubleBuffered)
    {
        ddsd.dwSize = sizeof( ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
        GUI.NumFlipFrames = 3;
        ddsd.dwBackBufferCount = 2;
    }
    else
    {
        GUI.NumFlipFrames = 1;
        ddsd.dwSize = sizeof (ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    }

    LPDIRECTDRAWSURFACE lpDDSPrimary;

    dErr = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL);
    if( FAILED(dErr) )
    {
        if (pDoubleBuffered)
        {
            ddsd.dwBackBufferCount = 1;
            GUI.NumFlipFrames = 2;
            if (FAILED(dErr = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL)))
            {
                ddsd.dwFlags = DDSD_CAPS;
                ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

                pDoubleBuffered = false;
                GUI.NumFlipFrames = 1;
                dErr = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL);
            }
    	}

        if (FAILED(dErr))
        {
            BLOCK = false;
            lpDDSOffScreen2->PageUnlock(0);
            lpDDSOffScreen2->Release();
            lpDDSOffScreen2 = NULL;

            return (false);
        }
    }

    ZeroMemory (&DDPixelFormat, sizeof (DDPixelFormat));
    DDPixelFormat.dwSize = sizeof (DDPixelFormat);
    lpDDSPrimary->GetPixelFormat (&DDPixelFormat);

	Clipped = true;
    if((!pWindowed && pDoubleBuffered) || FAILED(lpDDSPrimary->SetClipper( lpDDClipper)))
		Clipped = false;

    if (FAILED (lpDDSPrimary->QueryInterface (IID_IDirectDrawSurface2, (void **)&lpDDSPrimary2)))
    {
        BLOCK = false;
        lpDDSPrimary->Release();
        lpDDSPrimary = NULL;

        return (FALSE);
    }

    lpDDSPrimary->Release();
    lpDDSPrimary = NULL;

    if((!pWindowed && pDoubleBuffered) || FAILED(lpDDSPrimary2->SetClipper( lpDDClipper)))
		Clipped = false;

    if (pDepth == 8)
    {
        dErr = lpDD->CreatePalette (DDPCAPS_8BIT | DDPCAPS_ALLOW256,
                                    PaletteEntries, &lpDDPalette, NULL);
        if( FAILED(dErr))
        {
            lpDDPalette = NULL;
            BLOCK = false;
            return false;
        }
    }

    Depth = pDepth;
    Height = pHeight;
    Width = pWidth;
    DoubleBuffered = pDoubleBuffered;
    BLOCK = false;

    return (true);
}

void CDirectX::GetPixelFormat ()
{
    if (lpDDSPrimary2)
    {
        ZeroMemory (&DDPixelFormat, sizeof (DDPixelFormat));
        DDPixelFormat.dwSize = sizeof (DDPixelFormat);
        lpDDSPrimary2->GetPixelFormat (&DDPixelFormat);
    }
}

#ifdef FMOD_SUPPORT
// The FMOD API changed the return type of the stream callback function
// somewhere between version 3.20 and 3.33. The FMOD API defines a version
// string but you can't test for that at compile time. Instead, I've picked on
// a symbol that wasn't defined in version 3.20 to test for the change in API.
#if !defined (FSOUND_LOADRAW)
void
#else
signed char
#endif
F_CALLBACKAPI FMODStreamCallback (FSOUND_STREAM *stream, void *buff, int len, void *param)
{
	bool8 mute = DirectX.IdleCount >= GUI.PausedFramesBeforeMutingSound;
    if (mute || Settings.ForcedPause ||
        (Settings.Paused && (GUI.FAMute || !Settings.FrameAdvance && !(DirectX.IdleCount < 8))) // gives frame advance sound
		|| Settings.StopEmulation || GUI.AVIOut)
    {
        ZeroMemory (buff, len);
    }
    else
    {
        int sample_count = len;

		//MK: Sixteenbit was a rendering variable!!
        if (Settings.SixteenBitSound)
            sample_count /= 2;
        S9xMixSamples ((unsigned char *) buff, sample_count);

        if (Settings.Mute)
            ZeroMemory (buff, len);
    }
#if defined (FSOUND_LOADRAW)
    return (1);
#endif
}
#endif

extern long _buffernos;
extern long _blocksize;
extern long _buffersize;
extern long _samplecount;
extern long _bytecount;
extern unsigned long _interval;

bool CDirectX::SetSoundMode( void)
{
#ifdef FMOD_SUPPORT
    static FSOUND_STREAM *fmod_stream = NULL;
#endif
    if (lpDS != NULL)
    {
        if (lpDSB != NULL)
        {
            lpDSB->Stop ();
            lpDSB->Release();
            lpDSB = NULL;
        }
        if (lpDSBPrimary != NULL)
        {
            lpDSBPrimary->Stop ();
            lpDSBPrimary->Release();
            lpDSBPrimary = NULL;
        }
        lpDS->SetCooperativeLevel (GUI.hWnd, DSSCL_NORMAL);
        lpDS->Release ();
        lpDS = NULL;
    }

#ifdef FMOD_SUPPORT
    if (fmod_stream)
    {
        FSOUND_StopSound (0);
        FSOUND_Stream_Stop (fmod_stream);
        FSOUND_Stream_Close (fmod_stream);
        FSOUND_Close ();
        fmod_stream = NULL;
    }
#endif

    InitDirectSound ();

#ifdef FMOD_SUPPORT
    if (Settings.SoundDriver >= WIN_FMOD_DIRECT_SOUND_DRIVER)
    {
        if (Settings.SoundPlaybackRate == 0 || !Settings.APUEnabled || Settings.Mute)
            return false;

        if (!FSOUND_Init (Settings.SoundPlaybackRate, 16, 0))
        {
            DSAvailable = false;
            MessageBox (GUI.hWnd, "\
Unable to initialise FMOD sound system. You will not be able to hear\n\
any sound effects or music while playing.\n\n\
It is usually caused by not having DirectX installed or another\n\
application that has already opened DirectSound in exclusive\n\
mode or the Windows WAVE device has been opened.",
                        "Snes9X - Unable to Open FMOD",
                        MB_OK | MB_ICONWARNING);
        }

        int l = FSOUND_DSP_GetBufferLength ();
        if (Settings.SixteenBitSound)
            l *= 2;
        if (Settings.Stereo)
            l *= 2;
	fmod_stream = FSOUND_Stream_Create (FMODStreamCallback, l,
                                            FSOUND_LOOP_OFF |
                                            FSOUND_STREAMABLE |
                                            FSOUND_SIGNED |
                                            FSOUND_LOOP_NORMAL |
                                            (Settings.SixteenBitSound ?
                                               FSOUND_16BITS : FSOUND_8BITS) |
                                            (Settings.Stereo ?
                                               FSOUND_STEREO : FSOUND_MONO),
                                            Settings.SoundPlaybackRate, 0);
        so.playback_rate = Settings.SoundPlaybackRate;
        so.stereo = Settings.Stereo;
        so.sixteen_bit = Settings.SixteenBitSound;

	if (!fmod_stream ||
            FSOUND_Stream_Play (FSOUND_FREE, fmod_stream) == -1)
	{
            MessageBox (GUI.hWnd, "\
Unable to create or play an FMOD sound stream. You will not be able\n\
to hear any sound effects or music while playing.",
                        "Snes9X - Unable to Open FMOD",
                        MB_OK | MB_ICONWARNING);
	}
    }
    else
    {
#endif
        DSBUFFERDESC dsbd;

        if (Settings.SoundPlaybackRate == 0 || !Settings.APUEnabled || Settings.Mute ||!DSAvailable)
            return false;

        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = Settings.Stereo ? 2 : 1;
        wfx.nSamplesPerSec = Settings.SoundPlaybackRate;
        wfx.nBlockAlign = (Settings.SixteenBitSound ? 2 : 1) * (Settings.Stereo ? 2 : 1);
        wfx.wBitsPerSample = Settings.SixteenBitSound ? 16 : 8;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
        wfx.cbSize = 0;

        ZeroMemory (&dsbd, sizeof(DSBUFFERDESC) );
        dsbd.dwSize = sizeof(dsbd);
        dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_STICKYFOCUS;

        dErr = lpDS->CreateSoundBuffer (&dsbd, &lpDSBPrimary, NULL);
        if (dErr != DS_OK)
        {
            lpDSB = NULL;
            return (false);
        }

        lpDSBPrimary->SetFormat (&wfx);
        if (lpDSBPrimary->GetFormat (&wfx, sizeof (wfx), NULL) == DS_OK)
        {
            so.playback_rate = wfx.nSamplesPerSec;
            so.stereo = wfx.nChannels > 1;
            so.sixteen_bit = wfx.wBitsPerSample == 16;
        }

//        DSBCAPS caps;
//        caps.dwSize = sizeof (DSBCAPS);
//        lpDSBPrimary->GetCaps (&caps);

        lpDSBPrimary->Play (0, 0, DSBPLAY_LOOPING);

#if 0
        DWORD buffer_bytes = (200 * so.playback_rate *
                              (Settings.Stereo ? 2 : 1) *
                              (Settings.SixteenBitSound ? 2 : 1)) / 1000;
#else
        DWORD buffer_bytes = _blocksize * _buffernos;
#endif
        ZeroMemory (&dsbd, sizeof (dsbd));
        dsbd.dwSize = sizeof( dsbd);
        dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME |
	               DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STICKYFOCUS;
        dsbd.dwBufferBytes = buffer_bytes;
        dsbd.lpwfxFormat = &wfx;

        if (lpDS->CreateSoundBuffer (&dsbd, &lpDSB, NULL) != DS_OK)
        {
            lpDSBPrimary->Release ();
            lpDSBPrimary = NULL;
            lpDSB->Release();
            lpDSB = NULL;
            return (false);
        }

#if 0
        // Compute how far ahead, in milliseconds, the sound buffer write
        // position is compared to the play position.
        DWORD play_pos = 0, write_pos = 0;
        DWORD len, milli;

        lpDSB->Play (0, 0, DSBPLAY_LOOPING);
        if (lpDSB->GetCurrentPosition (&play_pos, &write_pos) != DS_OK)
            milli = 150;
        else
        {
            if (write_pos < play_pos)
                len = write_pos + buffer_bytes - play_pos;
            else
                len = write_pos - play_pos;

            milli = (1000 * len) / so.playback_rate;
            if (Settings.Stereo)
                milli /= 2;
            if (Settings.SixteenBitSound)
                milli /= 2;
        }
        lpDSB->Stop ();
        lpDSB->Release ();
        lpDSB = NULL;

        milli = (milli * 3) / 2;
        if (milli < _interval)
            milli = _interval;

        int s = (milli * so.playback_rate *
                 (Settings.Stereo ? 2 : 1) *
                 (Settings.SixteenBitSound ? 2 : 1)) / 1000;

        _blocksize = 64;
        while (_blocksize < s)
            _blocksize *= 2;

        ZeroMemory (&dsbd, sizeof (dsbd));
        dsbd.dwSize = sizeof (dsbd);
        dsbd.dwFlags = DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STICKYFOCUS;
        dsbd.dwBufferBytes = _blocksize * _buffernos;
        dsbd.lpwfxFormat = &wfx;

        if (lpDS->CreateSoundBuffer (&dsbd, &lpDSB, NULL) != DS_OK)
        {
            lpDSBPrimary->Release ();
            lpDSBPrimary = NULL;
            lpDSB->Release();
            lpDSB = NULL;
            return (false);
        }
        _buffersize = _blocksize * _buffernos;
#endif

#ifdef FMOD_SUPPORT
    }
#endif
    return (true);
}
