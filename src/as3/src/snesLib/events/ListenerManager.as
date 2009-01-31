package snesLib.events
{
	import structures.LinkedQueue;
	
	import flash.display.DisplayObject;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Mouse;
	
	/**
	 * @private
	 * 
	 * Event Queue interface. Sets keyboard/mouse event listeners on a Display Object,
	 * and exposes a set of C-accessible methods for retreiving past events.
	 */
	public class ListenerManager
	{
		private var eventTarget:DisplayObject;
		private var eventQueue:QueueCollection;
		
		/*
		 * Must be a display object which supports Mouse and Keypress events.
		 */
		public function ListenerManager( eventTarget:DisplayObject ) {
			
			this.eventTarget = eventTarget;
			this.eventQueue = new QueueCollection();
			
			// Mouse
			eventTarget.addEventListener( MouseEvent.MOUSE_OVER, onMouseOver );
			eventTarget.addEventListener( MouseEvent.MOUSE_OUT, onMouseOut );
			eventTarget.addEventListener( MouseEvent.MOUSE_MOVE, onMouseMove );
			eventTarget.addEventListener( MouseEvent.MOUSE_DOWN, onMouseDown );
			eventTarget.addEventListener( MouseEvent.MOUSE_UP, onMouseUp );
			
			// Keyboard
			eventTarget.stage.addEventListener( KeyboardEvent.KEY_DOWN, onKeyDown );
			eventTarget.stage.addEventListener( KeyboardEvent.KEY_UP, onKeyUp );
		}
		
		
		/*
		 * C Interface - Return all key & mouse events which have occured since the last C poll
		 */
		
		public function pumpKeyEvents():LinkedQueue {
			return eventQueue.pumpKeyEvents();
		}
		
		public function pumpMouseEvents():LinkedQueue {
			return eventQueue.pumpMouseEvents();
		}
		
		public function pumpMousePosition():Object {
			return eventQueue.pumpMousePosition();
		}
		
		
		/*
		 * Handlers - Mouse/Keyboard
		 */
		
		private function onMouseOver( e:MouseEvent ):void {
			Mouse.hide();
		}
		
		private function onMouseOut( e:MouseEvent ):void {
			Mouse.show();
		}
		
		private function onMouseMove( e:MouseEvent ):void {
			eventQueue.setMousePosition( e );
		}
		
		private function onMouseDown( e:MouseEvent ):void {
			eventQueue.enqueueMouseEvent( e, true );
		}
		
		private function onMouseUp( e:MouseEvent ):void {
			eventQueue.enqueueMouseEvent( e, false );
		}
		
		private function onKeyDown( e:KeyboardEvent ):void {
			eventQueue.enqueueKeyEvent( e, true );
		}
		
		private function onKeyUp( e:KeyboardEvent ):void {
			eventQueue.enqueueKeyEvent( e, false );
		}
	}
}