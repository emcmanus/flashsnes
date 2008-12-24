Snes9x: The Portable Super Nintendo Entertainment System Emulator
=================================================================
Files included in the Snes9x archive:
Snes9x.exe, readme.txt, license.txt fmod.dll faqs.txt changes.txt

v1.51  May 1st, 2007
=================

Home page: http://www.snes9x.com

Contents
========
Changes Since Last Release
Introduction
What You Will Need
Getting Started
Keyboard Controls
Joystick support
Game Saving
Netplay Support
Movie Support
Cheat Support
What's Emulated?
What's Not?
Super FX
S-DD1 and SPC7110
S-RTC
SA-1
C4
Problems With ROMs
Sound Problems
Converting ROM Images
Speeding up the Emulation
Getting Help
Credits

Changes Since Last Release
==========================

Check the changes.txt file for a complete history of Snes9x changes between
versions.

Introduction
============

Snes9x is a portable, freeware Super Nintendo Entertainment System (SNES)
emulator. It basically allows you to play most games designed for the SNES
and Super Famicom Nintendo game systems on your PC or Workstation. The games
include some real gems that were only ever released in Japan.

The original Snes9x project was founded by Gary Henderson and Jerremy Koot as
a collaboration of their earlier attempts at SNES emulation (snes96 and snes97)
Over the years the project has grown and has collected some of the greatest
talent in the emulation community (at least of the SNES variety) some of which
have been listed in the credits section, others have helped but have been loss
in the course of time.

"Why emulate the SNES?"

Well, there are many reasons for this. The main reason is for nostalgia purposes.
It's a hard find in this day and age to get a SNES and games for it. Plus many
of us over the course of time have lost our beloved consoles (may they R.I.P) but
still have our original carts. With no other means to play them, we turn to
emulators. Besides this there are many conveniences of doing this on the
computer instead of dragging out your old system.

Advantages consist of:
- ability to save in any location of the game, despite how the game was designed.
  It's amazingly useful when you don't want to redo the same level over and over.
- built-in peripherals. This is anything from multi-taps, to super scopes, to
  cheat devices.
- ability to rip sprites and music for your own personal use
- easier to organize and no stacks of cartridges that scare off non-nerdish women.
- filters can be used to enhance graphics on old games.

As with all things there are disadvantages though:
- If you have an ancient PC (pre-Pentium 2) you aren't likely to get a playable
  experience.
- some games are still unemulated (though this a very tiny minority)
- the emulator can be difficult for new users to configure (please read "getting
  started" section below for info)


What You Will Need
==================

A windows 9x/2k based machine for this port
DirectX 6.1b or later
133mhz processor BARE MINIMUM (1ghz+ rec for best settings)
16MB ram BARE MINIMUM (128megs+ rec for graphic pack games)
Any directsound capable sound card

Certain games use added hardware which will REQUIRE a faster pc. Again, the
specs listed above the BARE MINIMUM to use snes9x in any playable form. Most
people will not even find that playable. It is recommended that you get a semi-
modern PC with a 600mhz Pentium 3/celeron/duron/athlon processor if you want
good results. A 1 GHz is recommended for those that want a near perfect
experience. Also 64MB of ram is highly suggested with the possibility of more
being needed if you play a game that requires graphic packs.


Software
--------
Access to SNES ROM images in *.smc, *.sfc, *.fig or *.1, *.2, or sf32xxxa,
sf32xxxb, etc., format otherwise you will have nothing to run!

Some home-brewed ROM images can be downloaded from http://www.zophar.com. To
find commercial games, you could try a web search engine and some imaginative
use of search strings. Please note, it is illegal in most countries to have
commercial ROM images without also owning the actual SNES ROM cartridge.

Getting Started
===============

Launch Snes9x using the Windows explorer to locate the directory where you
un-zipped the snes9x.exe and the fmod.dll files and double-click on
the snes9x.exe executable. You could create a shortcut to Snes9x and drag
that icon out onto your desktop.

Loading Games
-------------
Use the Open option from the File menu to open the ROM load dialog. The dialog
allows you to browse your computer to locate the directory where you have
stored your SNES games. Single-click and then press Load to load and start the
game.

SNES Rom images come in lots of different formats. Predominately you will be
playing ROMs that are still zipped. Snes9x supports zipped ROMs as long as there
is only 1 per zip file. Other formats are listed above in the "software" section.

Game colour System
------------------

Snes9x displays the ROM information when a ROM is first loaded. Depending on the
colours used you can tell whether or not a ROM is a good working ROM, or if
it's been altered or is corrupted.

white   the ROM should be a perfect working copy.
green   the ROM is mode 1 interleaved.
orange  the ROM is mode 2 interleaved.
aqua    the ROM is Game Doctor 24M interleaved.
yellow  the ROM has probably been altered. Either it's a translation, PD ROM,
        hacked, or possibly a bad ROM. It may also be an overdumped ROM.
red     the ROM is definitely hacked and that a proper version should be exist.
        some ROM Tools such as NSRT can also fix these ROMs.

When asking for help on the Snes9x forums, please list the colour and CRC32
that is displayed. This will help to find out what the problem is.

These colours do NOT signify whether a game will work or not. It is just a means
for reference so we can understand what may or may not be a problem. If the
name is red search the internet for a program called NSRT (At present time this
can be found at http://nsrt.edgeemu.com) which may be able to fix it. Most
often the problem with games that don't work it's because they are corrupt or
are a bad dump and should be redownloaded.

SNES Joypad Emulation
---------------------
The default key mapping for joy-pad 1 is:

'up arrow'              Up direction
'down arrow'            Down direction
'left arrow'            Left direction
'right arrow'           Right direction
'a'                     TL button
's'                     TR button
'd'                     X button
'x'                     Y button
'v'                     A button
'c'                     B button
'space'                 Start button
'enter'                 Select button

The real SNES allowed up to five joy-pads to be plugged in at once via a
special adapter. Having five people crowd around the keyboard would not be
much fun, and anyway, all keyboards have a limit on the number of keys that
can be pressed simultaneously and still be detected correctly; much better to
use multiple joysticks or Netplay.

Joystick Support
================

Configure and calibrate your joystick/joy-pad using Windows joystick applet in
the control panel BEFORE starting Snes9X, then use Snes9X's joy-pad config
dialog available from the Options menu to map your joystick/joy-pad's buttons
to the emulated SNES joy-pad(s) buttons. See 'Keyboard/Joystick Config' above
for details.

Keyboard/Joystick Config
------------------------

Add support for your joystick and calibrate it using Windows' joystick applet
from the Windows control panel before starting Snes9x, then use Joy-pad
Configuration dialog in Snes9x to customize the keyboard/joystick to SNES
joy-pad mappings. The dialog is easy to use: select which SNES joy-pad you are
configuring using the combo box (#1 to #5). Make sure that you click the
"enabled" box on that controller or snes9x won't recognize a controller being
plugged in. Click on the text box next to 'UP' and then press the key on the
keyboard or button on your joystick that you would like to perform the UP action.
The focus will automatically move on to the 'RIGHT' text box, press the key or
joystick button that you want to perform the RIGHT action, and so on until
you've customized all the SNES joy-pad buttons.

Use of the special diagonal keys should only be used by keyboard users who are
having problems pressing more then one or 2 buttons at a time. First you must
hit "toggle diagonals" so that you are able to change them.

If you want to play a game that uses the multitap, you must first enable it in
the input menu. Controllers #3, #4, and #5 will not do anything unless multitap
is enabled.

Alternate Controllers
=====================

Peripherals like the SNES Mouse, Super Scope, Justifiers, Multitap are disabled
by default, but you can enable them like so:

First, load your game. Then select the optional controller you want enabled from
the Input menu. Or, the controller is selectable by pressing '7' to cycle to it.

If you use NSRT to add header information to your ROMs, Snes9x will automatically
detect this information and choose the best controller configuration for you
when the game starts up. Incompatible choices will also be grayed out from the 
Input menu, but if you really want, they remain selectable by pressing '7'.



Additional Keyboard Controls
============================

While the emulator is running:

'Escape'       Show/hide the menu-bar.

'Pause'        Pause or unpause the emulator

Alt+'Enter'    Toggle between full-screen and windowed mode.

'Tab'          Turbo mode (fast forward) - for skipping long intros.

'F12'          Takes a screenshot

'0'            Toggle H-DMA emulation on/off.

'1'            Toggle background 1 on/off.

'2'            Toggle background 2 on/off.

'3'            Toggle background 3 on/off.

'4'            Toggle background 4 on/off.

'5'            Toggle sprites on/off

'6'            Toggle swapping of joy-pad one and two around

'7'            Rotate between Multi-player 5, mouse on port 1,
               mouse on port 2 and SuperScope emulation. (need to enable
               special controllers in the menu first)

'8'            Toggle emulation of graphics window effects on/off.

'9'            Toggle transparency effects on and off - only if
               16-bit or higher screen mode selected.

'`'            Superscope turbo button.

'~'            Superscope pause button.

Shift+'F1-F9 ' Save a freeze game file.

'F1-F9'        Load a freeze game file, restoring a game to an
               exact position.

'-'            Increase emulated frame time by 1ms - slowing down
               the game. (auto-frame skip must be on)

'+'            Decrease emulated frame time by 1ms - speeding up the
               game. (auto-frame skip must be on)

'\'            Pauses the game, or slowly advances gameplay if it's already paused.
               To return to normal, press the 'Pause' key.

Shift+'insert' Toggles turbo on the 'L' button. Note: toggles for all
               controllers

Shift+'delete' Toggles turbo on the 'R' button...

Shift+'['      Toggles turbo on the 'select' button...

Shift+']'      Toggles turbo on the 'start' button...

Shift+'home'   Toggles turbo on the 'Y' button...

Shift+'pageup' Toggles turbo on the 'X' button...

Shift+'end'     Toggles turbo on the 'B' button...

Shift+'pagedown' Toggles turbo on the 'A' button...


Shift+'+'      Increase frame rendering skip rate, making the screen
               updates more jerky but speeding up the game.

Shift+'-'      Decrease frame rendering skip rate, making the game
               update more smoothly, but potentially slowing down the
               game. Repeatedly pressing the key will eventually
               switch to auto-frame skip rate where the rate is
               dynamically adjusted to keep a constant game play
               speed.

','            Toggle display of input, so you can see which SNES buttons
               are registering as pressed.

'.'            Toggles movie frame display on/off. Movie must be open.

Shift+'8'      Toggles movie read-only status. Movie must be open.

Ctrl+Shift+'R' Resets the game.


Go to Input > Customize Hotkeys... to configure these and more.


Game Saving
===========

Many SNES games could take a very long time to complete from start to finish so
they allowed your progress to be saved into RAM fitted inside the game pack;
the RAM contents were backed up by a battery when the SNES was switched off or
the game removed. Snes9x simulates this by saving the contents of the emulated
battery-backed RAM into a file (*.srm) when you load a new game or exit Snes9x.
The file is then automatically re-loaded the next time you play the game.

Snes9x also provides freeze-files; these are files that save a game's
position at any point in the game, not just at predefined places chosen by
the game's designers - ideal for saving your game just before a tricky bit!
Snes9x provides 9 save slots; during a game, press Shift + F1 to F9 to save a
game, and just F1 to F9 to load it again later.

Freeze game files and Save-RAM (S-RAM) save files are normally written to and
read from the folder called Saves where your snes9x.exe is located, but
sometimes this is not desirable or possible, especially if it’s a CD-ROM,
which is of course is usually read-only! You can change the folder where
Snes9X saves and loads S-RAM and freeze-files using the Settings Dialog,
available from the Options menu.

Snes9x uses its own unique format for freeze-files, as does ZSNES, but Snes9x
can also load ZSNES format freeze-files. Just copy the ZSNES freeze files into
your save directory and, if the native format Snes9x freeze file doesn't exist
(<ROM image name>.00X where X is a digit), Snes9x will try to load the
corresponding ZSNES freeze file instead (<ROM image name>.zsX where X is a 't'
or a digit). When you freeze a game position after loading a ZSNES format
freeze file, Snes9x will save it in native Snes9x format.

Netplay Support
===============

This support should currently be considered beta. Netplay support allows
up to five players to sit in front of different computers and simultaneously
play the same game, or just watch someone else play a game. All the computers
have to be connected to a network that allows TCP/IP traffic to flow between
them; this includes a local Ethernet-style network, a modem connection to
another machine, a Windows direct-cable connection, or, if you're lucky and 
have short ping times, the Internet.

Its currently easier if you use Snes9x in windowed mode while using Netplay,
mainly because Netplay currently displays status information in the window's
title bar, and it might be necessary to setup a separate chat application so
you can talk to the other players when deciding what game to play next.

One machine has to act as a server which other players (client sessions) 
connect to. The 'master' player, player 1, uses the server machine; the master
decides what game to play. The server machine should be selected to be the
fastest machine on the fastest connection in the group taking part due to the
extra functions it has to perform.

Load up a game, then select the 'Act as server' option from the Netplay menu
to become a Netplay server; the 'network', in whatever form it takes, will
need to be initialised, if necessary, before you do this. Then just wait for 
other players to connect...

Clients connect to the server using the 'Connect to server...' dialog, again
available from the Netplay menu. Type in the IP address or host name of the
machine running the Snes9x server session and press OK. The first remote client
to connect will become player 2, and so on. Start Menu->Run->winipcfg will tell
you your current IP address, but note that most dial-up ISPs will allocate you
a new IP address each time you dial in.

If the server has the 'Send ROM Image to Client' option checked, it will send
the client a copy of the game it is currently playing; don't enable this option
when using a slow network - sending 4Mbytes+ to several clients will takes ages
when using a modem! If the option is not checked the server will request the
client loads up the correct game first before joining the game.

Once the client has got a copy of the game the server is playing, the server
will then either send it S-RAM data and reset all players' games if the 
'Sync Using Reset Game' option is checked, or send it a freeze file to get the
new client in sync with the other player's progress in a game.

If the master player loads a different game, the server will either 
automatically send remote clients a copy, or request that they load the game.
If the master player loads a freeze file, the server will automatically send
that to remote clients as well.

Client sessions must be able to keep up with the server at all times - if they
can't, either because the machine is just too slow, or its busy, the games
will get out of sync and it will be impossible to successfully play a 
multi-player game...

...To make sure this doesn't happen, don't move the Snes9x window unnecessarily
and don't use Ctlt+Alt+Del to display the task manager while playing. Also stop
any unnecessary applications and as many background tasks as possible. Even
something as simple as a text editor might periodically write unsaved data to
the disk, stealing CPU time away from Snes9x causing it to skip a frame or
delay a sound effect; not a problem for most games, but the Bomberman series
(the best multi-player games on the SNES) sync the game to sound samples 
finishing. Turning off 'Volume envelope height reading' from the Sound Options
dialog might help with this problem.

Movie Support
=============

This feature allows you to record your actions while playing a game. This can be
used for your own personal playback or to show other people that you can do
something without them having to be around when you did it. These can be saved
and shared on the internet with ease as they are comparatively small.

To use, simply click file and click on movie. Click the record button. Here you
can decide when to start recording. If you want to record from the very start
of a game,click on record from reset. If you want to start recording from where
you are alreadyin a game click the record from now. You can also choose which
controllers to record. If you are playing by yourself leave joypad 1 as the
only one selected. The more controllers you choose to record the larger the
file size will be.

To play back a movie you recorded simple click file, movie, play and select the
file to play. Make sure the movie was recorded with the same ROM that you have
loaded"

If you make a mistake while recording a movie, there is a movie rerecord
function. Simply create a save state anytime while recording. If you want to
re-record simply load the save state and it will bring up the message
"movie re-record". Loading any save state while a movie is playing or recording
will cause this to happen. If you want to watch a video with no chance to
accidentally alter it check "open as read only" when you go to play it.


Cheat Support
=============

Use the Cheat Code Entry and Editor dialog from the Cheats menu to enter
Game Genie or Pro-Action Reply cheat codes. Cheat codes allow you to,
surprisingly, cheat at games; they might give you more lives, infinite health,
enable special powers normally only activated when a special item is found,
etc.

Many existing Game Genie and Pro-Action Reply codes can be found at:
http://vgstrategies.about.com/library/ggn/bl_ggnsnes.htm?once=true&

Type in a Game Genie or Pro-Action Reply code into the "Enter Cheat Code" text
edit box and press <Return>. Be sure to include the '-' when typing in a Game
Genie code. You can then type in an optional short description as a reminder
to yourself of what function the cheat performs. Press <Return> again or click
the Add button to add the cheat to the list.

Note that the Add button remains insensitive while "Enter Cheat Code" text
edit box is empty or contains an invalid code. The cheat code is always
translated into an address and value pair and displayed in the cheat list as
such.

Beware of cheat codes designed for a ROM from a different region compared to
the one you are playing or for a different version of the ROM; the source of
the cheats should tell you which region and for which version of the game they
were designed for. If you use a code designed for a different region or version
of your game, the game might crash or do other weird things.

It is also possible to enter cheats as an address and value pair; some users
have requested this functionality. Type in the address into the "Address"
text edit box then type the value into the "Value" text edit box. The value is
normally entered in decimal, but if you prefix the value with a '$' or append
an 'h' then you can enter the value in hex.

Double-clicking on an cheat line from the list in the dialog or clicking on
the "En" column toggles an individual cheat on and off. All cheats can be
switched on and off by checking and unchecking the "Apply cheats" item from
the Cheat menu.

Selecting a cheat from the list causes its details to be filled into the text
edit boxes in the dialog box; the details can then be edited and the Change
button pressed to commit the edits. Note that the "Enter Cheat Code" text edit
box always redisplays the cheat code as a Pro-Action Replay code regardless of
whether you originally entered it as a Game Genie or Pro-Action Replay code.

Selecting a cheat from the list then pressing the Delete button permanently
removes that cheat.

Cheats are saved in .cht files stored in the Freeze File Directory and are
automatically loaded the next time a game with the same filename is loaded.
The format for the .cht files is the same format as used by the other excellent
SNES emulator, ZSNES.

Snes9X also allows new cheats to be found using the Search for New Cheats
dialog, again available from the Cheats menu. The easiest way to describe the
dialog is to walk through an example.

Cheat Search Example
--------------------
Let’s give ourselves infinite health and lives on Ocean's Addams Family
platform game:

Load up the game; keep pressing the start button (Return key by default) to
skip past the title screens until you actually start playing the game. You'll
notice the game starts with 2 health hearts and 5 lives. Remember that
information, it will come in useful later.

Launch the cheat search dialog for the first time; Alt+A is its accelerator.
Press the Reset button just in case you've used the dialog before, leave the
Search Type and Data Size radio boxes at their default values and press OK.

Play the game for a while until you loose a life by just keep walking into
baddies, when the game restarts and the life counter displays 4, launch the
cheat search dialog again but this time press the Search button rather than
Reset. The number of items in the list will reduce, each line shows a memory
location, its current value and its previous value; what we're looking for is
the memory location where the game stores its life counter.

Look at address line 7E00AC, its current value is 4 and its previous value was
5. Didn't we start with 5 lives? Looks interesting...

Note that some games store the current life counter as displayed on the screen,
while others store current number of lives minus 1. Looks like Addams Family
stores the actual life count as displayed on the screen.

Just to make sure you've found the correct location, press OK on the dialog,
and play the game until you loose another life. Launch the search dialog again
after the life counter on screen has been updated and press the Search
button. Now there are even fewer items in the list, but 7E00AC is there again,
this time the current value is 3 and the previous value was 4. Looks very much
like we've found the correct location.

Now that we're happy we've found the correct location, click on the 7E00AC
address line in the list and then press the Add Cheat button. Another dialog,
Cheat Details, will be displayed. Type in a new value of say 5, this will be
number of lives that will be displayed by the lives counter. Don't be greedy;
some games display a junk life counter or might even crash if you enter a
value that's too high; Snes9X keeps the value constant anyway, so even if you
do loose a life and life counter goes down by one, less than 20ms later,
Snes9X resets the counter back to the value you chose!

If the memory location you add a cheat on proves to be wrong, just go to the
Cheat Code Editor dialog and delete the incorrect entry.

Now let’s try and find the Addams Family health counter. While two hearts are
displayed on the screen, visit the cheat search dialog and press the Reset
button followed by OK. Play the game until you loose a heart by touching a
baddie, then visit the cheat search dialog again.

Press the Search button to update the list with all memory locations that have
gone down in value since the last dialog visit. We're going to have to try and
find the heart memory location now because there were only two hearts to start
with.

Look at address line 7E00C3, its current value is 1 and its previous value was
2. Scrolling through the list doesn't reveal any other likely memory locations,
so let’s try our luck. Click on the 7E00C3 line, press the Add Cheat button and
type in a new value of say 4 into the dialog that appears and press OK. Press
OK on the Search for New Cheats dialog to return to the game.

At first sight it looks like 7E00C3 wasn't the correct memory location because
the number of hearts displayed on screen hasn't gone up, but fear not, some
games don't continually update health and life displays until they think they
need to. Crash into another baddie - instead of dying, the number of hearts
displayed jumps up to 4! We've found the correct location after all!

Now every time you play Addams Family you'll have infinite lives and health.
Have fun finding cheats for other games.

What's Emulated?
===============
- The 65c816 main CPU.
- The Sony SPC700 sound CPU.
- SNES variable length machine cycles.
- 8 channel DMA and H-DMA (raster effects).
- All background modes, 0 to 7.
- Sound DSP, with eight 16-bit, stereo channels, compressed samples, hardware
  attack-decay-sustain-release volume processing, echo, pitch modulation
  and digital FIR sound filter.
- 8x8, 16x8 and 16x16 tile sizes, flipped in either direction.
- 32x32, 32x64, 64x32 and 64x64 screen tile sizes.
- H-IRQ, V-IRQ and NMI.
- Mode 7 screen rotation, scaling and screen flipping.
- Vertical offset-per-tile in modes 2, and 4.
- Horizontal offset-per-tile in modes 2, 4 and 6.
- 256x224, 256x239, 512x224, 512x239, 512x448 and 512x478 SNES screen
  resolutions.
- Sub-screen and fixed colour blending effects.
- Mosaic effect.
- Single and dual graphic clip windows, with all four logic combination modes.
- Colour blending effects only inside or outside a window.
- 128 8x8, 16x16, 32x32 or 64x64 sprites, flipped in either direction.
- SNES palette changes during frame (15/16-bit internal rendering only).
- Direct colour mode - uses tile and palette-group data directly as RGB value.
- Super FX, a 21/10MHz RISC CPU found in the cartridge of several games.
- S-DD1, a data decompression chip used only in Star Ocean and Street Fighter 2
  Alpha. The compression algorithm is integrated into Snes9x, but you may still
  use the old graphics pack cheat as a speed boost.
- SPC7110, similar in use to S-DD1, but the algorithm is still unknown.
- S-RTC, a real-time clock chip. Dai Kaijyu Monogatari II is the only game
  that uses it.
- SA-1, a faster version of CPU found in the main SNES unit together with some
  custom game-accelerator hardware.
- C4, a custom Capcom chip used only in Megaman X2 and X3. It’s a sprite scaler/
  rotator/line drawer/simple maths co-processor chip used to enhance some
  in-game effects.
- OBC1 is a sprite management chip. Metal combat is the only game to use this.
- Greater DSP-1 support, enough that all games should load, but some may have
  graphical glitches.
- DSP-2 support. Only used in Dungeon Master
- DSP-4 partial support. Top Gear 3000 goes in game but still very glitchy
- SNES mouse.
- SuperScope (light gun) emulated using computer mouse.
- Multi-player 5 - allowing up to five people to play games simultaneously on
  games that support that many players.
- Game-Genie and Action Replay cheat codes.
- Multiple ROM image formats, with or without a 512 byte copier header.
- Single or split images, compressed using zip and gzip, and interleaved in one
  of two ways.
- Auto S-RAM (battery backed RAM) loading and saving.
- Freeze-game support, now portable between different Snes9x ports.
- 4-point gaussian interpolated sound.
- Justifier support. Konami's Justifier is similar to the Super Scope and
  used in Lethal Enforcers.
- Seta-10 CPU (ST010). This is used F1 Race of Champions 2.
- Fixed color and mosaic effects in SNES hi-res. (512x448) modes.
- Offset-per-tile in mode 6.
- Pseudo hi-res.
- Mosaic effect on mode 7.
- Satellaview and BS-X, partially.

What's Not?
==========
- DSP-1 support not complete, enough to play Mario Kart, Pilotwings and many
  others. All DSP-1 games should boot, but may display graphical errors.
- Any other odd chips that manufactures sometimes placed inside the
  cartridge to enhance games and as a nice side-effect, also act as an
  anti-piracy measure. (DSP-3, SETA 11 and SETA 18, as examples)
- A couple of SPC700 instructions that I can't work exactly out what they
  should do.
- The expansion slot found in many carts.

Super FX
========
The Super FX is a 10.5/21MHz RISC CPU developed by Argonaut Software used as a
game enhancer by several game tiles. Support is still a little buggy but most
games work very well, if a little slowly. Released SNES Super FX games included
Yoshi's Island (best single-player game on SNES, if you like platform games),
Doom, Winter Gold, Dirt Trax FX, StarFox, Stunt Race FX and Vortex. If you're
lucky, you might find a copy of the unreleased Starfox 2 image floating around,
but its sound code is corrupt and you'll need to disable sound CPU emulation to
play it. (NOTE: A new version of Starfox 2 was recently released that is closer
to being a complete ROM. It is mostly playable in the emulator but due to it
being an  incomplete game it does have many errors. This is not the fault of
the emulator)

Lots of Super FX ROM images available are in an odd, interleaved format that I
haven't worked out an easy way to auto-detect. If Snes9x detects that a
Super FX game crashes (by executing a BRK instruction), it automatically
assumes the ROM is in this odd format and de-mangles the ROM and tries to run
it again. If your ROM image isn't working, you could try selecting the
Interleave mode 2 option on the ROM load dialog before loading the game to help
out Snes9x.

S-DD1 and SPC7110
=====
The S-DD1 is a custom data decompression chip that can decompress data in real-
time as the SNES DMA's data from the ROM to RAM. Only two games are known to
use the chip: Star Ocean and Street Fighter Alpha 2.

SPC7110 is a compression and memory mapping chip. It provides a few extra
features, as well. It functions as an RTC interface, and has a multiply/divide
unit that has more precision than the SNES. The SPC7110 is found only in 4
games: Super Power League 4, Far East of Eden Zero, Far East of Eden Zero -
Shounen Jump no Shou, and Momotaro Densetsu Happy.

These chips use some unknown compression algorithms, so to actually support the
games using these, pre-decompressed graphics packs must be downloaded from the
web, unpacked, and the resultant folder can be selected from the Snes9x options
-> GFX Pack Configuration. As of Snes9x 1.42, the S-DD1 can be used without the
graphics packs.

To use the graphics packs for the S-DD1, as of 1.42 and later, you MUST tell
Snes9x where the pack is located. If you do not, it will use on-the-fly
decompression. On the other hand, to use on-the-fly decompression, make sure the
game's pack entry is blank.

The SPC7110 still uses packs at all times, and if no pack is defined, it will
search the locations just like ZSNES.

SA-1
====
The SA-1 is a fast, custom 65c816 8/16-bit processor, the same as inside the
SNES itself, but clocked at 10MHz compared to a maximum of 3.58MHz for the CPU
inside the SNES.

The SA-1 isn't just a CPU; it also contains some extra circuits developed by
Nintendo which includes some very fast RAM, a memory mapper, DMA and, several
real-time timers.

Snes9X includes emulation of most features of the SA-1, enough to play all SA-1
games I've located so far, these include Mario RPG, Kirby Superstar and
Parodius 3.

C4
==

The C4 is custom Capcom chip used only in the Megaman X2 and Megaman X3 games.
It can scale and rotate images, draw line-vector objects and do some simple
maths to rotate them.

Snes9x's C4 emulation is a direct copy of the ZSNES C4 emulation; Intel-based
ports even make use of ZSNES code. Without zsKnight's hard work, Snes9x would
not have C4 emulation. Many thanks go to him.


Problems With ROMs
==================

If the emulator just displays a black screen for over 10 seconds after you've
loaded a ROM image, then one of the following could be true:

1) You just loaded some random ROM image you've downloaded from the Internet
   and it isn't even a SNES game or you only downloaded part of the image.
   Snes9x only emulates games designed for the Super NES, not NES, or
   Master System, or Game Boy, or <insert your favourite old games system here>.
2) If it's a Super FX game, chances are it's in interleaved2 format, try
   switching to "Interleaved mode 2" on the ROM load dialog before loading the
   game.
3) Someone has edited the Nintendo ROM information area inside the ROM image
   and Snes9x can't work out what format ROM image is in. Try playing
   around with the ROM format options on the ROM load dialog.
4) The ROM image is corrupt. If you're loading from CD, I know it might
   sound silly, but is the CD dirty? Clean, un-hacked ROM images will display
   [checksum ok] when first loaded, corrupt or hacked ROMs display
   [bad checksum].
5) The original SNES ROM cartridge had additional hardware inside that is not
   emulated yet and might never be.
6) You might be using a file that is compressed in a way Snes9x does not
   understand.

The following ROMs are known to currently not to work with any version of Snes9x:
- SD Gundam GX - DSP-3
- Hayazashi Nidan Morita Shougi - Seta 11
- Hayazashi Nidan Morita Shougi 2 - Seta 18

Sound Problems
==============

No sound coming from any SNES game using Snes9x? Could be any or all of
these:

- If all sound menu options are grayed out, or an error dialog about Snes9x not
  being able to initialize DirectSound is displayed - then DirectSound could
  not initialize itself. Make sure DirectX 6 or above is installed and your
  sound card is supported by DirectX.

  Installing the latest drivers for your sound card might help. Another
  Windows application might have opened DirectSound in exclusive mode or
  opened the Windows WAVE device - WinAmp uses the Windows WAVE device by
  default - in which case you will need to stop that application and then
  restart Snes9x. It is possible to switch WinAmp to use DirectSound, in
  which case both Snes9x and WinAmp output can be heard at the same time.

  If your sound card isn't supported by DirectX very well (or not at all) you
  will have to use FMOD's WAVE output option; but WAVE output introduces a
  0.15s delay between Snes9x generating sample data and you hearing it.
  Select FMOD's WAVE output by choosing the "FMOD Windows Multimedia" sound
  driver option from the Sound Settings dialog.

- The sound card's volume level might be set too low. Snes9x doesn't alter the
  card's master volume level so you might need to adjust it using the sound
  card's mixer/volume controls usually available from the task bar or start
  menu.
- Make sure your speakers and turned on, plugged in and the volume controls are
  set to a suitable level.
- You've turned off sound CPU emulation, clicked the Mute button in the
  sound settings dialog, or set the playback rate to "<No sound>".

General sound problems:
- A continuous, crackling sound or buzz can be heard.

  First make sure it is happening in all games - Snes9x still does have one or
  two sound emulation bugs that cause the odd pop, crackle and buzz in a few
  games.

  Once you're happy that it is not just the game you're playing, set the
  playback rate in the Sound Settings dialog to 32KHz, and uncheck "Stereo" and
  "16bit playback". Next set both sound buffer length and mix values to 10ms,
  then try slowly increasing both values until clear sound can be heard. The
  ideal is that the mix interval and sound buffer length values should be as
  small as possible. The mix interval value must always be smaller than the
  sound buffer length otherwise sound data will be lost.

  If your sound card requires larger values, above, say, 40ms, then it might
  also be necessary to enable the "Generate sample data in sync with sound CPU"
  option to maintain accurate sound emulation.

  If all else fails, try selecting the "FMOD Window Multimedia" sound driver
  option and live with the 0.15s lagged sound that is unavoidable when using
  the older Windows WAVE sound API.

  Once you have clear sound, set the playback rate, 16bit sound and stereo
  settings to quality you would like - it might be necessary to tweak the
  sound buffer length and mix interval values again.

- Sound the lagged/delayed compared to on-screen action. If you're using the
  "FMOD Windows Multimedia" sound driver then delayed sound is unavoidable;
  otherwise your sound buffer length/mix interval settings are too large - if
  you can, reduce their values.
- Sound quality is poor on all games. You might have a noisy sound card
  (usually cheaper sound cards), turning on 16-bit, interpolated sound,
  sync-sound and/or increasing the playback rate might help.
- Sound seems to have gaps. Using larger sound buffer or mix interval values
  can cause this; reduce them if you can, or click on the "Generate sample data
  in sync with sound CPU" option.
- Sound is awful in all games. You might have selected a playback rate/stereo/
  8-bit/16-bit combination that your sound card can't cope with. Try setting
  8-bit (uncheck the 16bit playback option), mono (uncheck the Stereo option)
  22KHz from the sound menu and if that cures the problem, try other
  combinations until you find the best setting that works on your sound card.
- Snes9x plays any game fine for a few seconds then freezes or crashes.
  Try turning off the "Generate sample data in sync with sound CPU" option.

If all else fails, try posting a message describing your problem and
requesting help on the Snes9X message board at the Snes9X web site,
http://www.snes9x.com/

Converting ROM Images
=====================

If you have a ROM image in several pieces, simply rename them so their
filename extensions are numbered: e.g. game.1, game.2, etc. Then, when
loading the ROM image, just specify the name of the first part; the remaining
parts will be loaded automatically.

If they are already in the form sf32xxxa, sf32xxxb, etc., you don't even have
to rename them; just choose the name of the first part from the ROM load
dialog, as above.

If you want the ROM to be 1 piece instead of many, you can use tools such as
SNESTool to remerge the files into one file.

Emulation speed
===============

Emulating an SNES is very processor intensive, with its two or sometimes three
CPUs, an 8 channel digital sound processor with real-time sound sample
decompression and stereo sound, two custom graphics processors, etc.

If you only have a minimum level machine, you will need to stick to using 
minimal or no graphics filters or sound. Disabling the joystick support will
also help.

Full-screen mode is generally faster than windowed mode.

Enabling one of the output image processing modes from the Display Config
dialog can slow down overall emulation speed greatly depending on the type of
game and video RAM speed. Enabling the stretch image option further reduces
emulation speed.

Lowering the sound playback rate, selecting 8-bit mono sound or turning off
interpolated or sync-sound modes, or turning off sound CPU emulation altogether
will also help boost emulation speed.

Credits
-------

- Jerremy Koot for all his hard work on previous versions of Snes96, Snes97
  and Snes9x.
- Ivar for the original Super FX C emulation, DSP-1 emulation work and
  information on both chips.
- zsKnight and _Demo_ for the Intel Super FX assembler, DSP-1 and C4 emulation
  code.
- zsKnight and _Demo_ for all the other ideas and code I've nicked off them;
  they've nicked lots of my ideas and information too!
- John Weidman and Darkforce for the S-RTC emulation information and code.
- Kreed for his excellent image enhancer routines.
- Nose000 for code changes to support various Japanese SNES games.
- Neill Corlett for the IPS patching support code.
- DiskDude's SNES Kart v1.6 document for the Game Genie(TM) and Pro-Action
  Replay cheat system information.
- Lord ESNES for some nice chats and generally useful stuff.
- Lee Hyde (lee@jlp1.demon.co.uk) for his quest for sound information and
  the Windows 95 icon.
- Shawn Hargreaves for the rather good Allegro 3.0 DOS library.
- Robert Grubbs for the SideWinder information - although I didn't use his
  actual driver in the end.
- Steve Snake for his insights into SNES sound sample decompression.
- Vojtech Pavlik for the Linux joystick driver patches.
- Maciej Babinski for the basics of Linux's DGA X server extensions.
- Alexander Larsson for the GGI Linux port code.
- Harald Fielker for the original sound interpolation code (never used directly
  due to problems).
- Takehiro TOMINAGA for many speed up suggestions and bug fixes.
- Predicador for the Windows icon.
- Lindsey Dubb for the mode 7 bi-linear filter code and the improved
  colour addition and subtraction code.
- Anti Resonance for his super-human efforts to help get his fast sound CPU
  core and sound DSP core working in Snes9x.
- Brad Martin and TRAC for sound emulation.
- byuu for the most exact timing information.
- pagefault, TRAC, Dark Force, byuu, and others who have donated ideas
  and/or code to the project.

Nintendo is a trademark.
Super NES, SuperScope and Super FX are trademarks of Nintendo.
Sun, Solaris and Sparc are all trademarks of Sun Microsystems, Inc.
Game Genie is a trademark of Lewis Galoob Toys, Inc.
MS-DOS and Windows 95 are trademarks of Microsoft Corp.
Intel, Pentium and MMX are all trademarks of Intel Corp.
Sony is a trademark of Sony Corp.
UNIX is a registered trademark of X/Open.
Glide is a trademark of 3Dfx Interactive, inc.
Linux is a registered trademark of Linus Torvalds.

------------------------------------------------------------------------------
Gary Henderson
gary@snes9x.com
