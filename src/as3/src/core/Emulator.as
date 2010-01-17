package core
{
	import cmodule.libSNES9x.CLibInit;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Stage;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.SampleDataEvent;
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
		
		// Config
		private var displayWidth:int = 256;
		private var displayHeight:int = 224;
		
		////////////////////////////////////////////////////////////
		
		/*
		 *	Constructor
		 */
		public function Emulator( rom:ByteArray, stage:Stage )
		{
			var ram_NS:Namespace;
			
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
		}
		
		
		public function set muted( muted:Boolean ):void
		{
			mute = muted;
			cLib.setMute( muted as int );
		}
		public function get muted():Boolean
		{
			return mute;
		}
		
		public function set paused( paused:Boolean ):void
		{
			pause = paused;
		}
		public function get paused():Boolean
		{
			return pause;
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
		
		
	}
}