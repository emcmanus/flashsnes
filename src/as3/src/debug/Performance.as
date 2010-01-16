package debug
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	public class Performance extends Sprite
	{
		private var output:TextField;
		
		// FPS
		private var frameCount:Number = 0;
		private var startTime:Number;
		private var lastTime:Number;
		
		public function Performance()
		{
			super();
			
			frameCount = 0;
			startTime = new Date().time;
			
			output = new TextField;
			output.selectable = false;
			output.defaultTextFormat = new TextFormat("_sans", 11, 0xCCCCCC);
			output.multiline = true;
			output.width = 300;
			output.height = 150;
			
			addChild(output);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(e:Event):void
		{
			var lifetimeAvg:int;
			var currentTime:Number;
			var fps:Number;
			
			frameCount++;
			
			currentTime = new Date().time;
			fps = Math.floor(1000/(currentTime - lastTime));
			
			lifetimeAvg = Math.round(frameCount/((currentTime - startTime)/1000));
			output.htmlText = "fps = \t" + fps.toString() + "<br/>" + Math.floor(100*fps/50).toString() + "%<br/>avg = " + lifetimeAvg;
			
			lastTime = currentTime;
		}
	}
}