package
{

	// http://board.flashkit.com/board/showthread.php?t=677408
	
	public class Profiler
	{
		/********* Private Variables **************/
		// Keep a data record of each "code block" in a "hash-map" object
		private static var blockTotalRecord:Object = new Object();
		// Keep temporary "hash-map" object to keep track of the start times
		// for each block. These values are used for each loop of the block
		private static var blockLoopRecord:Object = new Object();
		// A stack of blocks, allows for nested block loops
		private static var blockStack:Array = new Array();
		
		// last active block to be highligthed
		private static var lastActiveBlock:String = "";
		
		
		
		// Reset Profiler
		public static function reset():void
		{
			blockTotalRecord = new Object();
			blockLoopRecord = new Object();
			blockStack = new Array();
			lastActiveBlock = "";
		}
		
		
		/********* Start Block **************/
		public static function begin(blockName:String):void
		{
			// set the start time for this block of code
			blockLoopRecord[blockName] = new Date().getTime(); 
			// See if we have to set up a record for this block of code
			if(blockTotalRecord[blockName]==undefined || blockTotalRecord[blockName]==null)
				setupBlockRecord(blockName);
			
			// push this block to the top of the stack
			//blockStack.push(blockName);
		}
		
		public static function count(blockName:String):void {
			if ( !blockTotalRecord[blockName] ){
				setupBlockRecord(blockName);
			}
			blockTotalRecord[blockName].iterations++;
		}
		
		/********* End Block **************/
		public static function end(blockName:String):void
		{
			// get the ending time (right away, to get more accurate results)
			var stopTime:Number = new Date().getTime();
			
			// if our block stack is empty, this call is an error
			/*
			if(blockStack.length <= 0) {
				trace("ERROR => d3s.profiler.Profiler: Called to end() when no block was begun.");
				return;
			}
			*/
			// find out what block we are ending
			//var blockName:String = String(blockStack.pop());
			
			if(blockName != "FPS")
				lastActiveBlock = blockName;
			
			// calculate the time taken, and add it to the total record
			var timeTaken:Number = stopTime - blockLoopRecord[blockName]; 
			var record:Object = blockTotalRecord[blockName];
			if(!record)
				return;

			record.lastTime = timeTaken;
			record.totalTimeElapsed += timeTaken;
			record.iterations += 1;
			if(record.longestTime < timeTaken)
				record.longestTime = timeTaken;
			if(record.shortestTime > timeTaken)
				record.shortestTime = timeTaken;
			
			// See if this block has a parent :)
			if(blockStack.length > 0) {
				record.parent = blockStack[blockStack.length-1];
			}
		}
		
		/********* Get Block **************/
		// Get a record set of data for a specified block
		public static function getBlockRecord(blockName:String):Object
		{
			// if there is none associated, notify it
			if(blockTotalRecord[blockName]==undefined || blockTotalRecord[blockName]==null) {
				trace("ERROR => d3s.profiler.Profiler: Tried to get a record for non-existent block ("+blockName+")");
				return null;
			}
			// return the record
			return blockTotalRecord[blockName];
		}
		
		/********* Get Block Key Set **************/
		// Returns an array of all the block names in the profiler
		public static function getBlockNames():Array
		{
			var blockNames:Array = new Array();
			// loop through and get them all
			for(var blockName:Object in blockTotalRecord) {
				blockNames.push(blockName);
			}
			return blockNames;
		}
		
		/********* Text Report **************/
		// Returns a string representation of the data in the profiler
		public static function report():String
		{
			// we need to first get a list of all the blocks in the profiler
			var blockNames:Array = getBlockNames();
			// set up the string for the reporting
			var out:String = "";
			out += "Total           #        Average      Longest      Shortest        Last         Block \n";
			out += "Time(s)   Loops   Time(ms)   Time(ms)    Time(ms)   Time(ms)   Name \n";
			out += "----------------------------------------------------------------------------------------\n";
			/* Output Format:
			Total        #      Average     Longest     Shortest       Last     Block
			Time (s)   Loops   Time (ms)   Time (ms)    Time (ms)   Time (ms)   Name
			*/
			// now loop through all the blocks and put in some data
			var record:Object;
			for(var i:Number = 0; i < blockNames.length; i++)
			{
				/*if(blockNames[i] == lastActiveBlock)
					out += "<b>";*/
				
				record = getBlockRecord(blockNames[i]);
				out += formatNumber(record.totalTimeElapsed/1000, 2, 6);
				out += formatNumber(record.iterations,       0, 10);
				out += formatNumber( (record.totalTimeElapsed/record.iterations), 2, 15);
				out += formatNumber(record.longestTime,      2, 15);
				out += formatNumber(record.shortestTime,     2, 15);
				out += formatNumber(record.lastTime,         2, 15);
				out += "            " + blockNames[i] + "\n";
				
				/*if(blockNames[i] == lastActiveBlock)
					out += "</b>";*/
			}
			return out;
		}
		
		/********* Print Report *************/
		public static function printReport():void {
			trace( "printReport" );
			trace( report() );
		}
		
		/********* Setup Block **************/
		// Set up a record with default values for the given block name
		private static function setupBlockRecord(blockName:String):void
		{
			// set up default values for the block record
			blockTotalRecord[blockName] = new Object();
			var record:Object = blockTotalRecord[blockName];
			record.totalTimeElapsed = 0;
			record.iterations = 0;
			record.longestTime = 0;
			record.shortestTime = 999999;
			record.lastTime = 0;
			record.parent = null;
		}
		
		// NOTE! Below code is taken from the original ASProf class by 	by David Chang => email: dchang@nochump.com
		// returns string of number n, set to d decimal places and left padded with spaces to have length p
		// note: doesn't round the decimal but it's better than doing pow, round, and divide
		private static function formatNumber(num:Number, precision:Number, padding:Number):String
		{
			var string:String = num + "";
			var i:int = string.indexOf(".");
			if(i > -1) string = string.slice(0, i + precision + 1);
			var l:int = string.length;
			while (l++ < padding) {
				string = " " + string;
			}
			return string;
		}
		
	}
}


// Exemple code use
/*
Profiler.begin("my name");
for(var i:Number = 0; i < 10000; i++) {
     // Inner block of code
     Profiler.begin("inner code");
     // math
    Math.sqrt(102);
    Profiler.end();
}
Profiler.end();

// Display it
trace(Profiler.report());
*/


/*
// This goes in the main frame code section.

Profiler.begin("FPS");
// Main loop
onEnterFrame = function() {
     Profiler.end();
     Profiler.begin("FPS");

     // Calculate the fps
     var fps:Number = Math.round(1000 / Profiler.getBlockRecord("FPS").lastTime);
      // now do what you want with that number
}
*/

