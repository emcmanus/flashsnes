package core
{
	import flash.display.Sprite;
	import flash.display.StageDisplayState;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class EmulatorControl extends Sprite
	{
		// External References
		private var snes9x:Emulator;
		
		// Config
		private var paddingX:int = 15;
		private var paddingY:int = 15;
		
		private var bgWidth:int = 150 + 2 * paddingX;
		private var bgHeight:int = 50 + 2 * paddingY;
		
		// Buttons
		private var pauseButton:Sprite;
		private var paused:Boolean = false;
		
		private var muteButton:Sprite;
		private var muted:Boolean = false;
		
		private var fullscreenButton:Sprite;
		
		public function EmulatorControl( snes9x:Emulator )
		{
			super();
			
			this.snes9x = snes9x;
			
			// Background
			this.graphics.beginFill( 0x0, 0.6 );
			this.graphics.lineStyle( 3, 0xFFFFFF, 0.25 );
			this.graphics.drawRect( 0, 0, bgWidth, bgHeight );
			this.graphics.endFill();
			
			// Pause
			pauseButton = new Sprite;
			
			pauseButton.graphics.beginFill(0x00ff00);
			pauseButton.graphics.lineStyle( 0, 0 );
			pauseButton.graphics.drawRect(0, 0, 50, 20);
			pauseButton.graphics.endFill();
			
			pauseButton.x = paddingX;
			pauseButton.y = paddingY;
			pauseButton.addEventListener( MouseEvent.CLICK, onPauseClick );
			
			addChild( pauseButton );
			
			// Mute
			muteButton = new Sprite;
			
			muteButton.graphics.beginFill( 0x0000FF );
			muteButton.graphics.lineStyle( 0, 0 );
			muteButton.graphics.drawRect( 0, 0, 50, 20 );
			muteButton.graphics.endFill();
			
			muteButton.x = paddingX + 50;
			muteButton.y = paddingY;
			muteButton.addEventListener( MouseEvent.CLICK, onMuteClick );
			
			addChild( muteButton );
			
			// Fullscreen
			fullscreenButton = new Sprite;
			
			fullscreenButton.graphics.beginFill( 0xFF0000 );
			fullscreenButton.graphics.lineStyle( 0, 0 );
			fullscreenButton.graphics.drawRect( 0, 0, 50, 20 );
			fullscreenButton.graphics.endFill();
			
			fullscreenButton.x = paddingX + 100;
			fullscreenButton.y = paddingY;
			fullscreenButton.addEventListener( MouseEvent.CLICK, onFullscreenClick );
			
			addChild( fullscreenButton);
			
			// Key
			var key:TextField = new TextField;
			key.textColor = 0xFFFFFF;
			key.text = "Pause\tmute\t\tfullscreen";
			key.width = 150;
			key.x = paddingX;
			key.y = paddingY + 20;
			
			addChild(key);
		}
		
		
		private function onFullscreenClick( e:Event ):void
		{
			// Toggle
			if ( this.stage.displayState == StageDisplayState.NORMAL )
			{
				this.stage.displayState = StageDisplayState.FULL_SCREEN;
			}
			else
			{
				stage.scaleMode = StageScaleMode.NO_SCALE;
			}
		}
		
		
		private function onPauseClick( e:Event ):void
		{
//			this.snes9x.paused = !paused;
			paused = !paused;
		}
		
		
		private function onMuteClick( e:Event ):void
		{
			// Toggle Mute
			if ( muted )
			{
				trace("unmuting");
//				this.snes9x.cLib.setMute( 0 );
				muted = false;
			}
			else
			{
				trace("muting");
//				this.snes9x.cLib.setMute( 1 );
				muted = true;
			}
		}
	}
}