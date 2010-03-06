package
{
	import core.Emulator;
	import core.PerformanceAdapter;
	
	import gui.PlayerControls;
	
	import debug.Performance;
	
	import flash.display.Bitmap;
	import flash.display.LoaderInfo;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageQuality;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	
	[SWF(frameRate="50",backgroundColor="0x000000")]
	public class Snes extends Sprite
	{
		// Config
		private const ROM_PREFIX:String = "http://localhost/test_roms/snes/";
		private const ORIGINAL_SURFACE_WIDTH:int = 256;
		private const ORIGINAL_SURFACE_HEIGHT:int = 224;
		
		private var integerScale:Boolean = true;
		
		////////////////////////////////////////////////////////////
		
		// Emulator
		private var emulator:Emulator;
		
		// Rom Loader
		private var romLoader:URLLoader;
		private var romLocation:String;
		
		// Display
		private var screen:Bitmap;
		
		// Interface
		private var playerControls:PlayerControls;
		private var performance:Performance;
		
		////////////////////////////////////////////////////////////
		
		/*
		 * Constructor
		 */
		public function Snes()
		{
			super();
			
			// Flashvars
			parseFlashVars( LoaderInfo(this.root.loaderInfo).parameters );
			
			// Load remote ROM file
			romLoader = new URLLoader();
			romLoader.dataFormat = URLLoaderDataFormat.BINARY;
			romLoader.addEventListener( Event.COMPLETE, onRomLoaded );
			
			// Resize prefs
			stage.addEventListener(Event.RESIZE, onResize);
			stage.align = StageAlign.TOP_LEFT;
			stage.quality = StageQuality.BEST;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			// Check rom location
			if ( verifyRomPath(romLocation) )
			{
				romLoader.load(new URLRequest( ROM_PREFIX + romLocation ));
			}
			else	// TODO Report error in GUI -- call home?
			{
				throw new Error( "Invalid ROM path. Nice try, hacker!" );
			}
			
		}
		
		
		private function onRomLoaded(e:Event):void
		{
			// Emulator
			emulator = new Emulator( romLoader.data, stage );
			screen = emulator.screen;
			
			// Performance Adapter
			var adapter:PerformanceAdapter = new PerformanceAdapter;
			adapter.wrapEmulator(emulator);
			
			// Interface
			playerControls = new PlayerControls( emulator );
			performance = new Performance();
			
			// Show
			addChild( screen );
			addChild( performance );
//			addChild( playerControls );
			
			onResize();
		}
		
		
		private function onResize(e:Event=null):void
		{
			// Resize surface to be the largest integer multiple that fits inside the stage
			var scaleX:Number, scaleY:Number, scale:Number, integerScale:int;
			
			if ( screen && stage.contains( screen ) )
			{
				scaleX = stage.stageWidth / ORIGINAL_SURFACE_WIDTH;
				scaleY = stage.stageHeight / ORIGINAL_SURFACE_HEIGHT;
				
				scale = Math.min( scaleX, scaleY );
				integerScale = Math.floor( scale );
				
				if ( this.integerScale )
				{
					screen.scaleX = integerScale;
					screen.scaleY = integerScale;
				}
				else
				{
					screen.scaleX = scale;
					screen.scaleY = scale;
				}
				
				// Center
				screen.x = Math.floor((stage.stageWidth - screen.width)/2);
				screen.y = Math.floor((stage.stageHeight - screen.height)/2);
			}
			
			if ( this.playerControls )
			{
				// Center Control
				this.playerControls.x = Math.floor((stage.stageWidth - this.playerControls.width)/2);
				this.playerControls.y = Math.floor((stage.stageHeight - this.playerControls.height)/2);
			}
		}
		
		
		////////////////////////////////////////////////////////////
		
		/*
		 * Helpers
		 */
		
		private function verifyRomPath( location:String ):Boolean
		{
			// 			General checks
			//			if (path.length > 32)	// using MD5s to generate key name
			//			{
			//				return false;
			//			}
			//			
			//			// Only allow alphanumeric paths
			//			for (var i:int = 0; i<path.length; i++)
			//			{
			//				if ( path.charAt(i).search(/[0-9a-zA-Z]/g) < 0 )	// encountered non-alphanumeric character
			//				{
			//					return false;
			//				}
			//			}
			
			if ( location.length )
				return true;
			else
				return false;
		}
		
		
		private function parseFlashVars( flashVars:Object ):void
		{
			var keyStr:String = "";
			var valueStr:String = "";
			
			for (keyStr in flashVars) {
				valueStr = String(flashVars[keyStr]);
				
				switch(keyStr)
				{
					case "rom":
						romLocation = valueStr;
						break;
				}
			}
		}
	}
}