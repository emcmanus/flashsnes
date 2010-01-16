package snesLib {
	
	import cmodule.libSNES9x.CLibInit;
	
	import flash.display.DisplayObject;
	import flash.utils.ByteArray;
	
	import snesLib.events.ListenerManager;
	import snesLib.video.VideoSurface;
	
	/**
	 * This class contans the public interface for an SDL application.
	 */
	public class Snes9x {
		
		/** @private */
		protected var videoSurface:VideoSurface;
		
		/** @private */
		public var eventManager:ListenerManager;
		
		/** @default 320 */
		protected var SnesWidth:int = 320;
		
		/** @default 240 */
		protected var SnesHeight:int = 240;
		
		/**
		 * @private
		 * 
		 * Loads and initializes the C library.
		 */
		internal var cLoader:CLibInit;
		
		/**
		 * Reference to the initialized C application. Contains available C callbacks.
		 */
		public var cLib:Object;
		
		/** @private */
		private var romData:ByteArray;
		

		/**
		 * Constructor. Following construction, use getSurface() to build an SDL video surface,
		 * and setEventTarget() to receive user input.
		 * 
		 * <p>If your application requires some special initialization process, add it here.</p>
		 */
		public function Snes9x( romData:ByteArray ){
			
			this.romData = romData;
			
			cLoader = new CLibInit();
			cLoader.supplyFile( 'romfile', romData );
			
			cLib = cLoader.init();
			cLib.setup();
		}
		
		/**
		 * Pause or unpause emulator
		 */
		public function set paused( paused:Boolean ):void
		{
			this.videoSurface.updateTimer.paused = paused;
		}
		
		/**
		 * Initializes an video surface and attaches required event listeners. Returned
		 * bitmap must be added to the display hierarchy.
		 * 
		 * <p>Application-specific code required to initialize the display should be added here.</p>
		 * 
		 * @param	displayWidth	The video surface desired width. [Optional]
		 * @param	displayHeight	The video surface desired height. [Optional]
		 * 
		 * @return	A bitmap mapped to the SDL Video Surface.
		 */
		public function getSurface( width:int=256, height:int=224 ):VideoSurface {
			if (!videoSurface) {
				
				this.SnesWidth = width;
				this.SnesHeight = height;
				
				videoSurface = new VideoSurface( this, width, height );
			}
			return videoSurface;
		}
		
		/**
		 * Registers the given display object to receive keypress and mouse events. MouseMove events are
		 * recorded relative to this display object.
		 * 
		 * @param	eventTarget	The display object to register for keypress and mouse events.
		 */
		public function setEventTarget( eventTarget:DisplayObject ):void {
			if (!eventManager){
				eventManager = new ListenerManager( eventTarget );
				cLib.setEventManager( eventManager );	// pass manager reference to c lib for event retrieval
			}
		}
	}
}