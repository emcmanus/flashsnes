package snesLib.events {
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	
	import structures.LinkedQueue;
	
	/**
	 * @private
	 * 
	 * A collection of event queues for FIFO reads.
	 */
	internal class QueueCollection {
		
		private var mousePosition:Object;
		private var mouseClickEvents:LinkedQueue;
		private var keyEvents:LinkedQueue;
		private var knownKeyStates:Array;
		
		/*
		 * Uses Polygonal's LinkedQueue Data Structure for enqueuing events. More fun than using arrays and shift!
		 */
		public function QueueCollection()
		{
			mouseClickEvents = new LinkedQueue();
			keyEvents = new LinkedQueue();
			
			// Initialize keystates array
			knownKeyStates = new Array( 256 );
			for ( var i:int = 0; i<knownKeyStates.length; i++ )
			{
				knownKeyStates[i] = 0;
			}
		}
		
		/*
		 * C Interface mapped through ListenerManager
		 */
		internal function pumpMousePosition():Object
		{
			return mousePosition;
		}
		
		internal function pumpMouseEvents():LinkedQueue
		{
			return mouseClickEvents;
		}
		
		internal function pumpKeyEvents():LinkedQueue
		{
//			trace("pumping key events");
			return keyEvents;
		}
		
		/*
		 * Modifiers
		 */
		internal function enqueueMouseEvent( e:MouseEvent, pressed:Boolean ):void
		{
			mouseClickEvents.enqueue( uint(pressed) );
		}
		
		internal function enqueueKeyEvent( e:KeyboardEvent, pressed:Boolean ):void
		{
			// Only report new events
			if ( knownKeyStates[ e.keyCode ] != pressed )
			{
				knownKeyStates[ e.keyCode ] = pressed;
				keyEvents.enqueue( e.keyCode | (uint(pressed) << 8) );							// Packed event format: 9th bit for press/release, lower 8 bits for scan code
			}
		}
		
		internal function setMousePosition( e:MouseEvent ):void
		{
			mousePosition = { x: e.localX, y: e.localY };									// Only store most-current position
		}
	}
}