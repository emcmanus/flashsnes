package snesLib.events {
	import structures.LinkedQueue;
	
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	
	/**
	 * @private
	 * 
	 * A collection of event queues for FIFO reads.
	 */
	internal class QueueCollection {
		
		private var mousePosition:Object;
		private var mouseClickEvents:LinkedQueue;
		private var keyEvents:LinkedQueue;
		
		/*
		 * Uses Polygonal's LinkedQueue Data Structure for enqueuing events. More fun than using arrays and shift!
		 */
		public function QueueCollection() {
			mouseClickEvents = new LinkedQueue();
			keyEvents = new LinkedQueue();
		}
		
		/*
		 * C Interface mapped through ListenerManager
		 */
		internal function pumpMousePosition():Object {
			return mousePosition;
		}
		
		internal function pumpMouseEvents():LinkedQueue {
			return mouseClickEvents;
		}
		
		internal function pumpKeyEvents():LinkedQueue {
			return keyEvents;
		}
		
		/*
		 * Modifiers
		 */
		internal function enqueueMouseEvent( e:MouseEvent, pressed:Boolean ):void {
			mouseClickEvents.enqueue( uint(pressed) );
		}
		
		internal function enqueueKeyEvent( e:KeyboardEvent, pressed:Boolean ):void {
			keyEvents.enqueue( e.keyCode | (uint(pressed) << 8) );							// Packed event format: 9th bit for press/release, lower 8 bits for scan code
		}
		
		internal function setMousePosition( e:MouseEvent ):void {
			mousePosition = { x: e.localX, y: e.localY };									// Only store most-current position 
		}
	}
}