package core
{
	public interface IEmulator
	{
		function set muted( muted:Boolean ):void;
		function get muted():Boolean;
		
		function set paused( muted:Boolean ):void;
		function get paused():Boolean;
	}
}