package utils {
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class FPSCounter extends Sprite {
		
		private var _tf:TextField;
		private var _fmt:TextFormat;
		
		private var ticks:Number = 0;
		
		private var startTime:Number;
		private var lastTime:Number;
		
		public function FPSCounter() {
			_fmt = new TextFormat("_sans", 11, 0xCCCCCC);
			init();
		}
		
		private function init():void {
			_tf = createText();
			addChild(_tf);
			
			ticks = 0;
			startTime = new Date().time;
			
			this.addEventListener(Event.ENTER_FRAME, onTick, false, 0, true);
		}
		
		// Get the time elapsed since last frame, update FPS counter
		private function onTick( e:Event ):void {
			var currentTime:Number = new Date().time;
			var fps:Number = Math.floor(1000/(currentTime - lastTime));
			ticks++;
			var lifetimeAvg:int = Math.round(ticks/((currentTime - startTime)/1000));
			if (!lastTime) lastTime = currentTime;
			_tf.htmlText = "fps = \t" + fps.toString() + "<br/>" + Math.floor(100*fps/50).toString() + "%<br/>avg = " + lifetimeAvg;
			lastTime = currentTime;
		}
		
		private function createText():TextField {
			var t:TextField = new TextField();
			t.width = 0;
			t.height = 0;
			t.autoSize = TextFieldAutoSize.LEFT;
			t.selectable = false;
			t.defaultTextFormat = _fmt;
			t.multiline = true;
			return t;
		}
		
		public function set textColor(col:uint):void {
			_tf.textColor = col;
		}
	}
}