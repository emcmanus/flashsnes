package {
	import flash.display.LoaderInfo;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.SampleDataEvent;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import snesLib.Snes9x;
	import snesLib.gui.EmulatorControl;
	import snesLib.video.VideoSurface;
	
	import utils.FPSCounter;
	
	[SWF(frameRate="60",backgroundColor="0x000000")]
	public class Snes extends Sprite {
		
		private var snes9x:Snes9x;
		private var romLoader:URLLoader;
		
		private var surface:VideoSurface;
		private var surfaceContainer:Sprite;
		
		// Config
		private var integerScale:Boolean = true;	// Only scale the display surface by a whole integer to prevent interpolation
		
		// GUI
		private var emulatorControl:EmulatorControl;
		
		// Constants
		private const ORIGINAL_SURFACE_WIDTH:int = 256;
		private const ORIGINAL_SURFACE_HEIGHT:int = 224;
		
		private const ROM_PREFIX:String = "http://localhost/test_roms/snes/";
		
		// Sound object we'll stream audio to
		protected var soundOut:Sound;
		protected var soundBuffer:ByteArray;	// AS3's sound buffer, not C's
		protected var soundChannel:SoundChannel;	// We use this to restart the sound object
		protected var soundBufferAddress:Number;
		
		public function Snes()
		{
			// Container for display buffer and input events
			this.surfaceContainer = new Sprite();
			addChild( this.surfaceContainer );
			
			// Load the ROM bytes - localhost/test_rom.smc
			this.romLoader = new URLLoader();
			this.romLoader.dataFormat = URLLoaderDataFormat.BINARY;
			this.romLoader.addEventListener( Event.COMPLETE, onRomLoaded );
			
			var paramObj:Object = LoaderInfo(this.root.loaderInfo).parameters;
			var romString:String = parseRom( paramObj );
			
			// Resize prefs
			this.stage.addEventListener(Event.RESIZE, onResize);
			this.stage.align = StageAlign.TOP_LEFT;
			this.stage.quality = StageQuality.BEST;
			this.stage.scaleMode = StageScaleMode.NO_SCALE;
			
			// Sound setup
			this.soundOut = new Sound;
			this.soundOut.addEventListener( SampleDataEvent.SAMPLE_DATA, onSoundSampleData );
			this.soundBuffer = new ByteArray;
			
			// TODO check URL of SWF
			
			// Check rom location
			if ( verifyRomPath(romString) )
			{
				this.romLoader.load(new URLRequest(ROM_PREFIX + romString));
			}
			else
			{
				// TODO Display error
				// call home?
				throw new Error( "Invalid ROM path. Nice try, hacker!" );
			}
		}
		
		// Tick
		protected function onEnterFrame(e:Event):void
		{
			trace("enter frame");
			if (!this.soundChannel)
			{
//				trace("\nrestart sound channel");
				
				this.soundChannel = soundOut.play();
				this.soundChannel.addEventListener(Event.SOUND_COMPLETE, onSoundComplete);
			}
		}
		
		
		protected function onSoundComplete(e:Event):void
		{
//			trace("sound complete");
			
			this.soundChannel.removeEventListener(Event.SOUND_COMPLETE, onSoundComplete);
			this.soundChannel = null;
		}
		
		
		protected function onSoundSampleData( e:SampleDataEvent ):void
		{
			trace("pump");
			// When the sound object wants more data...
			// Tell flash to empty its buffer into our sound object
			e.data.endian = Endian.LITTLE_ENDIAN;
			snes9x.cLib.paintSound( e.data );
			
//			var oldPos:uint = e.data.position;
//			
//			trace("Data:");
//			
//			// Let's peek into the data
//			for ( var i:int=0; i<50; i++ )
//			{
//				e.data.position = i * 4;	// floats
//				trace( e.data.readFloat() );
//			}
//			
//			e.data.position = oldPos;
		}
		
		
		private function verifyRomPath(path:String):Boolean
		{
			// General checks
//			if (path.length > 32)	// using MD5s to generate key name
//			{
//				return false;
//			}
//			
//			// Only allow alphanumeric paths
//			for (var i:int = 0; i<path.length; i++)
//			{
//				
//				if ( path.charAt(i).search(/[0-9a-zA-Z]/g) < 0 )	// encountered non-alphanumeric character
//				{
//					return false;
//				}
//			}
			
			return true;
		}
		
		
		private function onResize(e:Event=null):void 
		{
			// ... Not really necessary yet
			
			// Move Suface Container
			this.surfaceContainer.x = 0;
			this.surfaceContainer.y = 0;
			
			// Resize surface to be the largest integer multiple that fits inside the stage
			var scaleX:Number, scaleY:Number, scale:Number, integerScale:int;
			
			if ( surface && stage.contains( surface ) )
			{
				scaleX = stage.stageWidth / ORIGINAL_SURFACE_WIDTH;
				scaleY = stage.stageHeight / ORIGINAL_SURFACE_HEIGHT;
				
				scale = Math.min( scaleX, scaleY );
				integerScale = Math.floor( scale );
				
				if ( this.integerScale )
				{
					this.surface.scaleX = integerScale;
					this.surface.scaleY = integerScale;
				}
				else
				{
					this.surface.scaleX = scale;
					this.surface.scaleY = scale;
				}
				
				// Center
				this.surface.x = Math.floor((stage.stageWidth - this.surface.width)/2);
				this.surface.y = Math.floor((stage.stageHeight - this.surface.height)/2);
			}
			
			if ( emulatorControl )
			{
				// Center Control
				this.emulatorControl.x = Math.floor((stage.stageWidth - this.emulatorControl.width)/2);
				this.emulatorControl.y = Math.floor((stage.stageHeight - this.emulatorControl.height)/2);
			}
		}
		
		private function onRomLoaded(e:Event):void
		{
			
			this.snes9x = new Snes9x( romLoader.data );
			this.surface = snes9x.getSurface( ORIGINAL_SURFACE_WIDTH, ORIGINAL_SURFACE_HEIGHT );
			
			this.surface.scaleX = 2;
			this.surface.scaleY = 2;
			
			emulatorControl = new EmulatorControl( snes9x );
//			this.addChild( emulatorControl );
			
			surfaceContainer.addChild( this.surface );
			snes9x.setEventTarget( this.surfaceContainer );
			
			// Monitor FPS performance
			var fps:FPSCounter = new FPSCounter();
			addChild(fps);
			
			onResize();
			
			// Sound Handler
			parent.stage.addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		
		
		public function parseRom(paramObj:Object):String
		{
			var keyStr:String = "";
			var valueStr:String = "";
			
			for (keyStr in paramObj) {
				valueStr = String(paramObj[keyStr]);
				
				switch(keyStr)
				{
					case "rom":
						return valueStr;
						break;
				}
			}
			return "";
		}
	}
}
