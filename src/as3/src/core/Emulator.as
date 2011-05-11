package core
{
	import cmodule.libSNES9x.CLibInit;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.display.StageDisplayState;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.SampleDataEvent;
	import flash.external.ExternalInterface;
	import flash.geom.Rectangle;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	public class Emulator
	{
		// Public
		public var screen:Bitmap;
		public var requestedSamples:int;	// Samples per frame
		
		////////////////////////////////////////////////////////////
		
		// Audio
		private var soundChannel:SoundChannel;
		private var sound:Sound;
		
		// Video
		private var displayBufferAddress:Number;
		private var displayData:BitmapData;
		private var displayRect:Rectangle;
		private var stage:Stage;
		
		// Keyboard
		private var keyStates:Array;
		private var keyEvents:Array;
		
		// Emulator
		private var cLib:Object;
		private var cLoader:CLibInit;
		private var cMemory:ByteArray;
		
		// State
		private var mute:Boolean;
		private var pause:Boolean;
		private var isFullscreen:Boolean;
		
		// Config
		private var displayWidth:int = 256;
		private var displayHeight:int = 224;
		
		////////////////////////////////////////////////////////////
		
		/*
		 *	Constructor
		 */
		public function Emulator( rom:ByteArray, stageRef:Stage )
		{
			var ram_NS:Namespace;
			
			stage = stageRef;
			
			// C Application
			cLoader = new CLibInit();
			cLoader.supplyFile( "romfile", rom );
			
			cLib = cLoader.init();
			cLib.setup();
			
			// RAM
			ram_NS = new Namespace("cmodule.libSNES9x");
			cMemory = (ram_NS::gstate).ds;
			
			// Display
			displayData = new BitmapData( displayWidth, displayHeight, false, 0x0 );
			displayRect = new Rectangle( 0, 0, displayWidth, displayHeight );
			
			screen = new Bitmap( displayData );
			
			// Keyboard
			keyStates = new Array( 256 );
			keyEvents = new Array();
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyPress);
			stage.addEventListener(KeyboardEvent.KEY_UP, onKeyRelease);
			
			for ( var i:int = 0; i<keyStates.length; i++ )
			{
				keyStates[i] = false;
			}
			
			// Audio
			sound = new Sound;
			sound.addEventListener( SampleDataEvent.SAMPLE_DATA, onSampleData );
			
			requestedSamples = 600;		// Undersample
			
			// Timer
			stage.addEventListener( Event.ENTER_FRAME, onFrameEnter );
			
			// JavaScript API consumer detection
			var enableJsApi:Boolean = ExternalInterface.call( "eval", "(function(){ return window.onFlashSnesReady != undefined; }).();" );
			
			if (enableJsApi)
			{
				this.initializeJsApi();
				
				// Fire JS callback
				ExternalInterface.call( "window.onFlashSnesReady", ExternalInterface.objectID ); 
			}
		}
		
		
		/*
		 * JavaScript API
		 *
		 * When FlashSNES is initialized we check for the existence of window.onFlashSnesReady.
		 * 
		 * If the function is defined we'll initialize the JavaScript API and turn off our internal 
		 * key event listeners. then we'll finally execute window.onFlashSnesReady and pass in our 
		 * object ID.
		 */
		
		private function initializeJsApi():void
		{
			// Register Javascript API
			
			ExternalInterface.addCallback( "setMute", setMute );
			ExternalInterface.addCallback( "getMute", getMute );
			ExternalInterface.addCallback( "getPause", getPause );
			ExternalInterface.addCallback( "setPause", setPause );
			ExternalInterface.addCallback( "setFullscreen", setFullscreen );
			ExternalInterface.addCallback( "getFullscreen", getFullscreen );
			ExternalInterface.addCallback( "getKeyState", getKeyState );
			ExternalInterface.addCallback( "setKeyState", setKeyState );
			
			// Unregister key press listeners
			
			stage.removeEventListener(KeyboardEvent.KEY_DOWN, onKeyPress);
			stage.removeEventListener(KeyboardEvent.KEY_UP, onKeyRelease);
		}
		
		
		/*
		 * Paused 
		 */
		
		private function getPause():Boolean
		{
			return this.pause;
		}
		
		
		private function setPause( paused:Boolean ):void
		{
			pause = paused;
		}
		
		
		/*
		 * Fullscreen
		 */
		
		public function setFullscreen( fullscreen:Boolean ):void
		{
			if ( isFullscreen )
			{
				stage.displayState = StageDisplayState.FULL_SCREEN;
			}
			else
			{
				stage.displayState = StageDisplayState.NORMAL;
			}
		}
		
		
		public function getFullscreen():Boolean
		{
			isFullscreen = stage.displayState == StageDisplayState.FULL_SCREEN;
			
			return isFullscreen;
		}
		
		
		/*
		 * Mute
		 */
		
		public function setMute( muted:Boolean ):void
		{
			mute = muted;
			
			cLib.setMute( muted as int );
		}
		
		
		public function getMute():Boolean
		{
			return mute;
		}
		
		
		////////////////////////////////////////////////////////////
		
		/*
		 *	Audio
		 */
		private function onSoundComplete( e:Event ):void
		{
//			trace("\tcomplete");
			
			this.soundChannel.removeEventListener(Event.SOUND_COMPLETE, onSoundComplete);
			this.soundChannel = null;
		}
		
		private function onSampleData( e:SampleDataEvent ):void
		{
			e.data.endian = Endian.LITTLE_ENDIAN;
			cLib.paintSound( e.data );
		}
		
		
		/*
		 *	Keyboard
		 */
		private function onKeyPress( e:KeyboardEvent ):void
		{
			if ( keyStates[ e.keyCode ] != true )
			{
				keyEvents.push( e.keyCode | 1 << 8 );
			}
			keyStates[ e.keyCode ] = true;
		}
		
		private function onKeyRelease( e:KeyboardEvent ):void
		{
			if ( keyStates[ e.keyCode ] != false )
			{
				keyEvents.push( e.keyCode | 0 << 8 );
			}
			keyStates[ e.keyCode ] = false;
		}
		
		
		/*
		 *	Tick
		 */
		private function onFrameEnter( e:Event ):void
		{
			if ( pause )
			{
				return;
			}
						
			// General
			cLib.tick( keyEvents, requestedSamples );
			keyEvents.length = 0;
			
			// Display
			if (!displayBufferAddress)
			{
				displayBufferAddress = cLib.getDisplayPointer();
			}
			
			if (displayBufferAddress)
			{
				cMemory.position = displayBufferAddress;
				displayData.setPixels( displayRect, cMemory );
			}
			
			// Sound
			if (!mute && !soundChannel)
			{
				soundChannel = sound.play();
				soundChannel.addEventListener(Event.SOUND_COMPLETE, onSoundComplete);
			}
		}
		
		
		/*
		
		External Interface
		
		
		Packed event format: 9th bit for press/release, lower 8 bits for scan code
		
		
		Player 1
		
		S9xMapButton( 65, cmd = S9xGetCommandT("Joypad1 Left"), false );    // A
		S9xMapButton( 68, cmd = S9xGetCommandT("Joypad1 Right"), false );   // D
		S9xMapButton( 87, cmd = S9xGetCommandT("Joypad1 Up"), false );      // W
		S9xMapButton( 83, cmd = S9xGetCommandT("Joypad1 Down"), false );    // S
		
		S9xMapButton( 79, cmd = S9xGetCommandT("Joypad1 X"), false );       // O
		S9xMapButton( 80, cmd = S9xGetCommandT("Joypad1 Y"), false );       // P
		S9xMapButton( 75, cmd = S9xGetCommandT("Joypad1 A"), false );       // K
		S9xMapButton( 76, cmd = S9xGetCommandT("Joypad1 B"), false );       // L
		
		S9xMapButton( 88, cmd = S9xGetCommandT("Joypad1 L"), false );       // X
		S9xMapButton( 77, cmd = S9xGetCommandT("Joypad1 R"), false );       // M
		
		S9xMapButton( 13, cmd = S9xGetCommandT("Joypad1 Start"), false );   // Enter
		S9xMapButton( 16, cmd = S9xGetCommandT("Joypad1 Select"), false );  // Shift
		
		
		Player 2
		
		S9xMapButton( 70, cmd = S9xGetCommandT("Joypad2 Left"), false );    // F
		S9xMapButton( 72, cmd = S9xGetCommandT("Joypad2 Right"), false );   // H
		S9xMapButton( 84, cmd = S9xGetCommandT("Joypad2 Up"), false );      // T
		S9xMapButton( 71, cmd = S9xGetCommandT("Joypad2 Down"), false );    // G
		
		S9xMapButton( 67, cmd = S9xGetCommandT("Joypad2 X"), false );       // C
		S9xMapButton( 86, cmd = S9xGetCommandT("Joypad2 Y"), false );       // V
		S9xMapButton( 66, cmd = S9xGetCommandT("Joypad2 A"), false );       // B
		S9xMapButton( 78, cmd = S9xGetCommandT("Joypad2 B"), false );       // N
		
		S9xMapButton( 89, cmd = S9xGetCommandT("Joypad2 L"), false );       // Y
		S9xMapButton( 85, cmd = S9xGetCommandT("Joypad2 R"), false );       // U
		
		S9xMapButton( 81, cmd = S9xGetCommandT("Joypad2 Start"), false );   // Q
		S9xMapButton( 69, cmd = S9xGetCommandT("Joypad2 Select"), false );  // E
		
		*/
		
		
		private function mapKeyDescription( keyDescription:String ):uint
		{
			var code:uint = 0;
			
			switch( keyDescription.toLowerCase() )
			{
				// Player 1
				
				case "joypad1 left":
					code = 65;
					break;
				
				case "joypad1 right":
					code = 65;
					break;
				
				case "joypad1 up":
					code = 65;
					break;
				
				case "joypad1 down":
					code = 65;
					break;
				
				case "joypad1 x":
					code = 79;
					break;
				
				case "joypad1 y":
					code = 80;
					break;
				
				case "joypad1 a":
					code = 75;
					break;
				
				case "joypad1 b":
					code = 76;
					break;
				
				case "joypad1 l":
					code = 88;
					break;
				
				case "joypad1 r":
					code = 77;
					break;
				
				case "joypad1 start":
					code = 13;
					break;
				
				case "joypad1 select":
					code = 16;
					break;
				
				// Player 2
				
				case "joypad2 left":
					code = 70;
					break;
				
				case "joypad2 right":
					code = 72
					break;
				
				case "joypad2 up":
					code = 84;
					break;
				
				case "joypad2 down":
					code = 71;
					break;
				
				case "joypad2 x":
					code = 67;
					break;
				
				case "joypad2 y":
					code = 86;
					break;
				
				case "joypad2 a":
					code = 66;
					break;
				
				case "joypad2 b":
					code = 78;
					break;
				
				case "joypad2 l":
					code = 89;
					break;
				
				case "joypad2 r":
					code = 85;
					break;
				
				case "joypad2 start":
					code = 81;
					break;
				
				case "joypad2 select":
					code = 69;
					break;
			}
			
			return code;
		}
		
		
		/*
		 * Returns true when the key associated with the given description is in a down state,
		 * false otherwise.
		 */
		
		private function getKeyState( keyDescription:String ):Boolean
		{
			var code:uint = this.mapKeyDescription( keyDescription );
			
			if (code == 0)
			{
				// This is a key we don't watch so must be in a released state.
				return false;
			}
			else
			{
				return keyStates[ code ];
			}
		}
		
		
		/*
		 * Returns true when we recognize the keyDescription, false otherwise.
		 */
		
		private function setKeyState( keyDescription:String, keyDown:Boolean ):Boolean
		{
			var code:uint = this.mapKeyDescription( keyDescription );
			
			if (code == 0)
			{
				// No match found.
				return false;
			}
			else
			{
				// See packed event description above.
				keyEvents.push( code | (keyDown as uint) << 8 );
				
				// Local persistence of key state
				keyStates[ code ] = keyDown;
				
				// Looks like a mapped key.
				return true;
			}
		}
	}
}