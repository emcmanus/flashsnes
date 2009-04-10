package {
	import flash.display.LoaderInfo;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	
	import snesLib.Snes9x;
	import snesLib.video.VideoSurface;
	
	import utils.FPSCounter;
	
	import Profiler;
	
	[SWF(width=512,height=448,frameRate=60)]
	public class Snes extends Sprite {
		
		private var snes9x:Snes9x;
		private var romLoader:URLLoader;
		
		private var surface:VideoSurface;
		private var surfaceContainer:Sprite;
		
		public function Snes(){
			// Build container for input events
			surfaceContainer = new Sprite();
			addChild(surfaceContainer);
			
			// Load the ROM bytes - localhost/test_rom.smc
			this.romLoader = new URLLoader();
			this.romLoader.dataFormat = URLLoaderDataFormat.BINARY;
			this.romLoader.addEventListener( Event.COMPLETE, onRomLoaded );
			
			var paramObj:Object = LoaderInfo(this.root.loaderInfo).parameters;
			var romString:String = parseRom( paramObj ); 
			
			this.romLoader.load(new URLRequest("http://localhost/test_roms/dk_rom_1"));
//			this.romLoader.load(new URLRequest("http://192.168.1.58/test_roms/dk_rom_3"));
//			this.romLoader.load(new URLRequest(romString));
//			this.romLoader.load(new URLRequest("http://localhost/test_roms/1"));
//			this.romLoader.load(new URLRequest("http://localhost/test_roms/test_rom.smc"));
		}
		
		private function onRomLoaded(e:Event):void {
			
			this.snes9x = new Snes9x( romLoader.data );
			this.surface = snes9x.getSurface( 256, 224 );
			
			this.surface.scaleX = 2;
			this.surface.scaleY = 2;
			
			surfaceContainer.addChild( this.surface );
			snes9x.setEventTarget( this.surfaceContainer );
			
			// Monitor FPS performance
			var fps:FPSCounter = new FPSCounter();
			addChild(fps);
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
