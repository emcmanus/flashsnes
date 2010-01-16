package core
{
	import flash.display.Sprite;
	import flash.events.Event;

	/**
	 * Wraps an emulator instance and varies its parameters based on its performance
	 */
	
	public class PerformanceAdapter
	{
		// Emulator
		private var emulator:Emulator;
		
		// Performance Metrics
		private var frameCount:int;
		
		// Moving Average
		private var movingAvg:Number;
		
		private var samples:Array;		// Queue for frame durations in ms
		private var sampleTotal:Number;	// Sum of all durations in the queue
		private var window:int = 30;	// Number of samples to collect
		
		private var tmpSampleCount:int;	// We'll re-adjust the emulator settings every window/2 samples
		
		private var time1:Number;	// Last
		private var time2:Number;	// Current
		
		// Fixed State
		private var muted:Boolean = false;
		
		// Config
		private const MIN_AUDIO_FPS:int = 35;	// Threshold framerate for Audio
		
		
		////////////////////////////////////////////////////////////
		
		
		public function PerformanceAdapter()
		{
			reset();
			(new Sprite).addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		
		public function wrapEmulator( emulator:Emulator ):void
		{
			reset();
			this.emulator = emulator;
		}
		
		
		////////////////////////////////////////////////////////////
		
		
		private function onEnterFrame( e:Event ):void
		{
			frameCount++;
			tmpSampleCount++;
			
			time2 = (new Date).time;
			
			samples.push( time2 - time1 );
			sampleTotal += time2 - time1;
			
			time1 = time2;
			
			// Update window
			if ( samples.length > window )
			{
				sampleTotal -= samples.shift();
			}
			
			// Update average
			movingAvg = (samples.length / sampleTotal) * 1000;
			
			if ( tmpSampleCount > window/2 )
			{
				updateEmulatorSettings();
				tmpSampleCount = 0;
			}
		}
		
		
		private function updateEmulatorSettings():void
		{
			if ( !muted )
			{
				// Sound sampling adapter
				var frameSampleRate:Number = Math.round(((44100/movingAvg)*0.80)/10)*10;	// balanced sample rate rounded to the nearest 10
				emulator.requestedSamples = frameSampleRate;
				
				// Mute logic
				if ( movingAvg < MIN_AUDIO_FPS )
				{
					// Mute
					this.emulator.muted = true;
					muted = true;
					
					// TODO user notification
				}
			}
		}
		
		
		private function reset():void
		{
			time1 = (new Date).time;
			time2 = (new Date).time;
			
			samples = new Array();
			
			tmpSampleCount = 0;
			sampleTotal = 0;
			frameCount = 0;
		}
	}
}