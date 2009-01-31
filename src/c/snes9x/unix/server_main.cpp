#include <stdio.h>
#include "cpuexec.h"
#include "snapshot.h"
#include "memmap.h"
#include "netplay.h"

extern struct SNPServer NPServer;

void CMemory::InitROM(void)
{
	// stub
}

char* freeze_file;

#include <signal.h>

void sighup_handler(int signal)
{
	/* Fortunately this is thread safe */
	S9xNPServerQueueSendingFreezeFile(freeze_file);
}

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		S9xTracef( "STDERR: s9xserver - Usage: s9xserver <port> <ROMName> <frametime> [freezefile]\n");
		S9xTracef( "STDERR:   <frametime> = 20000 (PAL)\n");
		S9xTracef( "STDERR:   <frametime> = 16667 (NTSC)\n");
		S9xTracef( "STDERR: \n");
		S9xTracef( "STDERR: Note: ROMName needs to match those of the clients.\n");
		exit(1);
	}
	strncpy(NPServer.ROMName, argv[2], 30);

	Settings.FrameTime=atoi(argv[3]);
	Settings.NetPlay=Settings.NetPlayServer=TRUE;

	Memory.SRAMSize = 0;

	if (argc > 4)
		freeze_file=argv[4];

	if (freeze_file)
	{
		NPServer.SyncByReset=FALSE;
		signal(SIGHUP, sighup_handler);
	}
	else
		NPServer.SyncByReset=TRUE;

	NPServer.SendROMImageOnConnect=FALSE;

	if (!S9xNPStartServer (atoi(argv[1])))
	{
		S9xTracef( "Print f: Server startup failed.\n");
		exit(1);
	}
	S9xTracef( "Print f: Server exited successfully.\n");
}

void S9xReset (void)
{
	// stub
}

bool8 S9xFreezeGame (const char *filename)
{
	uint8* data;
	uint32 len;
	FILE *ff;
	bool8 ok;

	if (!freeze_file)
		return FALSE;

        if (!(ff = fopen (freeze_file, "rb")))
		return FALSE;
	fseek (ff, 0, SEEK_END);
	len = ftell (ff);
	fseek (ff, 0, SEEK_SET);
	data = new uint8 [len];
	ok = (fread (data, 1, len, ff) == len);
	fclose (ff);

	if (!ok)
		return FALSE;

	if (!(ff = fopen (filename, "wb")))
		return FALSE;
	ok = (fwrite (data, 1, len, ff) == len);
	fclose (ff);

	return (ok);
}

bool8 S9xUnfreezeGame (const char *filename)
{
	(void*) filename = 0; // stub

	return FALSE;
}
#include <stdio.h>
#include "cpuexec.h"
#include "snapshot.h"
#include "memmap.h"
#include "netplay.h"

extern struct SNPServer NPServer;

void CMemory::InitROM(void)
{
	// stub
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		S9xTracef( "Print f: s9xserver - Usage: s9xserver <port> <ROMName> [frametime] [romfilename] [freezefile]\n");
		exit(1);
	}
	strncpy(NPServer.ROMName, argv[2], 30);
	NPServer.SyncByReset=FALSE;
	if (argc > 3)
		Settings.FrameTime=atoi(argv[3]);
	Memory.SRAMSize = 0;

	if (!S9xNPStartServer (atoi(argv[1])))
	{
		S9xTracef( "Print f: Server startup failed.\n");
		exit(1);
	}
	S9xTracef( "Print f: Server exited successfully.\n");
}

void S9xReset (void)
{
	// stub
}

bool8 S9xFreezeGame (const char *filename)
{
	(void*) filename = 0; // stub

	return FALSE;
}

bool8 S9xUnfreezeGame (const char *filename)
{
	(void*) filename = 0; // stub

	return FALSE;
}
