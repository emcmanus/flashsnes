package utils {
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class FPSCounter extends Sprite {
		
		private var _tf:TextField;
		private var _fmt:TextFormat;
		
		private var lastTime:Number;
		
		public function FPSCounter() {
			_fmt = new TextFormat("_sans", 11, 0xCCCCCC);
			init();
		}
		
		private function init():void {
			_tf = createText();
			addChild(_tf);
			this.addEventListener(Event.ENTER_FRAME, onTick, false, 0, true);
		}
		
		// Get the time elapsed since last frame, update FPS counter
		private function onTick( e:Event ):void {
			var currentTime:Number = new Date().time;
			if (!lastTime) lastTime = currentTime;
			_tf.text = "fps = " + (1000/(currentTime - lastTime)).toString();
			lastTime = currentTime;
		}
		
		private function createText():TextField {
			var t:TextField = new TextField();
			t.width = 0;
			t.height = 0;
			t.autoSize = TextFieldAutoSize.LEFT;
			t.selectable = false;
			t.defaultTextFormat = _fmt;
			return t;
		}
		
		public function set textColor(col:uint):void {
			_tf.textColor = col;
		}
	}
}