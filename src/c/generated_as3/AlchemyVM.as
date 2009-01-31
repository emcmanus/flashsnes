
/*
*   This file contains the Alchemy VM.
* 
*   By separating this code we can make changes to the VM and regenerate the source without losing any work. It'll simply be prepended to the beginning of the generated output.
*   
*/

package cmodule.libSNES9x {

// Start of file scope inline assembly
import flash.utils.*
import flash.display.*
import flash.text.*
import flash.events.*
import flash.net.*
import flash.system.*


public var gdomainClass:Class;
public var gshell:Boolean = false;

public function establishEnv():void
{
  try
  {
    var ns:Namespace = new Namespace("avmplus");
  
    gdomainClass = ns::["Domain"];
    gshell = true;
  }
  catch(e:*) {}
  if(!gdomainClass)
  {
    var ns:Namespace = new Namespace("flash.system");
  
    gdomainClass = ns::["ApplicationDomain"];
  }
}

establishEnv();

public var glogLvl:int = Alchemy::LogLevel;

public function log(lvl:int, msg:String):void
{
  if(lvl < glogLvl)
    trace(msg);
}

class LEByteArray extends ByteArray
{
  public function LEByteArray()
  {
    super.endian = "littleEndian";
  }

  public override function set endian(e:String):void
  {
    throw("LEByteArray endian set attempted");
  }
}

class GLEByteArrayProvider
{
  public static function get():ByteArray
  {
    var result:ByteArray;

    try
    {
      result = gdomainClass.currentDomain.domainMemory;
    }
    catch(e:*) {}

    if(!result)
    {
      result = new LEByteArray;
      try
      {
        result.length = gdomainClass.MIN_DOMAIN_MEMORY_LENGTH;
        gdomainClass.currentDomain.domainMemory = result;
      }
      catch(e:*)
      {
        log(3, "Not using domain memory");
      }
    }
    return result;
  }
}

public class MemUser
{
  public final function _mr32(addr:int):int { gstate.ds.position = addr; return gstate.ds.readInt(); }
  public final function _mru16(addr:int):int { gstate.ds.position = addr; return gstate.ds.readUnsignedShort(); }
  public final function _mrs16(addr:int):int { gstate.ds.position = addr; return gstate.ds.readShort(); }
  public final function _mru8(addr:int):int { gstate.ds.position = addr; return gstate.ds.readUnsignedByte(); }
  public final function _mrs8(addr:int):int { gstate.ds.position = addr; return gstate.ds.readByte(); }
  public final function _mrf(addr:int):Number { gstate.ds.position = addr; return gstate.ds.readFloat(); }
  public final function _mrd(addr:int):Number { gstate.ds.position = addr; return gstate.ds.readDouble(); }
  public final function _mw32(addr:int, val:int):void { gstate.ds.position = addr; gstate.ds.writeInt(val); }
  public final function _mw16(addr:int, val:int):void { gstate.ds.position = addr; gstate.ds.writeShort(val); }
  public final function _mw8(addr:int, val:int):void { gstate.ds.position = addr; gstate.ds.writeByte(val); }
  public final function _mwf(addr:int, val:Number):void { gstate.ds.position = addr; gstate.ds.writeFloat(val); }
  public final function _mwd(addr:int, val:Number):void { gstate.ds.position = addr; gstate.ds.writeDouble(val); }
}

public const gstackSize:int = 1024 * 1024;

public class MState extends MemUser
{
 
  public const ds:ByteArray = (gstate == null || gstate.ds == null) ? GLEByteArrayProvider.get() : gstate.ds;

 
Alchemy::NoVector {
  public const funcs:Array = (gstate == null) ? [null] : gstate.funcs;
}
Alchemy::Vector {
  public var funcs:Vector.<Object> = (gstate == null) ? new Vector.<Object>(1) : gstate.funcs;
}

 
  public const syms:Object = (gstate == null) ? {} : gstate.syms;
 
  public var system:CSystem = (gstate == null) ? null : gstate.system;

 
  public var esp:int;
  public var ebp:int;
  public var eax:int;
  public var edx:int;
  public var st0:Number;
  public var cf:uint;

 
  public var gworker:Machine;

  public function MState(machine:Machine)
  {
    if(machine)
    {
      gworker = machine;
      gworker.mstate = this;
    }
   
    if(gstate == null)
    {
      ds.length += gstackSize;
      esp = ds.length;
    }
  }

  public function push(v:int):void
  {
    esp -= 4;
    _mw32(esp, v);
  }
  
  public function pop():int
  {
    var v:int = _mr32(esp);
  
    esp += 4;
    return v;
  }

  public function copyTo(state:MState):void
  {
    state.esp = esp;
    state.ebp = ebp;
    state.eax = eax;
    state.edx = edx;
    state.st0 = st0;
    state.cf = cf;
    state.gworker = gworker;
  }
}

public var gpreStaticInits:Array;

public function regPreStaticInit(f:Function):void
{
  if(!gpreStaticInits)
    gpreStaticInits = [];

  gpreStaticInits.push(f);
}

public function modPreStaticInit():void
{
  if(gpreStaticInits)
    for(var i:int = 0; i < gpreStaticInits.length; i++)
      gpreStaticInits[i]();
}

public var gpostStaticInits:Array;

public function regPostStaticInit(f:Function):void
{
  if(!gpostStaticInits)
    gpostStaticInits = [];

  gpostStaticInits.push(f);
}

public function modPostStaticInit():void
{
  if(gpostStaticInits)
    for(var i:int = 0; i < gpostStaticInits.length; i++)
      gpostStaticInits[i]();
}



// Enable Simple Profiler?

/*import Profiler;
var profiler:* = Profiler;
var regCount:int = 0;
public function regFunc(fsm:Function, optTitle:String=""):int
{
    var fsmName:String = optTitle + regCount.toString();
    var newFSM:Function = function( ... params ):* {
        profiler.begin( fsmName );
        var returnVal:* = fsm.apply(null, params);
        profiler.end( fsmName );
        return returnVal;
    }
  regCount++;
  return gstate.funcs.push(newFSM) - 1;
}*/


public function regFunc( fsm:Function ):int {
    return gstate.funcs.push(fsm) - 1;
}



/*
 * Print profiler info on enterFrame
 */
var frameTimer:Sprite = new Sprite();
frameTimer.addEventListener( "enterFrame", function(e:Event){
	Profiler.printReport();
});


public function unregFunc(n:int):void
{
 
  if(n+1 == gstate.funcs.length)
    gstate.funcs.pop();
}

public function importSym(s:String):int
{
  var res:int = gstate.syms[s];

  if(!res)
  {
    log(3, "Undefined sym: " + s);
   
   
    return exportSym(s, regFunc(function() {
      throw("Undefined sym: " + s);
    }));
  }
  return res;
}

public function exportSym(s:String, a:int):int
{
  gstate.syms[s] = a;
  return a;
}

public class StaticInitter
{
  var ptr:int = 0;
 
  public function alloc(size:int, align:int):int
  {
    if(!align)
      align = 1;
   
    ptr = ptr ? ptr : gstate.ds.length ? gstate.ds.length : 1024;
   
    ptr = (ptr + align - 1) & ~(align - 1);
   
    var result:int = ptr;
   
    ptr += size;
   
    gstate.ds.length = ptr;
    return result;
  }
 
  public function start(sym:int):void
  {
    ptr = sym;
  }

  private function ST8int(ptr:int, v:int):void
  {
    gstate.gworker.mstate._mw8(ptr, v);
  }

  private function ST16int(ptr:int, v:int):void
  {
    gstate.gworker.mstate._mw16(ptr, v);
  }

  private function ST32int(ptr:int, v:int):void
  {
    gstate.gworker.mstate._mw32(ptr, v);
  }

  public function set i8(v:uint):void
  {
    ST8int(ptr, v);
    ptr += 1;
  }
 
  public function set i16(v:uint):void
  {
    ST16int(ptr, v);
    ptr += 2;
  }
 
  public function set i32(v:uint):void
  {
    ST32int(ptr, v);
    ptr += 4;
  }
 
  public function set ascii(v:String):void
  {
    var len:int = v.length;
 
    for(var i:int = 0; i < len; i++)
      this.i8 = v.charCodeAt(i);
  }
 
  public function set asciz(v:String):void
  {
    this.ascii = v;
    this.i8 = 0;
  }
 
  public function set zero(n:int):void
  {
    while(n--)
      this.i8 = 0;
  }
}

class AlchemyLibInit
{
  public var rv:int;

  function AlchemyLibInit(_rv:int)
  {
    rv = _rv;
  }
};

class AlchemyExit
{
  public var rv:int;

  function AlchemyExit(_rv:int)
  {
    rv = _rv;
  }
};

class AlchemyYield
{
  public var ms:int;

  function AlchemyYield(_ms:int = 0)
  {
    ms = _ms;
  }
};

class AlchemyDispatch
{
};

class AlchemyBlock
{
};

class AlchemyBreakpoint
{
  public var bp:Object;

  function AlchemyBreakpoint(_bp:Object)
  {
    bp = _bp;
  }
};

class IO
{
  public function close():int { return -1; }

  public function read(buf:int, nbytes:int):int { return 0; }
  public function write(buf:int, nbytes:int):int { return 0; }
  public function set position(offs:int):void { }
  public function get position():int { return -1; }
  public function set size(n:int):void { }
  public function get size():int { return 0; }
}

class ByteArrayIO extends IO
{
  public var byteArray:ByteArray;

  public override function read(buf:int, nbytes:int):int
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    var n:int = Math.min(nbytes, byteArray.bytesAvailable);

    if(n)
      byteArray.readBytes(gstate.ds, buf, n);
    return n;
  }

  public override function write(buf:int, nbytes:int):int
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    if(nbytes)
      byteArray.writeBytes(gstate.ds, buf, nbytes);
    return nbytes;
  }

  public override function set position(offs:int):void
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    byteArray.position = offs;
  }

  public override function get position():int
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    return byteArray.position;
  }

  public override function set size(n:int):void
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    byteArray.length = n;
  }

  public override function get size():int
  {
    if(!byteArray)
      throw(new AlchemyBlock);

    return byteArray.length;
  }
}

Alchemy::Shell
class ShellIO extends IO
{
  private var m_buf:String = "";
  private var m_trace:Boolean;
  private var m_closed:Boolean;

  public function ShellIO(_trace:Boolean = false)
  {
    m_trace = _trace;
  }

  public override function read(buf:int, nbytes:int):int
  {
    if(!m_buf)
    {
      var ns:Namespace = new Namespace("avmshell");
      var sys:Object = ns::["System"];

      m_buf = sys.readLine() + "\n";
      if(m_buf == "\x04\n")
        m_closed = true;
    }

    if(m_closed)
      return 0;

    var r:int = 0;

    while(m_buf && nbytes--)
    {
      r++;
      gstate._mw8(buf++, m_buf.charCodeAt(0));
      m_buf = m_buf.substr(1);
    }
    return r;
  }

  public override function write(buf:int, nbytes:int):int
  {
    var c:int = nbytes;
    var s:String = "";
    while(c--)
    {
      s += String.fromCharCode(gstate._mru8(buf));
      buf++;
    }
    if(m_trace)
      trace(s);
    else
    {
      var ns:Namespace = new Namespace("avmshell");
      var sys:Object = ns::["System"];

      sys.write(s);
    }
    return nbytes;
  }
}

Alchemy::NoShell {

class TextFieldI extends IO
{
  private var m_tf:TextField;
  private var m_start:int = -1;
  private var m_buf:String = "";
  private var m_closed:Boolean = false;

  public function TextFieldI(tf:TextField)
  {
    m_tf = tf;
    m_tf.addEventListener(KeyboardEvent.KEY_DOWN, function(event:KeyboardEvent)
    {
     
      if(String.fromCharCode(event.charCode).toLowerCase() == "d"
          && event.ctrlKey)
        m_closed = true;
     
      if(String.fromCharCode(event.charCode).toLowerCase() == "t"
          && event.ctrlKey)
        setTimeout(function():void {
          m_start = -1;
          m_tf.text = "";
        },1);
    });
   
   
   
   
   
    m_tf.addEventListener(TextEvent.TEXT_INPUT, function(event:TextEvent)
    {
      var len:int = m_tf.length;
      var selStart:int = m_tf.selectionBeginIndex;
      if(m_start < 0 || m_start > selStart)
        m_start = selStart;
      event.preventDefault();
      m_tf.replaceSelectedText(event.text);
      var selEnd:int = m_tf.selectionEndIndex;
      var fmt:TextFormat = m_tf.getTextFormat(selStart, selEnd);
      fmt.bold = false;
      m_tf.setTextFormat(fmt, selStart, selEnd);
      if(event.text.indexOf("\n") >= 0)
      {
        var ptext:String = m_tf.text;
        var nonBold:String = "";
        var len:int = m_tf.length;
        for(var i:int = m_start; i < len; i++)
        {
          var fmt:TextFormat = m_tf.getTextFormat(i, i+1);
          var bold:Boolean = fmt.bold;

          if(bold != null && !bold.valueOf())
            nonBold += ptext.charAt(i);
        }
        nonBold = nonBold.replace(/\r/g, "\n");
        var nl:int = nonBold.lastIndexOf("\n");
        var sel:int = len - (nonBold.length - nl - 1);
        m_tf.setSelection(sel, sel);
        nonBold = nonBold.substr(0, nl + 1);
        if(!m_closed)
          m_buf += nonBold;
        m_start = sel;
      }
    });
  }

  public override function read(buf:int, nbytes:int):int
  {
    if(!m_buf)
    {
      if(m_closed)
        return 0;
     
      throw new AlchemyBlock;
    }

    var r:int = 0;

    while(m_buf && nbytes--)
    {
      r++;
      gstate._mw8(buf++, m_buf.charCodeAt(0));
      m_buf = m_buf.substr(1);
    }
    return r;
  }

}

class TextFieldO extends IO
{
  private var m_tf:TextField;
  private var m_trace:Boolean;

  public function TextFieldO(tf:TextField, shouldTrace:Boolean = false)
  {
    m_tf = tf;
    m_trace = shouldTrace;
  }

  public override function write(buf:int, nbytes:int):int
  {
    var c:int = nbytes;
    var s:String = "";
    while(c--)
    {
      s += String.fromCharCode(gstate._mru8(buf));
      buf++;
    }
    if(m_trace)
      trace(s);
    var start:int = m_tf.length;
    m_tf.replaceText(start, start, s);
    var end:int = m_tf.length;
    var fmt:TextFormat = m_tf.getTextFormat(start, end);
    fmt.bold = true;
    m_tf.setTextFormat(fmt, start, end);
    m_tf.setSelection(end, end);
    return nbytes;
  }
}

}

public interface CSystem
{
 
  function setup(f:Function):void;

 
  function getargv():Array;
  function getenv():Object;
  function exit(val:int):void;

 
  function fsize(fd:int):int;
  function psize(p:int):int;
  function access(path:int, mode:int):int;
  function open(path:int, flags:int, mode:int):int;
  function ioctl(fd:int, req:int, va:int):int;
  function close(fd:int):int;
  function write(fd:int, buf:int, nbytes:int):int;
  function read(fd:int, buf:int, nbytes:int):int;
  function lseek(fd:int, offset:int, whence:int):int;
  function tell(fd:int):int;
} 


function shellExit(res:int):void
{
  Alchemy::NoShell {

  var ns:Namespace = new Namespace("flash.desktop");
  var nativeApp:Object;

  try
  {
    var nativeAppClass:Object = ns::["NativeApplication"];

    nativeApp = nativeAppClass.nativeApplication;
  }
  catch(e:*)
  {
    log(3, "No nativeApplication: " + e);
  }
  if(nativeApp)
  {
    nativeApp.exit(res);
    return;
  }

  }

  Alchemy::Shell {

  var ns:Namespace = new Namespace("avmshell");
  var sys:Object = ns::["System"];

  sys.exit(res);

  return;

  }

  throw new AlchemyExit(res);
}



var gfiles:Object = {};



Alchemy::NoShell
public class CSystemBridge implements CSystem
{
  private var sock:Socket;

  public function CSystemBridge(host:String, port:int)
  {
    sock = new Socket();
    sock.endian = "littleEndian";

    sock.addEventListener(flash.events.Event.CONNECT, sockConnect);
    sock.addEventListener(flash.events.ProgressEvent.SOCKET_DATA, sockData);
    sock.addEventListener(flash.events.IOErrorEvent.IO_ERROR, sockError);

    sock.connect(host, port);
  }

  private function sockConnect(e:Event):void
  {
log(2, "bridge connected");
  }

  private function sockError(e:IOErrorEvent):void
  {
log(2, "bridge error");
  }

 
  private var curPackBuf:ByteArray = new LEByteArray();
  private var curPackId:int;
  private var curPackLen:int;

  private function sockData(e:ProgressEvent):void
  {
    while(sock.bytesAvailable)
    {
      if(!curPackLen)
      {
        if(sock.bytesAvailable >= 8)
        {
          curPackId = sock.readInt();
          curPackLen = sock.readInt();
log(3, "bridge packet id: " + curPackId + " len: " + curPackLen);
          curPackBuf.length = curPackLen;
          curPackBuf.position = 0;
        }
        else break;
      }
      else
      {
        var len:int = sock.bytesAvailable;
  
        if(len > curPackLen)
          len = curPackLen;
        curPackLen -= len;
        while(len--)
          curPackBuf.writeByte(sock.readByte());
        if(!curPackLen)
          handlePacket();
      }
    }
  }

 
  private var handlers:Object = {};

  private function handlePacket():void
  {
    curPackBuf.position = 0;
    handlers[curPackId](curPackBuf);
    if(curPackId)
      delete handlers[curPackId];
  }

  private var sentPackId:int = 1;

  private function sendRequest(buf:ByteArray, handler:Function):void
  {
    if(handler)
      handlers[sentPackId] = handler;
    sock.writeInt(sentPackId);
    sock.writeInt(buf.length);
    sock.writeBytes(buf, 0);
    sock.flush();
    sentPackId++;
  }

 
  private var requests:Object = {};

  private function asyncReq(create:Function, handle:Function):*
  {
   
    var rid:String = String(gstate.esp);
    var req:Object = requests[rid];

    if(req)
    {
      if(req.pending)
        throw(new AlchemyBlock());
      else
      {
        delete requests[rid];
        return req.result;
      }
    }
    else
    {
      req = { pending: true };
      requests[rid] = req;

      var pack:ByteArray = new LEByteArray();

      create(pack);
      sendRequest(pack, function(buf:ByteArray):void {
        req.result = handle(buf);
        req.pending = false;
      });
      if(req.pending)
        throw(new AlchemyBlock());
    }
  }

 
  static const FSIZE:int = 1;
  static const PSIZE:int = 2;
  static const ACCESS:int = 3;
  static const OPEN:int = 4;
  static const CLOSE:int = 5;
  static const WRITE:int = 6;
  static const READ:int = 7;
  static const LSEEK:int = 8;
  static const TELL:int = 9;
  static const EXIT:int = 10;
  static const SETUP:int = 11;

  var argv:Array;
  var env:Object;

  public function setup(f:Function):void
  {
    var pack:ByteArray = new LEByteArray();

   
    pack.writeInt(SETUP);
    sendRequest(pack, function(buf:ByteArray):void {
     
     
      var argc:int = buf.readInt();

      argv = [];
      while(argc--)
        argv.push(buf.readUTF());

      var envc:int = buf.readInt();

      env = {};
      while(envc--)
      {
        var res:Array = (/([^\=]*)\=(.*)/).exec(buf.readUTF());

        if(res && res.length == 3)
          env[res[1]] = res[2];
      }
      f();
    });
  }

  public function getargv():Array
  {
    return argv;
  }

  public function getenv():Object
  {
    return env;
  }

  public function exit(val:int):void
  {
    var req:ByteArray = new LEByteArray();

    req.writeInt(EXIT);
    req.writeInt(val);
    sendRequest(req, null);
    shellExit(val);
  }

  public function fsize(fd:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(FSIZE);
        req.writeInt(fd);
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function psize(p:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(PSIZE);
        req.writeUTFBytes(gstate.gworker.stringFromPtr(p));
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function access(path:int, mode:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(ACCESS);
        req.writeInt(mode);
        req.writeUTFBytes(gstate.gworker.stringFromPtr(path));
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function ioctl(fd:int, req:int, va:int):int
  {
    return -1;
  }

  public function open(path:int, flags:int, mode:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(OPEN);
        req.writeInt(flags);
        req.writeInt(mode);
        req.writeUTFBytes(gstate.gworker.stringFromPtr(path));
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function close(fd:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(CLOSE);
        req.writeInt(fd);
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function write(fd:int, buf:int, nbytes:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(WRITE);
        req.writeInt(fd);
        if(nbytes > 4096)
          nbytes = 4096;
        req.writeBytes(gstate.ds, buf, nbytes);
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function read(fd:int, buf:int, nbytes:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(READ);
        req.writeInt(fd);
        req.writeInt(nbytes);
      },
      function(resp:ByteArray):int {
        var result:int = resp.readInt();
        var s:String = "";

        gstate.ds.position = buf; 
        while(resp.bytesAvailable)
        {
          var ch:int = resp.readByte();

          s += String.fromCharCode(ch);
          gstate.ds.writeByte(ch);
        }
log(4, "read from: " + fd + " : [" + s + "]");
        return result;
      }
    )
  }

  public function lseek(fd:int, offset:int, whence:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(LSEEK);
        req.writeInt(fd);
        req.writeInt(offset);
        req.writeInt(whence);
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

  public function tell(fd:int):int
  {
    return asyncReq(
      function(req:ByteArray):void {
        req.writeInt(TELL);
        req.writeInt(fd);
      },
      function(resp:ByteArray):int {
        return resp.readInt();
      }
    );
  }

}



public class CSystemLocal implements CSystem
{
 
  private const fds:Array = [];
  private const statCache:Object = {};
  private var forceSync:Boolean;

  public function CSystemLocal(_forceSync:Boolean = false)
  {
    forceSync = _forceSync;

    Alchemy::Shell {

    fds[0] = new ShellIO();
    fds[1] = new ShellIO();
    fds[2] = new ShellIO(true);

    }

    Alchemy::NoShell {

    gtextField = new TextField();
    gtextField.width = gsprite ? gsprite.stage.stageWidth : 800;
    gtextField.height = gsprite ? gsprite.stage.stageHeight : 600;
    gtextField.multiline = true;
    gtextField.defaultTextFormat = new TextFormat("Courier New");
    gtextField.type = TextFieldType.INPUT;
    gtextField.doubleClickEnabled = true;

    fds[0] = new TextFieldI(gtextField);
    fds[1] = new TextFieldO(gtextField, gsprite == null);
    fds[2] = new TextFieldO(gtextField, true);

    if(gsprite && gtextField)
      gsprite.addChild(gtextField);
    else
      log(3, "local system w/o gsprite");
    }
  }

  public function setup(f:Function):void
  {
    f();
  }

  public function getargv():Array
  {
    return gargs;
  }

  public function getenv():Object
  {
    return genv;
  }

  public function exit(val:int):void
  {
    log(3, "exit: " + val);
    shellExit(val);
  }

 
  private function fetch(path:String):Object
  {
    var res:Object = statCache[path];

    if(!res)
    {
      var gf:ByteArray = gfiles[path];

      if(gf)
      {
        res = { pending:false, size:gf.length, data:gf };
        statCache[path] = res;

        return res;
      }
    }

    Alchemy::Shell {

      var ns:Namespace = new Namespace("avmshell");
      var file:Object = ns::["File"];

      if(!file.exists(path))
      {
        log(3, "Doesn't exist: " + path);
        return { size: -1, pending: false };
      }
      
      var ns1:Namespace = new Namespace("avmplus");
      var bac:Object = ns1::["ByteArray"];
      var bytes:ByteArray = new ByteArray;
      bytes.writeBytes(bac.readFile(path));

      bytes.position = 0;

      return { size: bytes.length, data: bytes, pending: false };

    }

    if(forceSync)
      return res || { size: -1, pending: false };

    Alchemy::NoShell {

    if(!res)
    {
      var request:URLRequest = new URLRequest(path);
      var loader:URLLoader = new URLLoader();
  
      loader.dataFormat = URLLoaderDataFormat.BINARY;
      loader.addEventListener(Event.COMPLETE, function(e:Event):void
      {
        statCache[path].data = loader.data;
        statCache[path].size = loader.data.length;
        statCache[path].pending = false;
      });
      loader.addEventListener(IOErrorEvent.IO_ERROR, function(e:Event):void
      {
        statCache[path].size = -1;
        statCache[path].pending = false;
      });

      statCache[path] = res = { pending: true };

      loader.load(request);
    }

    }

    return res;
  }

  public function access(path:int, mode:int):int
  {
    var spath:String = gstate.gworker.stringFromPtr(path);

    if(mode & ~4/*R_OK*/)
    {
log(3, "failed access(" + spath + ") mode(" + mode + ")");
      return -1;
    }

    var stat:Object = fetch(spath);

    if(stat.pending)
      throw(new AlchemyBlock);

log(3, "access(" + spath + "): " + (stat.size >= 0));

    if(stat.size < 0)
      return -1;

    return 0;
  }

  public function ioctl(fd:int, req:int, va:int):int
  {
    return -1;
  }

  public function open(path:int, flags:int, mode:int):int
  {
    var spath:String = gstate.gworker.stringFromPtr(path);

    if(flags != 0)
    {
log(3, "failed open(" + spath + ") flags(" + flags + ")");
      return -1;
    }

    var stat:Object = fetch(spath);

    if(stat.pending)
      throw(new AlchemyBlock);

    if(stat.size < 0)
    {
log(3, "failed open(" + spath + ") doesn't exist");
      return -1;
    }

    var n:int = 0;

    while(fds[n])
      n++;

    var io:ByteArrayIO = new ByteArrayIO();

    io.byteArray = new ByteArray();
    io.byteArray.writeBytes(stat.data);
    io.byteArray.position = 0;

    fds[n] = io;

log(4, "open(" + spath + "): " + io.size);
    return n;
  }

  public function close(fd:int):int
  {
    var r:int = fds[fd].close();
    fds[fd] = null;
    return r;
  }

  public function write(fd:int, buf:int, nbytes:int):int
  {
    return fds[fd].write(buf, nbytes);
  }

  public function read(fd:int, buf:int, nbytes:int):int
  {
    return fds[fd].read(buf, nbytes);
  }

  public function lseek(fd:int, offset:int, whence:int):int
  {
    var io:IO = fds[fd];

    if(whence == 0)
      io.position = offset;
    else if(whence == 1)
      io.position += offset;
    else if(whence == 2)
      io.position = io.size + offset;
    return io.position;
  }

  public function tell(fd:int):int
  {
    return fds[fd].position;
  }

  public function fsize(fd:int):int
  {
    return fds[fd].size;
  }

  public function psize(p:int):int
  {
    var path:String = gstate.gworker.stringFromPtr(p);
    var stat:Object = fetch(path);

    if(stat.pending)
      throw(new AlchemyBlock);

if(stat.size < 0)
  log(3, "psize(" + path + ") failed");
else
  log(3, "psize(" + path + "): " + stat.size);
    return stat.size;
  }
}


public const gstaticInitter:StaticInitter = new StaticInitter();

public function __addc(a:uint, b:uint):uint
{
  var s:uint = a + b;
  gstate.cf = uint(s < a);
  return s;
}

public function __subc(a:uint, b:uint):uint
{
  var s:uint = a - b;
  gstate.cf = uint(s > a);
  return s;
}

public function __adde(a:uint, b:uint):uint
{
  var s:uint = a + b + gstate.cf;
  gstate.cf = uint(s < a);
  return s;
}

public function __sube(a:uint, b:uint):uint
{
  var s:uint = a - b - gstate.cf;
  gstate.cf = uint(s > a);
  return s;
}


public function memcpy
  (dst:int, src:int, size:int):int
{
  if(size)
  {
    gstate.ds.position = dst;
    gstate.ds.writeBytes(gstate.ds, src, size);
  }
  return dst;
}

public function memmove
  (dst:int, src:int, size:int):int
{
 
 
  if(src > dst || (src + size) < dst)
    memcpy(dst, src, size);
  else
  {
    var cur:int = dst + size;
    src += size;
    while(size--)
      gstate.ds[--cur] = gstate.ds[--src];
  }
  return dst;
}

public function memset
  (dst:int, v:int, size:int):int
{
  var w:int = v | (v << 8) | (v << 16) | (v << 24);

 
  gstate.ds.position = dst;
  while(size >= 4)
  {
    gstate.ds.writeUnsignedInt(w);
    size -= 4;
  }
  while(size--)
    gstate.ds.writeByte(v);
  return dst;
}

public function _brk(addr:int):int
{
  var newLen:int = addr;

  gstate.ds.length = newLen;
  return 0;
}

public function _sbrk(incr:int):int
{
  var prior:int = gstate.ds.length;
  var newLen:int = prior + incr;

  gstate.ds.length = newLen;
  return prior;
}

const inf:Number = Number.POSITIVE_INFINITY;
const nan:Number = Number.NaN;

public function isinf(a:Number):int
{
  return int(a === Number.POSITIVE_INFINITY ||
  a === Number.NEGATIVE_INFINITY);
}
 
public function isnan(a:Number):int
{
   return int(a === Number.NaN);
}

public class Machine extends MemUser
{
 
 
  public static const dbgFileNames:Array = [];

 
 
  public static const dbgFuncs:Array = [];

 
 
  public static const dbgFuncNames:Array = [];

 
 
 
  public static const dbgLabels:Array = [];

 
 
 
  public static const dbgLocs:Array = [];

 
 
  public static const dbgScopes:Array = [];

 
 
  public static const dbgGlobals:Array = [];

 
  public static const dbgBreakpoints:Object = {};
 
  public static var dbgFrameBreakLow:int = 0;
  public static var dbgFrameBreakHigh:int = -1;

  public var state:int = 0;
  public var caller:Machine = gstate ? gstate.gworker : null;
  public var mstate:MState = caller ? caller.mstate : null;

  public function work():void
    { throw new AlchemyYield; }

  Alchemy::SetjmpAbuse
  public var freezeCache:int;
  Alchemy::SetjmpAbuse
  public static const intRegCount:int = 0;
  Alchemy::SetjmpAbuse
  public static const NumberRegCount:int = 0;

 
  public function get dbgFuncId():int { return -1; }
  public function get dbgFuncName():String { return dbgFuncNames[dbgFuncId]; }

 
  public var dbgLabel:int = 0;
  public var dbgLineNo:int = 0;
  public var dbgFileId:int = 0;
  public function get dbgFileName():String { return dbgFileNames[dbgFileId]; }
  public function get dbgLoc():Object
    { return { fileId: dbgFileId, lineNo: dbgLineNo }; }

 
  public function debugTraceMem(start:int, end:int):void
  {
    trace("");
    trace("*****");
    while(start <= end)
    {
      trace("* " + start + " : " + mstate._mr32(start));
      start += 4;
    }
    trace("");
  }

 
 
  public static function debugTraverseScope(scope:Object, label:int, f:Function):void
  {
    if(scope && label >= scope.startLabelId && label < scope.endLabelId)
    {
      f(scope);

      var scopes:Array = scope.scopes;

      for(var n:int = 0; n < scopes.length; n++)
        debugTraverseScope(scopes[n], label, f);
    }
  }

  public function debugTraverseCurrentScope(f:Function):void
  {
    debugTraverseScope(dbgScopes[dbgFuncId], dbgLabel, f);
  }

 
  public function debugLoc(fileId:int, lineNo:int):void
  {
   
   
    if(dbgFileId == fileId && dbgLineNo == lineNo)
      return;

    dbgFileId = fileId;
    dbgLineNo = lineNo;

    var locStr:String = fileId + ":" + lineNo;
    var bp:Object = dbgBreakpoints[locStr];

    if(bp && bp.enabled)
    {
      if(bp.temp)
        delete dbgBreakpoints[locStr];
      debugBreak(bp);
    }
    else if(dbgFrameBreakHigh >= dbgFrameBreakLow)
    {
      var curDepth:int = dbgDepth;

      if(curDepth >= dbgFrameBreakLow && curDepth <= dbgFrameBreakHigh)
        debugBreak(null);
    }
  }

  public function debugBreak(bp:Object):void
  {
    throw new AlchemyBreakpoint(bp);
  }
 
  public function debugLabel(label:int):void
  {
    dbgLabel = label;
  }

 
  public function get dbgDepth():int
  {
    var cur:Machine = this;
    var result:int;

    while(cur)
    {
      result++;
      cur = cur.caller;
    }
    return result;
  }

  public function get dbgTrace():String
  {
    return this.dbgFuncName + "(" + (this as Object).constructor + ") - " + this.dbgFileName + " : " + this.dbgLineNo + "(" + this.state + ")";
  }

  public static var sMS:uint;

  public function getSecsSetMS():uint
  {
    var time:Number = (new Date()).time;

    Machine.sMS = time % 1000;
    return time / 1000;
  }

 
  public function stringToPtr(addr:int, max:int, str:String):int
  {
    var w:int = str.length;

    if(max >= 0 && max < w)
      w = max;
    for(var i:int = 0; i < w; i++)
      mstate._mw8(addr++, str.charCodeAt(i));
    return w;
  }

  public function stringFromPtr(addr:int):String
  {
    var result:String = "";

    while(true)
    {
      var c:int = mstate._mru8(addr++);

      if(!c)
        break;
      result += String.fromCharCode(c);
    }
    return result;
  }

  public function backtrace():void
  {
    var cur:Machine = this;

    trace("");
    trace("*** backtrace");
    var framePtr:int = mstate.ebp;
    while(cur)
    {
      trace(cur.dbgTrace);

      cur.debugTraverseCurrentScope(
          function(scope:Object):void {
        trace("{{{");
        var vars:Array = scope.vars;
        for(var n:int = 0; n < vars.length; n += 2)
        {
         
          var descAddr:int = vars[n + 0];
         
          var nameAddr:int = mstate._mr32(descAddr + 8);
          var name:String = stringFromPtr(nameAddr);

          var frameOffset:int = vars[n + 1];

          trace("--- " + name + " (" + (frameOffset + framePtr) + ")");
        }
      });
      framePtr = mstate._mr32(framePtr);
      cur = cur.caller;
    }
    trace("");
  }

}





Alchemy::NoShell {

public var gtextField:TextField;
public var gsprite:Sprite;

}

Alchemy::Shell {

public var gsprite:Object;

}

public var grunner:Object;


public var genv:Object = {
  LANG: "en_US.UTF-8",
  TERM: "ansi"
};
public var gargs:Array = [
  "a.out"
];

public const gstate:MState


= new MState(new Machine);
const mstate:MState = gstate;





public const gsetjmpMachine2ESPMap:Dictionary = new Dictionary(true);

Alchemy::SetjmpAbuse
{

public const gsetjmpAbuseMap:Object = {};

public const gsetjmpFrozenMachineSet:Dictionary = new Dictionary(true);


public var gsetjmpFreezeIRCs:Object = {};
public var gsetjmpAbuseAlloc:Function;
public var gsetjmpAbuseFree:Function;
}

public const i__setjmp = exportSym("__setjmp", regFunc(FSM__setjmp.start));
public const i_setjmp = exportSym("_setjmp", i__setjmp);


function findMachineForESP(esp:int):Machine
{
  for (var mach:Object in gsetjmpMachine2ESPMap)
  {
    if(gsetjmpMachine2ESPMap[mach] == esp)
      return Machine(mach);
  }
  return null;
}

public class FSM__setjmp extends Machine
{
  public static function start():void
  {
    gstate.gworker = new FSM__setjmp;
    throw new AlchemyDispatch;
  }

  public override function work():void
  {
    mstate.pop();

    var buf:int = _mr32(mstate.esp);

    _mw32(buf + 0, 667788);
    _mw32(buf + 4, caller.state);
    _mw32(buf + 8, mstate.esp);
    _mw32(buf + 12, mstate.ebp);
    _mw32(buf + 16, 887766);

log(4, "setjmp: " + buf);
    var mach:Machine = findMachineForESP(mstate.esp);

    if(mach)
      delete gsetjmpMachine2ESPMap[mach];

    gsetjmpMachine2ESPMap[caller] = mstate.esp;

    Alchemy::SetjmpAbuse
    {
      var abuse:* = gsetjmpAbuseMap[buf];

      if(abuse)
        abuse.setjmp(buf);
    }

    mstate.gworker = caller;
    mstate.eax = 0;
  }
}

public const i__longjmp = exportSym("__longjmp", regFunc(FSM__longjmp.start));
public const i_longjmp = exportSym("_longjmp", i__longjmp);

public class FSM__longjmp extends Machine
{
  public static function start():void
  {
    gstate.gworker = new FSM__longjmp;
    throw new AlchemyDispatch;
  }

  public override function work():void
  {
    mstate.pop();
    var buf:int = _mr32(mstate.esp);
    var ret:int = _mr32(mstate.esp + 4);

log(4, "longjmp: " + buf);

    var istate:int = _mr32(buf + 4);
    var nesp:int = _mr32(buf + 8);
    var nebp:int = _mr32(buf + 12);
log(3, "longjmp -- buf: " + buf + " state: " + istate + " esp: " + nesp +
  " ebp: " + nebp);
    if(!buf || !nesp || !nebp)
      throw("longjmp -- bad jmp_buf");

    var mach:Machine = findMachineForESP(nesp);

    Alchemy::SetjmpAbuse
    {
      var abuse:* = gsetjmpAbuseMap[buf];

      if(abuse)
        mach = abuse.longjmp(buf, mach);
    }

    if(!mach)
    {
      debugTraceMem(buf - 24, buf + 24);
/*      for(var k:String in gsetjmpESP2MachineMap)
        log(3, k + " -> " + gsetjmpESP2MachineMap[k].dbgFuncName);
*/
      throw("longjmp -- bad esp");
    }

   
    delete gsetjmpMachine2ESPMap[mach];

    mstate.gworker = mach;
    mach.state = istate;
    mstate.esp = nesp;
    mstate.ebp = nebp;
    mstate.eax = ret;

    throw new AlchemyDispatch;
  }

}

public interface Debuggee
{
  function suspend():void;
  function resume():void;
  function get isRunning():Boolean;

  function cancelDebug():void;
}

Alchemy::NoShell
public class GDBMIDebugger
{
Alchemy::NoDebugger {
  public function GDBMIDEbugger(dbge:Debuggee) {}
}
Alchemy::Debugger {
  var sock:Socket;
  var debuggee:Debuggee;

  public function GDBMIDebugger(dbge:Debuggee)
  {
    sock = new Socket();
    debuggee = dbge;

    sock.addEventListener(flash.events.Event.CONNECT, sockConnect);
    sock.addEventListener(flash.events.ProgressEvent.SOCKET_DATA, sockData);
    sock.addEventListener(flash.events.IOErrorEvent.IO_ERROR, sockError);
    sock.addEventListener(flash.events.SecurityErrorEvent.SECURITY_ERROR,
      sockError);

    debuggee.suspend();
    try
    {
      sock.connect("localhost", 5678);
    }
    catch(e:*)
    {
      sockError(e);
    }
  }

  private function sockConnect(e:Event):void
  {
    log(2, "debugger connect");
    prompt();
  }

  private function sockError(e:*):void
  {
    log(2, "debugger socket error: " + e.toString());
    if(!debuggee.isRunning)
      debuggee.resume();
    debuggee.cancelDebug();
  }

  private var buffer:String = "";

  private function sockData(e:ProgressEvent):void
  {
    while(sock.bytesAvailable)
    {
      var ch:int = sock.readUnsignedByte();

      if(ch == 3)
      {
        if(debuggee.isRunning)
        {
          debuggee.suspend();
          broken(null);
        }
      }
      else
        buffer += String.fromCharCode(ch);
    }

    var cmds:Array = buffer.split(/\n/);

    for(var i:int = 0; i < cmds.length - 1; i++)
      command(cmds[i]);
    buffer = cmds[cmds.length - 1];
  }

  private function toMI(obj:*, outer:Boolean = true):String
  {
    if(typeof obj == "object")
    {
      var arr:Array = obj as Array;
      var first:Boolean = true;

      if(arr)
      {
        var result:String = outer ? "" : "[";

        for(var i:int = 0; i < arr.length; i++)
        {
          if(first)
            first = false;
          else
            result += ",";
          result += toMI(arr[i], false);
        }
        return outer ? result : (result + "]");
      }
      else
      {
        var result:String = "";
        var nkeys:int = 0;
        var keys:Array = (obj && obj.__order) ? obj.__order : null;

        if(!keys)
        {
          keys = [];
          for(var key:String in obj)
            keys.push(key);
        }
        for(var ki:int = 0; ki < keys.length; ki++)
        {
          if(first)
            first = false;
          else
            result += ",";
          var key:String = keys[ki];
          result += key + "=" + toMI(obj[key], false);
          nkeys++;
        }
        return (outer || nkeys == 1) ? result : ("{" + result + "}");
      }
    }
    else
      return toCString(obj.toString());
  }

  private function toCString(str:String):String
  {
     
      return "\"" + str.replace("\\", "\\\\")
        .replace("\"", "\\\"").replace("\r", "\\r").replace("\n", "\\n")
        + "\"";
  }

  private function respond(str:String, resp:String = "",
      rest:Object = null):void
  {
    var whole:String = str;

    if(resp)
    {
      whole += resp;
      if(rest)
        whole += "," + toMI(rest);
    }
    sock.writeUTFBytes(whole + "\n");
    sock.flush();
    log(2, "DBG> " + whole);
  }

  private var promptStr:String = "(gdb) ";

  private function prompt():void
  {
    respond(promptStr);
  }

  private function console(str:String):void
  {
    respond("~" + toCString(str));
  }

  private function done(id:String, rest:Object = null):void
  {
    respond(id, "^done", rest);
  }

  private function running(id:String, rest:Object = null):void
  {
    respond(id, "^running", rest);
  }

  private function stopped(id:String, rest:Object = null):void
  {
    respond(id, "*stopped", rest);
  }

  private function error(id:String, msg:String):void
  {
    respond(id, "^error", { msg: msg } );
  }

 
  private function findSymNum(sym:String):int
  {
    var symNames:Array = Machine.dbgFuncNames;

    for(var i:int = 1; i < symNames.length; i++)
      if(sym == symNames[i])
        return i;
    return 0;
  }

 
  private function findSymLoc(sym:String):Object
  {
    var num:int = findSymNum(sym);

    if(num)
    {
      var locs:Array = Machine.dbgLocs;

      for(var i:int = 0; i < locs.length; i += 4)
        if(locs[i + 2] == num)
          return { fileId: locs[i + 0], lineNo: locs[i + 1] };
    }
    return null;
  }

/* DEAD?
 
  private function findNextLoc(loc:Object):Object
  {
    var locs:Array = Machine.dbgLocs;

    for(var i:int = 0; i < locs.length - 4; i += 4)
      if(locs[i + 0] == loc.fileId && locs[i + 1] == loc.lineNo &&
          locs[i + 2] == locs[i + 6])
        return { fileId: locs[i + 4], lineNo: locs[i + 5] };
    return null;
  }
*/

  private var breakpointNum:int = 1;

  private function breakInsert(cmdId:String, paramStr:String):void
  {
    var params:Array = paramStr.split(/\s+/);
    var temp:Boolean = false;
    var sym:String = "";

    for(var i:int = 0; i < params.length; i++)
    {
     
      if(params[i] == "-t")
        temp = true;
      else
        sym = params[i];
    }

   
    var loc:Object = findSymLoc(sym);

    if(loc)
    {
      var bp:Object = { temp: temp, number: breakpointNum++, enabled: true };
      var locStr = loc.fileId + ":" + loc.lineNo;

      log(2, "debug break insert: " + locStr);
      Machine.dbgBreakpoints[locStr] = bp;
      done(cmdId, { bkpt:
        { number: bp.number, enabled: bp.enabled ? "y" : "n" }});
    }
    else
      error(cmdId, "Can't find: " + sym);
  }

  private function info(id:String, paramString:String):void
  {
    if(paramString == "sharedlibrary")
    {
      console("From        To          Syms Read    Shared Object Library\n");
      done(id);
    }
    else if(paramString == "threads")
    {
      console("* 1 thread 1\n");
      done(id);
    }
    else
      error(id, "No info for: " + paramString);
  }

 
  static const maxStack:int = 10000;

 
  private var selectedFrameNum:int = 0;

 
  private function getFrame(n:int):int
  {
    var ptr:int = gstate.ebp;

    for(var i:int = 0; i < n; i++)
      ptr = gstate.gworker._mr32(ptr);
    return ptr;
  }

 
 
  private function getFrameR(n:int):int
  {
    return getFrame(gstate.gworker.dbgDepth - n);
  }

 
  private function get selectedFrame():int
  {
    return getFrame(selectedFrameNum);
  }

 
  private function getMachine(n:int):Machine
  {
    var curMach:Machine = gstate.gworker;

    for(var i:int = 0; i < n; i++)
      curMach = curMach.caller;
    return curMach;
  }

 
 
  private function getMachineR(n:int):Machine
  {
    return getMachine(gstate.gworker.dbgDepth - n);
  }

 
  private function get selectedMachine():Machine
  {
    return getMachine(selectedFrameNum);
  }

  public function stackListLocals(id:String, paramString:String):void
  {
    var params:Array = paramString.split(/\s+/);
   
    var showValues:int = (params && params.length > 0) ? params[0] : 0;
    var curMach:Machine = selectedMachine;

    var locals:Array = [];

    curMach.debugTraverseCurrentScope(function(scope:Object):void {
        var vars:Array = scope.vars;
        for(var n:int = 0; n < vars.length; n += 2)
        {
         
          var descAddr:int = vars[n + 0];
         
          var nameAddr:int = curMach._mr32(descAddr + 8);
          var name:String = curMach.stringFromPtr(nameAddr);

          locals.push({ name: name });
        }
    });
    done(id, { locals: locals });
  }

  public function stackListArguments(id:String, paramString:String):void
  {
    var params:Array = paramString.split(/\s+/);
   
    var showValues:int = (params && params.length > 0) ? params[0] : 0;
    var lowFrame:int = (params && params.length > 1) ? params[1] : 0;
    var highFrame:int = (params && params.length > 2) ? params[2] : maxStack;
    var curFrame:int = lowFrame;
    var curMach:Machine = getMachine(lowFrame);

    var frames:Array = [];

    while(curMach && curFrame <= highFrame)
    {
     
      var frame:Object = {
        level: curFrame,
        args: []
      };
      frames.push({ frame: frame });
      curFrame++;
      curMach = curMach.caller;
    } 
    done(id, { "stack-args": frames });
  }

  public function stackListFrames(id:String, paramString:String):void
  {
    var params:Array = paramString.split(/\s+/);
    var lowFrame:int = (params && params.length > 0) ? params[0] : 0;
    var highFrame:int = (params && params.length > 1) ? params[1] : maxStack;
    var curFrame:int = lowFrame;
    var curMach:Machine = getMachine(lowFrame);

    var frames:Array = [];

    while(curMach && curFrame <= highFrame)
    {
     
      var frame:Object = {
        level: curFrame,
        addr: "0xffffffff",
        func: curMach.dbgFuncName,
        file: curMach.dbgFileName,
        line: curMach.dbgLineNo
      };
      frames.push({ frame: frame });
      curFrame++;
      curMach = curMach.caller;
    } 
    done(id, { stack: frames });
  }

  private function execStep(id:String, how:String):void
  {
    if(debuggee.isRunning)
      error(id, "Already running");
    else
    {
      var depth:int = gstate.gworker.dbgDepth;

      switch(how)
      {
      default:
      case "in":
        Machine.dbgFrameBreakLow = 0;
        Machine.dbgFrameBreakHigh = maxStack;
        break;
      case "over":
        Machine.dbgFrameBreakLow = 0;
        Machine.dbgFrameBreakHigh = depth;
        break;
      case "out":
        Machine.dbgFrameBreakLow = 0;
        Machine.dbgFrameBreakHigh = depth - 1;
        break;
      }
        
      debuggee.resume();
      running(id);
    }
  }

  private var vars:Object = {};

  private function varCreate(id:String, paramString:String):void
  {
   
    var params:Array = paramString.split(/\s+/);

   
    if(!params || params.length != 3 || params[0] != "-" || params[1] != "*")
      error(id, "Invalid var-create params: " + paramString);
    else
    {
      var vname:String = params[2];
      var curMach:Machine = selectedMachine;
     
      var rframe:int = curMach.dbgDepth
      var varDesc:Object;

      if(vname.charAt(0) == "$")
        varDesc = { name: vname }
      else
        curMach.debugTraverseCurrentScope(function(scope:Object):void{
          var vars:Array = scope.vars;
          for(var n:int = 0; n < vars.length; n += 2)
          {
           
            var descAddr:int = vars[n + 0];
           
            var nameAddr:int = curMach._mr32(descAddr + 8);
            var name:String = curMach.stringFromPtr(nameAddr);
  
            if(name == vname)
            {
              varDesc =
                { name: name, frameOffset: vars[n + 1], rframe: rframe };
            }
          }
      });

      if(varDesc)
      {
        var i:int = 0;
        var name:String;

        while(vars[(name = ("var" + i))])
          i++;

        vars[name] = varDesc;
       
        done(id, { name: name, numchild: 0, type: "int" });
      }
      else
        error(id, "var-create can't find: " + params[2]);
    }
  }

  private function varDelete(id:String, paramString:String):void
  {
    if(vars[paramString])
    {
      delete vars[paramString];
      done(id);
    }
    else
      error(id, "var-delete not tracking: " + paramString);
  }

  private function varUpdate(id:String, paramString:String):void
  {
    if(vars[paramString])
    {
      var varDesc:Object = vars[paramString];

     
     
      done(id, { changelist: [ {
        name: paramString,
        in_scope: true,
        type_changed: false,
        __order: ["name", "in_scope", "type_changed"]
      } ] });
    }
    else
      error(id, "var-update not tracking: " + paramString);
  }

  private function varEvaluateExpression(id:String, paramString:String):void
  {
    if(vars[paramString])
    {
      var varDesc:Object = vars[paramString];
      var value:String;

      if(varDesc.name.charAt(0) == "$")
      {
        var reg:String = varDesc.name.substr(1);
        var curMach:Machine = selectedMachine

        value = gstate.hasOwnProperty(reg) ? gstate[reg] :
          curMach.hasOwnProperty(reg) ? curMach[reg] : "-1";
      }
      else
        value = String(
          gstate.gworker._mr32(getFrameR(varDesc.rframe) + varDesc.frameOffset))
     
      done(id, {
        value: value
      });
    }
    else
      error(id, "var-evaluate-expression not tracking: " + paramString);
  }

  static const regNames:Array = [
    "state",
    "eax", "edx", "ebp", "esp", "st0", "cf",
    "i0", "i1", "i2", "i3", "i4", "i5", "i6", "i7",
    "i8", "i9", "i10", "i11", "i12", "i13", "i14", "i15",
    "i16", "i17", "i18", "i19", "i20", "i21", "i22", "i23",
    "i24", "i25", "i26", "i27", "i28", "i29", "i30", "i31"/*,
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",
    "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
*/
  ];

  public function command(cmd:String):void
  {
    log(2, "DBG< " + cmd);
    var parse1:Array = /^(\d*)[- ](\S+)\s*(.*)/.exec(cmd);

    if(!parse1)
      error("", "Couldn't parse command");
    else
    {
      var cmdId:String = parse1[1];
      var cmdName:String = parse1[2];
      var paramString:String = parse1[3];

      switch(cmdName)
      {
     
      case "environment-cd":
      case "environment-directory":
      case "gdb-set":
       
        done(cmdId);
        break;
      case "gdb-exit":
        done(cmdId);
        sock.close();
        return;
      case "gdb-show":
        if(paramString == "prompt")
          done(cmdId, { value: promptStr });
        else
          error(cmdId, "Can't show: " + paramString);
        break;
      case "data-list-register-names":
        done(cmdId, { "register-names": regNames });
        break;
      case "data-list-changed-registers":
        done(cmdId, { "changed-registers": regNames.map(
          function(i:*, n:int, a:Array):int { return n; }
        )});
        break;
      case "info":
        info(cmdId, paramString);
        break;
      case "stack-select-frame":
       
        selectedFrameNum = int(paramString);
        done(cmdId);
        break;
      case "stack-info-depth":
        done(cmdId, { depth: gstate.gworker.dbgDepth });
        break;
      case "stack-list-frames":
        stackListFrames(cmdId, paramString);
        break;
      case "stack-list-arguments":
        stackListArguments(cmdId, paramString);
        break;
      case "stack-list-locals":
        stackListLocals(cmdId, paramString);
        break;
      case "exec-continue":
      case "exec-run":
        if(debuggee.isRunning)
          error(cmdId, "Already running");
        else
        {
          debuggee.resume();
          running(cmdId);
        }
        break;
      case "exec-next":
        execStep(cmdId, "over");
        break;
      case "exec-step":
        execStep(cmdId, "in");
        break;
      case "exec-finish":
        execStep(cmdId, "out");
        break;
      case "break-insert":
        breakInsert(cmdId, paramString);
        break;
      case "var-create":
        varCreate(cmdId, paramString);
        break;
      case "var-delete":
        varDelete(cmdId, paramString);
        break;
      case "var-update":
        varUpdate(cmdId, paramString);
        break;
      case "var-evaluate-expression":
        varEvaluateExpression(cmdId, paramString);
        break;
      default:
        error(cmdId, "Undefined MI command: " + cmdName);
        break;
      }
    }
    prompt();
  }

  public function signal(sig:Object):void
  {
   
    broken(null);
  }

  public function broken(bp:Object):void
  {
    log(2, "debugger broken");
    selectedFrameNum = 0;
    Machine.dbgFrameBreakLow = 0;
    Machine.dbgFrameBreakHigh = -1;
    stopped("");
    prompt();
  }
}
}


public class CRunner implements Debuggee
{
  Alchemy::NoShell
  var debugger:GDBMIDebugger;

  Alchemy::Shell
  var debugger:Object;

  Alchemy::NoShell
  var timer:Timer;

  var suspended:int = 0;
  var forceSyncSystem:Boolean;

  public function CRunner(_forceSyncSystem:Boolean = false)
  {
    if(grunner)
      log(1, "More than one CRunner!");
    grunner = this;
    forceSyncSystem = _forceSyncSystem;
  }

  public function cancelDebug():void
  {
    debugger = null;
  }

  public function get isRunning():Boolean
  {
    return suspended <= 0;
  }

  public function suspend():void
  {
    suspended++;

    Alchemy::NoShell {

    if(timer && timer.running)
      timer.stop();

    }
  }

  public function resume():void
  {
    if(!--suspended)
      startWork();
  }

  private function startWork():void
  {
    Alchemy::NoShell {

    if(!timer.running)
    {
      timer.delay = 1;
      timer.start();
    }

    }
  }

  Alchemy::Debugger
  private function startDebugger():void
  {
    Alchemy::NoShell {

    debugger = new GDBMIDebugger(this);

    }

    Alchemy::Shell {

      throw("No debug support in shell...");

    }
  }

  public function work():void
  {
    if(!isRunning)
      return;

    try
    {
      var startTime:Number = (new Date).time;

      while(true)
      {
        var checkInterval:int = 1000;
      

        while(checkInterval > 0)
        {
          try
          {
            while(checkInterval-- > 0)
              gstate.gworker.work();
          } catch(e:AlchemyDispatch) {}
        }
        if(((new Date).time - startTime) >= 1000 * 10)
          throw(new AlchemyYield);
      }
    }
    catch(e:AlchemyExit)
    {
      Alchemy::NoShell {

      timer.stop();

      }

      gstate.system.exit(e.rv);
    }
    catch(e:AlchemyYield)
    {
      Alchemy::NoShell {

      var ms:int = e.ms;

      timer.delay = (ms > 0 ? ms : 1);

      }
    }
    catch(e:AlchemyBlock)
    {
      Alchemy::NoShell {

     
      timer.delay = 10;

      }
    }
    catch(e:AlchemyBreakpoint)
    {
      Alchemy::Debugger
      {
        if(debugger)
        {
          suspend();
          debugger.broken(e.bp);
        }
        else
          throw(e);
      }
      Alchemy::NoDebugger
      {
        throw(e);
      }
    }
/*
    catch(e:AlchemyLibInit)
      { throw(e); }
    catch(e:*)
    {
      log(1, e);
      if(debugger && gstate && gstate.gworker)
      {
        suspend();
        debugger.signal(e);
      }
      else
      {
        if(gstate && gstate.gworker)
        {
          try {
            gstate.gworker.backtrace();
          } catch(e:*) {}
        }
        gstate.system.exit(-1);
        throw(e);
      }
    }
*/
  }

  public function rawAllocString(str:String):int
  {
    var result:int = gstate.ds.length;

    gstate.ds.length += str.length + 1;
    gstate.ds.position = result;
    for(var i:int = 0; i < str.length; i++)
      gstate.ds.writeByte(str.charCodeAt(i));
    gstate.ds.writeByte(0);
    return result;
  }

  public function rawAllocIntArray(arr:Array):int
  {
    var result:int = gstate.ds.length;

    gstate.ds.length += (arr.length + 1) * 4;
    gstate.ds.position = result;
    for(var i:int = 0; i < arr.length; i++)
      gstate.ds.writeInt(arr[i]);
    return result;
  }

  public function rawAllocStringArray(arr:Array):Array
  {
    var ptrs:Array = [];

    for(var i:int = 0; i < arr.length; i++)
      ptrs.push(rawAllocString(arr[i]));
    return ptrs;
  }

  public function createEnv(obj:Object):Array
  {
    var kvps:Array = [];

    for(var key:String in obj)
      kvps.push(key + "=" + obj[key]);

    return rawAllocStringArray(kvps).concat(0);
  }

  public function createArgv(arr:Array):Array
  {
    return rawAllocStringArray(arr).concat(0);
  }

  public function startSystem():void
  {
    Alchemy::NoShell {

    if(!forceSyncSystem)
    {
      var request:URLRequest = new URLRequest(".swfbridge");
      var loader:URLLoader = new URLLoader();
  
      loader.dataFormat = URLLoaderDataFormat.TEXT;
      loader.addEventListener(Event.COMPLETE, function(e:Event):void
      {
        var xml:XML = new XML(loader.data);
  
        if(xml && xml.name() == "bridge" && xml.host && xml.port)
          startSystemBridge(xml.host, xml.port);
        else
          startSystemLocal();
      });
      loader.addEventListener(IOErrorEvent.IO_ERROR, function(e:Event):void
      {
        startSystemLocal();
      });
      loader.load(request);
      return;
    }

    }

    startSystemLocal(true);
  }

 
  Alchemy::NoShell
  public function startSystemBridge(host:String, port:int):void
  {
log(3, "bridge: " + host + " port: " + port);
    gstate.system = new CSystemBridge(host, port);
    gstate.system.setup(startInit);
  }

 
 
  public function startSystemLocal(forceSync:Boolean = false):void
  {
log(3, "local system");
    gstate.system = new CSystemLocal(forceSync);
    gstate.system.setup(startInit);
  }

  public function startInit():void
  {
    log(2, "Static init...");
   
    modStaticInit();

    var args:Array = gstate.system.getargv();
    var env:Object = gstate.system.getenv();
    var argv:Array = createArgv(args);
    var envp:Array = createEnv(env);
    var startArgs:Array = [args.length].concat(argv, envp);
    var ap:int = rawAllocIntArray(startArgs);

   
    gstate.ds.length = (gstate.ds.length + 4095) & ~4095;

    gstate.push(ap);
    gstate.push(0);

    log(2, "Starting work...");

    Alchemy::NoShell {

    timer = new Timer(1);
    timer.addEventListener(flash.events.TimerEvent.TIMER, 
      function(event:TimerEvent):void { work() });

    }

    try
    {
      FSM__start.start();
    }
    catch(e:AlchemyExit)
    {
      gstate.system.exit(e.rv);
      return;
    }
    catch(e:AlchemyYield) {}
    catch(e:AlchemyDispatch) {}
    catch(e:AlchemyBlock) {}

    Alchemy::NoShell {
    Alchemy::Debugger {
    if(!forceSyncSystem)
    {
      startDebugger();
      return;
    }

    }
    }

    startWork();
  }
}



interface ICAllocator
{
  function alloc(size:int):int;
  function free(ptr:int):void;
}


class CHeapAllocator implements ICAllocator
{
  private var pmalloc:Function;
  private var pfree:Function;
  
  public function alloc(n:int):int
  {
    if(pmalloc == null)
      pmalloc = (new CProcTypemap(CTypemap.PtrType,
        [CTypemap.IntType])).fromC([_malloc]);
    var result:int = pmalloc(n);
    return result;
  }
  
  public function free(ptr:int):void
  {
    if(pfree == null)
      pfree = (new CProcTypemap(CTypemap.VoidType,
        [CTypemap.PtrType])).fromC([_free]);
    pfree(ptr);
  }
}




class CTypemap
{
  public static var BufferType:CBufferTypemap;
  public static var SizedStrType:CSizedStrUTF8Typemap;
  public static var AS3ValType:CAS3ValTypemap;

  public static var VoidType:CVoidTypemap;
  public static var PtrType:CPtrTypemap;
  public static var IntType:CIntTypemap;
  public static var DoubleType:CDoubleTypemap;
  public static var StrType:CStrUTF8Typemap;

  public static var IntRefType:CRefTypemap;
  public static var DoubleRefType:CRefTypemap;
  public static var StrRefType:CRefTypemap;
  
  public static function getTypeByName(name:String):CTypemap
  {
    return CTypemap[name];
  }

  public static function getTypesByNameArray(names:Array):Array
  {
    var result:Array = [];
    if(names)
      for each(var name:* in names)
        result.push(CTypemap.getTypeByName(name));
    return result;
  }

  public static function getTypesByNames(names:String):Array
  {
    return CTypemap.getTypesByNameArray(names.split(/\s*,\s*/));
  }

 
 
 
 
  public function get ptrLevel():int { return 0; }

 
 
  public function get typeSize():int { return 4; }
  
 
 
  public function getValueSize(v:*):int { return typeSize; }
  
 
  public function fromC(v:Array):* { return undefined; }
  
 
 
 
  public function createC(v:*, ptr:int = 0):Array { return null; }

 
  public function destroyC(v:Array):void { }
  
 
  public function fromReturnRegs(regs:Object):*
  {
    var a:Array = [regs.eax];
    var result:* = fromC(a);

    destroyC(a);
    return result;
  }

 
  public function toReturnRegs(regs:Object, v:*, ptr:int = 0):void
    { regs.eax = createC(v, ptr)[0]; }
  
 
  public function readValue(ptr:int):*
  {
   
    var a:Array = [];
    mstate.ds.position = ptr;
    for(var n:int = 0; n < typeSize; n++)
      a.push(mstate.ds.readInt());
    return fromC(a);
  }
  
 
  public function writeValue(ptr:int, v:*):void
  {
   
    var a:Array = createC(v);
    mstate.ds.position = ptr;
    for(var n:int = 0; n < a.length; n++)
      mstate.ds.writeInt(a[n]);
  }
}


class CVoidTypemap extends CTypemap
{
  public override function get typeSize():int { return 0; }

  public override function fromReturnRegs(regs:Object):* { return undefined; }
  public override function toReturnRegs(regs:Object, v:*, ptr:int = 0):void { }
}



class CAllocedValueTypemap extends CTypemap
{
  private var allocator:ICAllocator;
  
  public function CAllocedValueTypemap(_allocator:ICAllocator)
  {
    allocator = _allocator;
  }
  
  public override function fromC(v:Array):* { return readValue(v[0]); }

  public override function createC(v:*, ptr:int = 0):Array
  {
    if(!ptr)
      ptr = alloc(v);
    writeValue(ptr, v);
    return [ptr];
  }
  
  public override function destroyC(v:Array):void
  {
    free(v[0]);  
  }
  
  protected function alloc(v:*):int { return allocator.alloc(getValueSize(v)); }
  protected function free(ptr:int):void { return allocator.free(ptr); }
}


class CStrUTF8Typemap extends CAllocedValueTypemap
{
  public function CStrUTF8Typemap(allocator:ICAllocator = null)
  {
    if(!allocator)
      allocator =  new CHeapAllocator;
    super(allocator);  
  }
  
  public override function get ptrLevel():int { return 1; }

 
  protected function ByteArrayForString(s:String):ByteArray
  {
    var result:ByteArray = new ByteArray;

    result.writeUTFBytes(s);
    result.writeByte(0);
    result.position = 0;
    
    return result;
  }
  
  public override function getValueSize(v:*):int
  {
    return ByteArrayForString(String(v)).length;
  }
  
  public override function readValue(ptr:int):*
  {
    mstate.ds.position = ptr;

    var len:int = 0;
    
    while(mstate.ds.readByte() != 0)
      len++;
    mstate.ds.position = ptr;
    return mstate.ds.readUTFBytes(len);
  }
  
  public override function writeValue(ptr:int, v:*):void
  {
    ByteArrayForString(String(v)).readBytes(mstate.ds, ptr);
  }
}


class CIntTypemap extends CTypemap
{
  public override function fromC(v:Array):* { return int(v[0]); }
  public override function createC(v:*, ptr:int = 0):Array { return [int(v)]; }
}


class CPtrTypemap extends CTypemap
{
  public override function fromC(v:Array):* { return int(v[0]); }
  public override function createC(v:*, ptr:int = 0):Array { return [int(v)]; }
}



class CRefTypemap extends CTypemap
{
  private var subtype:CTypemap;

  public function CRefTypemap(_subtype:CTypemap)
  {
    subtype = _subtype;
  }

  public override function fromC(v:Array):*
  {
    var p:int = v[0];

    for(var n:int = 0; n < subtype.ptrLevel; n++)
    {
      mstate.ds.position = p;
      p = mstate.ds.readInt();
    }
    return subtype.readValue(p);
  }

  public override function createC(v:*, ptr:int = 0):Array { return null; }
}


class CSizedStrUTF8Typemap extends CTypemap
{
  public override function get typeSize():int { return 8; }
  
  public override function fromC(v:Array):*
  {
    mstate.ds.position = v[0];
    return mstate.ds.readUTFBytes(v[1]);
  }
}


class CDoubleTypemap extends CTypemap
{
  private var scratch:ByteArray;
  
  public function CDoubleTypemap()
  {
    scratch = new ByteArray;
    scratch.length = 8;
    scratch.endian = "littleEndian";  
  }
  
  public override function get typeSize():int { return 8; }
  
  public override function fromC(v:Array):*
  {
    scratch.position = 0;
    scratch.writeInt(v[0]);
    scratch.writeInt(v[1]);
    scratch.position = 0;
    return scratch.readDouble();
  }
  
  public override function createC(v:*, ptr:int = 0):Array
  {
    scratch.position = 0;
    scratch.writeDouble(v);
    scratch.position = 0;
    return [ scratch.readInt(), scratch.readInt() ];
  }
  
  public override function fromReturnRegs(regs:Object):* { return regs.st0; }
  public override function toReturnRegs(regs:Object, v:*, ptr:int = 0):void { regs.st0 = v; }
}

class RCValue
{
  public var value:*;
  public var id:int;
  public var rc:int = 1;

  public function RCValue(_value:*, _id:int) { value = _value; id = _id; }
}


class ValueTracker
{
 
  private var val2rcv:Dictionary = new Dictionary;
 
  private var id2key:Object = {};
  private var snum:int = 1;

  public function acquireId(id:int):int
  {
    if(id)
    {
      var key:Object = id2key[id];

      val2rcv[key].rc++;
    }
    return id;
  }

  public function acquire(val:*):int
  {
    if(typeof(val) == "undefined")
      return 0;

    var ov:Object = Object(val);

   
   
    if(ov instanceof QName)
      ov = "*VT*QName*/" + ov.toString();

    var v:* = val2rcv[ov];
    var id:int;

    if(typeof(v) == "undefined")
    {
      while(!snum || typeof(id2key[snum]) != "undefined")
        snum++;
      id = snum;
      val2rcv[ov] = new RCValue(val, id);
      id2key[id] = ov;
    }
    else
    {
      id = v.id;
      val2rcv[ov].rc++;
    }
    return id;
  }

  public function get(id:int):*
  {
    if(id)
    {
      var key:Object = id2key[id];
      var rcv:RCValue = val2rcv[key];

      return rcv.value;
    }
    return undefined;
  }

  public function release(id:int):*
  {
    if(id)
    {
      var key:Object = id2key[id];
      var rcv:RCValue = val2rcv[key];

      if(rcv)
      {
        if(!--rcv.rc)
        {
          delete id2key[id];
          delete val2rcv[key];
        }
        return rcv.value;
      }
      else
        log(1, "ValueTracker extra release!: " + id);
    }
    return undefined;
  }
}


class CAS3ValTypemap extends CTypemap
{
  private var values:ValueTracker = new ValueTracker;

  public function get valueTracker():ValueTracker
  {
    return values;
  }

  public override function fromC(v:Array):*
  {
    return values.get(v[0]);
  }
  
  public override function createC(v:*, ptr:int = 0):Array
  {
    return [values.acquire(v)];
  }
  
  public override function destroyC(v:Array):void
  {
    values.release(v[0]);
  }
}


class NotifyMachine extends Machine
{
  private var proc:Function;

  public function NotifyMachine(_proc:Function)
  {
    proc = _proc;
   
   
    mstate.push(0);
    mstate.push(mstate.ebp);
    mstate.ebp = mstate.esp;
  }
  
  public override function work():void
  {
    var noClean:Boolean;

    try
    {
      noClean = proc() ? true : false;
    }
    catch(e:*) { log(1, "NotifyMachine: " + e); }
    if(!noClean)
    {
      mstate.gworker = caller;
      mstate.ebp = mstate.pop();
      mstate.pop();
    }
  }
}

class CProcTypemap extends CTypemap
{
  private var retTypemap:CTypemap;
  private var argTypemaps:Array;
  private var varargs:Boolean;
  private var async:Boolean;
  
  public function CProcTypemap(_retTypemap:CTypemap, _argTypemaps:Array, _varargs:Boolean = false, _async:Boolean = false)
  {
    retTypemap = _retTypemap;
    argTypemaps = _argTypemaps;
    varargs = _varargs;
    async = _async;
  }
  
  private function push(arg:*):void
  {
    if(arg is Array)
      for(var i:int = arg.length - 1; i >= 0; i--)
        mstate.push(arg[i]);
    else
        mstate.push(arg);
  }
  
  public override function fromC(v:Array):*
  {
    return function(...args):*
    {
      var sp:int = mstate.esp;
      var cargs:Array = [];
      var n:int;
      var asyncHandler:Function;
      var oldWorker:Machine = mstate.gworker;
      
      function cleanup():void
      {
        for(n = cargs.length - 1; n >= 0; n--)
          argTypemaps[n].destroyC(cargs[n]);
  
        mstate.esp = sp;
        mstate.gworker = oldWorker;
      };
            
      if(async)
      {
       
        asyncHandler = args.shift();
       
        mstate.gworker = new NotifyMachine(function():Boolean
        {
          var result:* = retTypemap.fromReturnRegs(mstate);
          cleanup();
          try
          {
            asyncHandler(result);
          } catch(e:*) { log(1, "asyncHandler: " + e.toString()); }
          return true;
        });
      }

      for(n = args.length - 1; n >= 0; n--)
      {
        var arg:* = args[n];
        
        if(n >= argTypemaps.length)
          push(arg);
        else
        {
          var carg:Array = argTypemaps[n].createC(arg);
          
          cargs[n] = carg;
          push(carg);
        }
      }
      mstate.push(0);

      if(!asyncHandler)
      {
        try
        {
          try
          {
           
            mstate.funcs[int(v[0])]();
          }
          catch(e:AlchemyYield) {}
          catch(e:AlchemyDispatch) {}

         
          while(mstate.gworker !== oldWorker)
          {
            try
            {
              while(mstate.gworker !== oldWorker)
                mstate.gworker.work();
            }
            catch(e:AlchemyYield) {}
            catch(e:AlchemyDispatch) {}
          }
  
          return retTypemap.fromReturnRegs(mstate);        
        }
        finally
        {
          cleanup();
        }
      }
      else
      {
        try
        {
         
          mstate.funcs[int(v[0])]();
        }
        catch(e:AlchemyYield) {}
        catch(e:AlchemyDispatch) {}
        catch(e:AlchemyBlock) {}
        catch(e:*)
        {
          cleanup();
          throw(e);
        }
      }
    }
  }
  
  public override function createC(v:*, ptr:int = 0):Array
  {
    var id:int = regFunc(function():void
    {
      var args:Array = [];

      mstate.pop();
      
      var sp:int = mstate.esp;

     
      for(var n:int = 0; n < argTypemaps.length ; n++)
      {
        var tm:CTypemap = argTypemaps[n];
        var aa:Array = [];
        var ts:int = tm.typeSize;
      
       
        mstate.ds.position = sp;
       
        sp += ts;
       
        for(; ts; ts -= 4)
          aa.push(mstate.ds.readInt());
       
        args.push(tm.fromC(aa));
      }
     
     
      if(varargs)
        args.push(sp);

      try
      {
       
       
       
       
        retTypemap.toReturnRegs(mstate, v.apply(null, args));
      }
      catch(e:*)
      {
       
       
       
       
        mstate.eax = 0;
        mstate.edx = 0;
        mstate.st0 = 0;
        log(2, "v.apply: " + e.toString());
      }
    });
    return [id];
  }
  
  public override function destroyC(v:Array):void
  {
    unregFunc(int(v[0]));
  }
}


class CBuffer
{
  private static var ptr2Buffer:Object = {};

  public static function free(ptr:int):void
  {
    ptr2Buffer[ptr].free();
  }

  private var allocator:ICAllocator;
  private var ptrVal:int;
  private var sizeVal:int;
  private var valCache:*;

  public function get ptr():int { return ptrVal; }
  public function get size():int { return sizeVal; }

  public function get value():*
  {
    return ptrVal ? computeValue() : valCache;
  }

  public function set value(v:*):void
  {
    if(ptrVal)
      setValue(v);
    else
      valCache = v;
  }

  protected function computeValue():* { return undefined; }
  protected function setValue(v:*):void { }

  public function CBuffer(_size:int, _alloc:ICAllocator = null)
  {
    if(!_alloc)
      _alloc = new CHeapAllocator;
    allocator = _alloc;
    sizeVal = _size;
    alloc();
  }

  private function alloc():void
  {
    if(!ptrVal)
    {
      ptrVal = allocator.alloc(sizeVal);
      ptr2Buffer[ptrVal] = this;
    }
  }

  public function reset():void
  {
    if(!ptrVal)
    {
      alloc();
      setValue(valCache);
    }
  }

  public function free():void
  {
    if(ptrVal)
    {
      valCache = computeValue();
      allocator.free(ptrVal);
      delete ptr2Buffer[ptrVal];
      ptrVal = 0;
    }
  }
}


class CBufferTypemap extends CTypemap
{
  public override function createC(v:*, ptr:int = 0):Array
  {
    var buffer:CBuffer = v;

   
   
    buffer.reset();
    return [buffer.ptr];
  }

  public override function destroyC(v:Array):void
  {
    CBuffer.free(v[0]);
  }
}

class CStrUTF8Buffer extends CBuffer
{
  private var nullTerm:Boolean;

  protected override function computeValue():*
  {
    var len:int = 0;
    var max:int = this.size;

    mstate.ds.position = this.ptr;
    while(max-- && mstate.ds.readByte() != 0)
      len++;
    mstate.ds.position = this.ptr;
    return mstate.ds.readUTFBytes(len);
  }

  protected override function setValue(v:*):void
  {
    var ba:ByteArray = new ByteArray;
   
    var max:int = nullTerm ? this.size - 1 : this.size;

    ba.writeUTFBytes(v);
   
    if(ba.length > max)
      ba.length = max;
   
    if(ba.length < this.size)
      ba.writeByte(0);
   
    ba.position = 0;
    ba.readBytes(mstate.ds, this.ptr);
  }

  public function CStrUTF8Buffer(_size:int, _nullTerm:Boolean = true,
    alloc:ICAllocator = null)
  {
    super(_size, alloc);
    nullTerm = _nullTerm;
  }
}

CTypemap.BufferType = new CBufferTypemap;
CTypemap.SizedStrType = new CSizedStrUTF8Typemap;
CTypemap.AS3ValType = new CAS3ValTypemap;
CTypemap.VoidType = new CVoidTypemap;
CTypemap.PtrType = new CPtrTypemap;
CTypemap.IntType = new CIntTypemap;
CTypemap.DoubleType = new CDoubleTypemap;
CTypemap.StrType = new CStrUTF8Typemap;
CTypemap.IntRefType = new CRefTypemap(CTypemap.IntType);
CTypemap.DoubleRefType = new CRefTypemap(CTypemap.DoubleType);
CTypemap.StrRefType = new CRefTypemap(CTypemap.StrType);

const i_AS3_Acquire:int = exportSym("_AS3_Acquire",
  (new CProcTypemap(CTypemap.VoidType, [CTypemap.PtrType]))
  .createC(CTypemap.AS3ValType.valueTracker.acquireId)[0]
);

const i_AS3_Release:int = exportSym("_AS3_Release",
  (new CProcTypemap(CTypemap.VoidType, [CTypemap.PtrType]))
  .createC(CTypemap.AS3ValType.valueTracker.release)[0]
);

function AS3_NSGet(ns:*, prop:*):*
{
  var tns:String = typeof(ns);

  if(tns == "undefined" || !(ns instanceof Namespace))
  {
    if(tns == "string")
      ns = new Namespace(ns);
    else
      ns = new Namespace;
  }
  return ns::[prop];
}

const i_AS3_NSGet:int = exportSym("_AS3_NSGet",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_NSGet)[0]
);

const i_AS3_NSGetS:int = exportSym("_AS3_NSGetS",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.StrType]))
  .createC(AS3_NSGet)[0]
);

function AS3_TypeOf(v:*):String
{
  return typeof(v);
}

const i_AS3_TypeOf:int = exportSym("_AS3_TypeOf",
  (new CProcTypemap(CTypemap.StrType,
  [CTypemap.AS3ValType]))
  .createC(AS3_TypeOf)[0]
);

function AS3_NOP(v:*):*
{
  return v;
}

const i_AS3_String:int = exportSym("_AS3_String",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.StrType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_StringN:int = exportSym("_AS3_StringN",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.SizedStrType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_Int:int = exportSym("_AS3_Int",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.IntType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_Ptr:int = exportSym("_AS3_Ptr",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.PtrType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_Number:int = exportSym("_AS3_Number",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.DoubleType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_True:int = exportSym("_AS3_True",
  (new CProcTypemap(CTypemap.AS3ValType,
  []))
  .createC(function():Boolean { return true; })[0]
);

const i_AS3_False:int = exportSym("_AS3_False",
  (new CProcTypemap(CTypemap.AS3ValType,
  []))
  .createC(function():Boolean { return false; })[0]
);

const i_AS3_Null:int = exportSym("_AS3_Null",
  (new CProcTypemap(CTypemap.AS3ValType,
  []))
  .createC(function():* { return null; })[0]
);

const i_AS3_Undefined:int = exportSym("_AS3_Undefined",
  (new CProcTypemap(CTypemap.AS3ValType,
  []))
  .createC(function():* { return undefined; })[0]
);

const i_AS3_StringValue:int = exportSym("_AS3_StringValue",
  (new CProcTypemap(CTypemap.StrType,
  [CTypemap.AS3ValType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_IntValue:int = exportSym("_AS3_IntValue",
  (new CProcTypemap(CTypemap.IntType,
  [CTypemap.AS3ValType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_PtrValue:int = exportSym("_AS3_PtrValue",
  (new CProcTypemap(CTypemap.PtrType,
  [CTypemap.AS3ValType]))
  .createC(AS3_NOP)[0]
);

const i_AS3_NumberValue:int = exportSym("_AS3_NumberValue",
  (new CProcTypemap(CTypemap.DoubleType,
  [CTypemap.AS3ValType]))
  .createC(AS3_NOP)[0]
);

function AS3_Get(obj:*, prop:*):*
{
  return obj[prop];
}

const i_AS3_Get:int = exportSym("_AS3_Get",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_Get)[0]
);

const i_AS3_GetS:int = exportSym("_AS3_GetS",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.StrType]))
  .createC(AS3_Get)[0]
);

function AS3_Set(obj:*, prop:*, val:*):void
{
  obj[prop] = val
}

const i_AS3_Set:int = exportSym("_AS3_Set",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_Set)[0]
);

const i_AS3_SetS:int = exportSym("_AS3_SetS",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.AS3ValType, CTypemap.StrType, CTypemap.AS3ValType]))
  .createC(AS3_Set)[0]
);

function AS3_Array(tt:String, sp:int):*
{
  var result:Array = [];

  if(!tt || !tt.length)
    return result;

  var a:Array = CTypemap.getTypesByNames(tt);

  for(var n:int = 0; n < a.length; n++)
  {
    var tm:CTypemap = a[n];
    var ts:int = tm.typeSize;
    var aa:Array = [];

    mstate.ds.position = sp;
    sp += ts;
    for(; ts; ts -= 4)
      aa.push(mstate.ds.readInt());
    result.push(tm.fromC(aa));
  }
  return result;
}

const i_AS3_Array:int = exportSym("_AS3_Array",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.StrType], true /*varargs*/))
  .createC(AS3_Array)[0]
);

function AS3_Object(tt:String, sp:int):*
{
  var result:Object = {};

  if(!tt || !tt.length)
    return result;

  var a:Array = tt.split(/\s*[,\:]\s*/);

  for(var n:int = 0; n < a.length; n+=2)
  {
    var name:String = a[n];
    var tm:CTypemap = CTypemap.getTypeByName(a[n+1]);
    var ts:int = tm.typeSize;
    var aa:Array = [];

    mstate.ds.position = sp;
    sp += ts;
    for(; ts; ts -= 4)
      aa.push(mstate.ds.readInt());
    result[name] = tm.fromC(aa);
  }
  return result;
}

const i_AS3_Object:int = exportSym("_AS3_Object",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.StrType], true /*varargs*/))
  .createC(AS3_Object)[0]
);

function AS3_Call(func:*, thiz:Object, params:Array):*
{
  return func.apply(thiz, params);
}

const i_AS3_Call:int = exportSym("_AS3_Call",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_Call)[0]
);

function AS3_CallS(func:String, thiz:Object, params:Array):*
{
  return thiz[func].apply(thiz, params);
}

const i_AS3_CallS:int = exportSym("_AS3_CallS",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.StrType, CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_CallS)[0]
);

function AS3_CallT(func:*, thiz:Object, tt:String, sp:int):*
{
  return func.apply(thiz, AS3_Array(tt, sp));
}

const i_AS3_CallT:int = exportSym("_AS3_CallT",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType, CTypemap.StrType], true))
  .createC(AS3_CallT)[0]
);

function AS3_CallTS(func:String, thiz:Object, tt:String, sp:int):*
{
  return thiz[func].apply(thiz, AS3_Array(tt, sp));
}

const i_AS3_CallTS:int = exportSym("_AS3_CallTS",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.StrType, CTypemap.AS3ValType, CTypemap.StrType], true))
  .createC(AS3_CallTS)[0]
);

function AS3_Shim(func:Function, thiz:Object, rt:String, tt:String,
  varargs:Boolean):int
{
  var retType:CTypemap = CTypemap.getTypeByName(rt);
  var argTypes:Array = CTypemap.getTypesByNames(tt);
  var tm:CTypemap = new CProcTypemap(retType, argTypes, varargs);

  var id:int = tm.createC(function(...rest):*
  {
    return func.apply(thiz, rest);
  })[0];
  return id;
}

const i_AS3_Shim:int = exportSym("_AS3_Shim",
  (new CProcTypemap(CTypemap.PtrType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType, CTypemap.StrType, CTypemap.StrType,
   CTypemap.IntType]))
  .createC(AS3_Shim)[0]
);

function AS3_New(constr:*, params:Array):*
{
  switch(params.length)
  {
  case 0:
    return new constr;
  case 1:
    return new constr(params[0]);
  case 2:
    return new constr(params[0], params[1]);
  case 3:
    return new constr(params[0], params[1], params[2]);
  case 4:
    return new constr(params[0], params[1], params[2], params[3]);
  case 5:
    return new constr(params[0], params[1], params[2], params[3], params[4]);
  }

  log(1, "New with too many params! (" + params.length + ")");
  return undefined;
}

const i_AS3_New:int = exportSym("_AS3_New",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_New)[0]
);

function AS3_Function(data:int, func:Function):Function
{
  return function(...args):*
  {
    return func(data, args);
  }
}

const i_AS3_Function:int = exportSym("_AS3_Function",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.PtrType, 
    new CProcTypemap(CTypemap.AS3ValType,
    [CTypemap.PtrType, CTypemap.AS3ValType])
  ]))
  .createC(AS3_Function)[0]
);

function AS3_FunctionAsync(data:int, func:Function):Function
{
  return function(...args):*
  {
    var asyncHandler:Function = args.shift();

    return func(asyncHandler, data, args);
  }
}

const i_AS3_FunctionAsync:int = exportSym("_AS3_FunctionAsync",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.PtrType, 
    new CProcTypemap(CTypemap.AS3ValType,
    [CTypemap.PtrType, CTypemap.AS3ValType], false /*varargs*/, true /*async*/)
  ]))
  .createC(AS3_FunctionAsync)[0]
);

function AS3_FunctionT(data:int, func:int, rt:String, tt:String,
  varargs:Boolean):Function
{
  var tm:CTypemap = new CProcTypemap(CTypemap.getTypeByName(rt),
    CTypemap.getTypesByNames(tt), varargs);

  return AS3_Function(data, tm.fromC([func]));
}

const i_AS3_FunctionT:int = exportSym("_AS3_FunctionT",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.PtrType, CTypemap.PtrType, CTypemap.StrType, CTypemap.StrType,
   CTypemap.IntType
  ]))
  .createC(AS3_FunctionT)[0]
);

function AS3_FunctionAsyncT(data:int, func:int, rt:String, tt:String,
  varargs:Boolean):Function
{
  var tm:CTypemap = new CProcTypemap(CTypemap.getTypeByName(rt),
    CTypemap.getTypesByNames(tt), varargs, true);

  return AS3_FunctionAsync(data, tm.fromC([func]));
}

const i_AS3_FunctionAsyncT:int = exportSym("_AS3_FunctionAsyncT",
  (new CProcTypemap(CTypemap.AS3ValType,
  [CTypemap.PtrType, CTypemap.PtrType, CTypemap.StrType, CTypemap.StrType,
   CTypemap.IntType
  ]))
  .createC(AS3_FunctionAsyncT)[0]
);

function AS3_InstanceOf(val:*, type:Class):Boolean
{
  return val instanceof type;
}

const i_AS3_InstanceOf:int = exportSym("_AS3_InstanceOf",
  (new CProcTypemap(CTypemap.IntType,
  [CTypemap.AS3ValType, CTypemap.AS3ValType]))
  .createC(AS3_InstanceOf)[0]
);

function AS3_Stage():Object
{
  return gsprite ? gsprite.stage : null;
}

const i_AS3_Stage:int = exportSym("_AS3_Stage",
  (new CProcTypemap(CTypemap.AS3ValType, []))
  .createC(AS3_Stage)[0]
);



function AS3_ArrayValue(array:Array, tt:String, sp:int):void
{
  if(!tt || !tt.length)
    return;

  var a:Array = tt.split(/\s*,\s*/);

 
 
  for(var n:int = 0; n < a.length && n < array.length; n++)
  {
    var tm:CTypemap = CTypemap.getTypeByName(a[n]);

    mstate.ds.position = sp;

    var addr:int = mstate.ds.readInt();

    sp += 4;

    var aa:Array = tm.createC(array[n]);

    mstate.ds.position = addr;
    for(var i:int = 0; i < aa.length; i++)
      mstate.ds.writeInt(aa[i]);
  }
}

const i_AS3_ArrayValue:int = exportSym("_AS3_ArrayValue",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.AS3ValType, CTypemap.StrType], true /*varargs*/))
  .createC(AS3_ArrayValue)[0]
);

function AS3_ObjectValue(object:Object, tt:String, sp:int):void
{
  if(!tt || !tt.length)
    return;

  var a:Array = tt.split(/\s*[,\:]\s*/);

  for(var n:int = 0; n < a.length; n+=2)
  {
    var name:String = a[n];
    var tm:CTypemap = CTypemap.getTypeByName(a[n+1]);

    mstate.ds.position = sp;

    var addr:int = mstate.ds.readInt();

    sp += 4;

    var aa:Array = tm.createC(object[name]);

    mstate.ds.position = addr;
    for(var i:int = 0; i < aa.length; i++)
      mstate.ds.writeInt(aa[i]);
  }
}

const i_AS3_ObjectValue:int = exportSym("_AS3_ObjectValue",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.AS3ValType, CTypemap.StrType], true /*varargs*/))
  .createC(AS3_ObjectValue)[0]
);

Alchemy::NoShell {

public namespace flash_delegate =
  "http://www.adobe.com/2008/actionscript/flash/delegate";


public dynamic class DynamicProxy extends Proxy
{
  flash_proxy override function callProperty(name:*, ...rest):*
  {
    return this.flash_delegate::callProperty(name, rest);
  }

  flash_proxy override function deleteProperty(name:*):Boolean
  {
    return this.flash_delegate::deleteProperty(name);
  }

  flash_proxy override function getDescendants(name:*):*
  {
    return this.flash_delegate::getDescendants(name);
  }

  flash_proxy override function getProperty(name:*):*
  {
    return this.flash_delegate::getProperty(name);
  }

  flash_proxy override function hasProperty(name:*):Boolean
  {
    return this.flash_delegate::hasProperty(name);
  }

  flash_proxy override function isAttribute(name:*):Boolean
  {
    return this.flash_delegate::isAttribute(name);
  }

  flash_proxy override function nextName(index:int):String
  {
    return this.flash_delegate::nextName(index);
  }

  flash_proxy override function nextNameIndex(index:int):int
  {
    return this.flash_delegate::nextNameIndex(index);
  }

  flash_proxy override function nextValue(index:int):*
  {
    return this.flash_delegate::nextValue(index);
  }

  flash_proxy override function setProperty(name:*, value:*):void
  {
    this.flash_delegate::setProperty(name, value);
  }

  flash_delegate var callProperty:Function;
  flash_delegate var deleteProperty:Function;
  flash_delegate var getDescendants:Function;
  flash_delegate var getProperty:Function;
  flash_delegate var hasProperty:Function;
  flash_delegate var isAttribute:Function;
  flash_delegate var nextName:Function;
  flash_delegate var nextNameIndex:Function;
  flash_delegate var nextValue:Function;
  flash_delegate var setProperty:Function;
}

function AS3_Proxy():*
{
  return new DynamicProxy();
}

}

Alchemy::Shell {

function AS3_Proxy():*
{
  return null;
}

}

const i_AS3_Proxy:int = exportSym("_AS3_Proxy",
  (new CProcTypemap(CTypemap.AS3ValType,
  [], false /*varargs*/))
  .createC(AS3_Proxy)[0]
);

function AS3_Ram():ByteArray
{
  return gstate.ds;
}

const i_AS3_Ram:int = exportSym("_AS3_Ram",
  (new CProcTypemap(CTypemap.AS3ValType,
  [], false /*varargs*/))
  .createC(AS3_Ram)[0]
);

function AS3_ByteArray_readBytes(ptr:int, ba:ByteArray, len:int):int
{
  if(len > 0)
  {
    if ( ba.bytesAvailable < len )
      len = ba.bytesAvailable
    ba.readBytes(gstate.ds, ptr, len);
    return len;
  }
  return 0;
}

const i_AS3_ByteArray_readBytes:int = exportSym("_AS3_ByteArray_readBytes",
  (new CProcTypemap(CTypemap.IntType,
  [CTypemap.IntType, CTypemap.AS3ValType, CTypemap.IntType],
  false /*varargs*/))
  .createC(AS3_ByteArray_readBytes)[0]
);

function AS3_ByteArray_writeBytes(ba:ByteArray, ptr:int, len:int):int
{
log(5, "--- wrteBytes: ba length = " + ba.length + " / " + len);
  if(len > 0)
  {
    ba.writeBytes(gstate.ds, ptr, len);
    return len;
  }
  return 0;
}

const i_AS3_ByteArray_writeBytes:int = exportSym("_AS3_ByteArray_writeBytes",
  (new CProcTypemap(CTypemap.IntType,
  [CTypemap.AS3ValType, CTypemap.IntType, CTypemap.IntType],
  false /*varargs*/))
  .createC(AS3_ByteArray_writeBytes)[0]
);

function AS3_ByteArray_seek(ba:ByteArray, offs:int, whence:int):int
{
  if(whence == 0)
    ba.position = offs;
  else if(whence == 1)
    ba.position += offs;
  else if(whence == 2)
    ba.position = ba.length + offs;
  else
    return -1;
  return ba.position;
}

const i_AS3_ByteArray_seek:int = exportSym("_AS3_ByteArray_seek",
  (new CProcTypemap(CTypemap.IntType,
  [CTypemap.AS3ValType, CTypemap.IntType, CTypemap.IntType],
  false /*varargs*/))
  .createC(AS3_ByteArray_seek)[0]
);

const i_AS3_Trace:int = exportSym("_AS3_Trace",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.AS3ValType],
  false /*varargs*/))
  .createC(trace)[0]
);

Alchemy::SetjmpAbuse
{

/* freeze/thaw support for generic machines and stacks of machines...

** a frozen machine is comprised of:
** [int] reference count
** [int] CTypemap.AS3ValType.valueTracker id for the machine's class
** [int[]] integral registers
** [double[]] double registers

** a frozen stack is a NULL-terminate array of pointers to frozen machines

*/

function acquireFreeze(ptr:int):void
{
log(4, "acquireFreeze(" + ptr + ")");
  mstate.ds.position = ptr;
  var rc:int = mstate.ds.readInt();
  mstate.ds.position = ptr;
  mstate.ds.writeInt(rc+1);
}

function releaseFreeze(ptr:int, free:Function):void
{
log(4, "releaseFreeze(" + ptr + ")");
  mstate.ds.position = ptr;
  var rc:int = mstate.ds.readInt();
  if(rc == 1)
  {
log(4, "releaseFreeze free");
    mstate.ds.position = ptr + 4;

    var classId:int = mstate.ds.readInt();

    free(ptr);
    CTypemap.AS3ValType.valueTracker.release(classId);
  }
  else
  {
    mstate.ds.position = ptr;
    mstate.ds.writeInt(rc-1);
  }
}

function sweepFreezes(free:Function):void
{

  var newFreezeSet:Object = {};
  var oldFreezeSet:Object = gsetjmpFreezeIRCs;

  for (var mach:Object in gsetjmpFrozenMachineSet)
  {
    var ptr:int = mach.freezeCache;

    if(ptr)
    {
      newFreezeSet[ptr] = oldFreezeSet[ptr];
      delete oldFreezeSet[ptr];
    }
  }
  for(var aptr:* in oldFreezeSet)
  {
    var count:int = oldFreezeSet[aptr];

    while(count--)
      releaseFreeze(aptr, free);
  }
  gsetjmpFreezeIRCs = newFreezeSet;
}

function freezeMachine(mach:Machine, alloc:Function):int
{
  try
  {
   
    var cache:int = mach.freezeCache;

    if(cache)
    {
      acquireFreeze(cache);

      return cache;
    }

   
    var clazz:Object = Object(mach).constructor;
    var size:int = 12 + clazz.intRegCount * 4 + clazz.NumberRegCount * 8;
    var ptr:int = alloc(size);

log(4, "freezeMachine 1: " + clazz);

    gsetjmpFreezeIRCs[ptr] += 1;
    gsetjmpFrozenMachineSet[mach] = 1;
   
   
    mach.freezeCache = ptr;
    mstate.ds.position = ptr;
    mstate.ds.writeInt(1);
    mstate.ds.writeInt(
      CTypemap.AS3ValType.valueTracker.acquire(clazz));
    mstate.ds.writeInt(mach.state);

   
    var i:int;

    for(i = clazz.intRegCount - 1; i >= 0; i--)
      mstate.ds.writeInt(mach["i" + i]);
    for(i = clazz.NumberRegCount - 1; i >= 0; i--)
      mstate.ds.writeDouble(mach["f" + i]);

log(4, "freezeMachine 2: " + clazz);

    return ptr;
  } catch(e:*) {}
  return 0;
}

function thawMachine(ptr:int):Machine
{
log(4, "thawMachine start (" + ptr + ")");
  mstate.ds.position = ptr + 4;

  var classId:int = mstate.ds.readInt();
log(4, "thawMachine cid: " + classId);
  var clazz:* = CTypemap.AS3ValType.valueTracker.get(classId);
  var mach:Machine = Machine(new clazz());
  
log(4, "thawMachine " + clazz);

  mach.state = mstate.ds.readInt();

log(4, "thawMachine state: " + mach.state);

 
  var i:int;

  for(i = clazz.intRegCount - 1; i >= 0; i--)
    mach["i" + i] = mstate.ds.readInt();
  for(i = clazz.NumberRegCount - 1; i >= 0; i--)
    mach["f" + i] = mstate.ds.readDouble();
log(4, "thawMachine regs");
  acquireFreeze(ptr);
  gsetjmpFreezeIRCs[ptr] += 1;
  gsetjmpFrozenMachineSet[mach] = 1;
  mach.freezeCache = ptr;
  return mach;
}

function freeStack(ptr:int, free:Function):void
{
  mstate.ds.position = ptr;

  var frame:int;

  while((frame = mstate.ds.readInt()) != 0)
  {
    releaseFreeze(frame, free);
    ptr += 4;
    mstate.ds.position = ptr;
  }
  free(ptr);
}

function freezeStack(alloc:Function):int
{
  var frames:Array = [];
  var mach:Machine = mstate.gworker.caller;
  var frame:int;

log(4, "freezeStack");

  while((frame = freezeMachine(mach, alloc)) != 0)
  {
log(4, "freezeStack: " + frame);
    acquireFreeze(frame);
    frames.push(frame);
    mach = mach.caller;
  }

  var ptr:int = alloc(4 + frames.length * 4);

  mstate.ds.position = ptr;
  for(var i:int = 0; i < frames.length; i++)
    mstate.ds.writeInt(frames[i]);
  mstate.ds.writeInt(0);
log(4, "freezeStack= " + ptr);
  return ptr;
}

function thawStack(ptr:int, curMach:Machine):Machine
{
  var mach:Machine = null;
  var firstMach:Machine = null;

  mstate.ds.position = ptr;

  var frame:int;

log(4, "thawStack(" + ptr + ")");
  while((frame = mstate.ds.readInt()) != 0)
  {
   
    var curMachOk:Boolean = (curMach && frame == curMach.freezeCache);
    var newMach:Machine;

    if(curMachOk)
      newMach = curMach;
    else
      newMach = thawMachine(frame);

log(4, "thawMachine(" + frame + ")");
    newMach.mstate = mstate;
    if(mach)
      mach.caller = newMach;
    if(!firstMach)
      firstMach = newMach;
   
/*    if(curMachOk)
      return firstMach;*/
    mach = newMach;
    ptr += 4;
    mstate.ds.position = ptr;
    if(curMach)
      curMach = curMach.caller;
  }
  if(mach)
    mach.caller = null;
  return firstMach;
}

}

function AS3_Reg_jmp_buf_AbuseHelpers(alloc:Function, free:Function):void
{
  Alchemy::SetjmpAbuse
  {
    gsetjmpAbuseAlloc = alloc;
    gsetjmpAbuseFree = free;
  }
}

function AS3_RegAbused_jmp_buf(ptr:int):void
{
log(4, "regAbused: " + ptr);
  Alchemy::SetjmpAbuse
  {
   
    gsetjmpAbuseMap[ptr] = {
      alloc: gsetjmpAbuseAlloc,
      free: gsetjmpAbuseFree,
     
      setjmp: function(ptr:int):void
      {
        var abuseObj:Object = gsetjmpAbuseMap[ptr];

        abuseObj.stack = freezeStack(abuseObj.alloc);
        sweepFreezes(abuseObj.free);
      },
     
      longjmp: function(ptr:int, mach:Machine):Machine
      {
        var abuseObj:Object = gsetjmpAbuseMap[ptr];

        return thawStack(abuseObj.stack, mach);
      },
      cleanup: function(ptr:int):void
      {
        var abuseObj:Object = gsetjmpAbuseMap[ptr];

        freeStack(abuseObj.stack, abuseObj.free);
      }
    };
    return;
  }
  log(1, "Can't RegAbused -- abuse support disabled");
}

function AS3_UnregAbused_jmp_buf(ptr:int):void
{
log(4, "unregAbused: " + ptr);
  Alchemy::SetjmpAbuse
  {
    gsetjmpAbuseMap[ptr].cleanup(ptr);
    delete gsetjmpAbuseMap[ptr];
    return;
  }
  log(1, "Can't UnregAbused -- abuse support disabled");
}

const i_AS3_Reg_jmp_buf_AbuseHelpers:int =
exportSym("_AS3_Reg_jmp_buf_AbuseHelpers",
  (new CProcTypemap(CTypemap.VoidType, [
    (new CProcTypemap(CTypemap.PtrType, [CTypemap.IntType])),
    (new CProcTypemap(CTypemap.VoidType, [CTypemap.PtrType]))
  ],
  false /*varargs*/))
  .createC(AS3_Reg_jmp_buf_AbuseHelpers)[0]
);


const i_AS3_RegAbused_jmp_buf:int = exportSym("_AS3_RegAbused_jmp_buf",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.PtrType],
  false /*varargs*/))
  .createC(AS3_RegAbused_jmp_buf)[0]
);

const i_AS3_UnregAbused_jmp_buf:int = exportSym("_AS3_UnregAbused_jmp_buf",
  (new CProcTypemap(CTypemap.VoidType,
  [CTypemap.PtrType],
  false /*varargs*/))
  .createC(AS3_UnregAbused_jmp_buf)[0]
);


Alchemy::NoShell
public class ConSprite extends Sprite
{
  private var runner:CRunner = new CRunner;

  public function ConSprite()
  {
    if(gsprite)
      log(1, "More than one sprite!");

    gsprite = this;

    runner.startSystem();
  }
}


Alchemy::NoShell
public class CLibDummySprite extends Sprite
{
}


Alchemy::Shell
public class ShellCon
{
  private var runner:CRunner = new CRunner;

  public function ShellCon()
  {
    runner.startSystem();
  }

  public function work():void
  {
    runner.work();
  }
}

Alchemy::NoShell
public class CLibInit
{
  public function supplyFile(path:String, data:ByteArray):void
  {
    gfiles[path] = data;
  }

  public function putEnv(key:String, value:String):void
  {
    genv[key] = value;
  }

  public function setSprite(sprite:Sprite):void
  {
    gsprite = sprite;
  }

  public function init():*
  {
    var runner:CRunner = new CRunner(true);
    var result:*;
    var saveState:MState = new MState(null);

   
    mstate.copyTo(saveState);

    var regged:Boolean;

    try
    {
     
      runner.startSystem();
      while(true)
      {
        try
        {
          while(true)
            runner.work();
        }
        catch(e:AlchemyDispatch) {}
        catch(e:AlchemyYield) {}
      }
    }
    catch(e:AlchemyLibInit)
    {
      log(3, "Caught AlchemyLibInit " + e.rv);
      regged = true;
      result = CTypemap.AS3ValType.valueTracker.release(e.rv);
    }
    finally
    {
     
      saveState.copyTo(mstate);
     
      if(!regged)
        log(1, "Lib didn't register");
    }
    return result;
  }
}

Alchemy::Shell {

public function modEnd():void
{
  var ns:Namespace = new Namespace("avmshell");
  var sys:Object = ns::["System"];

  gargs = gargs.concat(sys.argv);

  var shellCon:ShellCon = new ShellCon();



  while(true)
  {


    shellCon.work();
  }
}

}

Alchemy::NoShell {

public function modEnd():void
{
}

}


Alchemy::NoShell {

public var gvglbmd:BitmapData;
public var gvglbm:Bitmap;
public var gvglpixels:int;

} // Alchemy::NoShell

public function vgl_lock():void
{
  // nop
}

public function vgl_unlock():void
{
  Alchemy::NoShell {

  // blit!
  if(gvglbmd && gvglpixels)
  {
    gstate.ds.position = gvglpixels;
    gvglbmd.setPixels(gvglbmd.rect, gstate.ds);
  }

  } // Alchemy::NoShell
}

public function vgl_end(dummy:int):int
{
  Alchemy::NoShell {

  var pixels:int = gvglpixels;
  gvglpixels = 0;
  return pixels;

  } // Alchemy::NoShell
  return 0;
}

public var vglKeys:Array = [];
public var vglKeyFirst:Boolean = true;
public var vglKeyUEL:*;
// mode...
// 1: VGL_RAWKEYS
// 2: VGL_CODEKEYS
// 3: VGL_XLATEKEYS
public var vglKeyMode:int;

public function vgl_keyinit(mode:int):int
{
  trace("vgl_keymode: " + mode);
  vglKeyMode = mode;
  return 0;
}

public function vgl_keych():int
{
  if(vglKeys.length)
    return vglKeys.shift();
  return 0;
}

public function vgl_init(width:int, height:int, pixels:int):int
{
  Alchemy::NoShell {

  var stage:Stage = gsprite.stage;

trace("vgl_init: " + width + " / " + height + " : " + pixels);
  if(vglKeyFirst)
  {
    // windows VK_ (keyCode) => scan code
    var vk2scan:Array = [
      0, 0, 0, 70, 0, 0, 0, 0, 14, 15, 0, 0, 76, 28, 0, 0, 
      42, 29, 56, 0, 58, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
      57, 73, 81, 79, 71, 75, 72, 77, 80, 0, 0, 0, 84, 82, 83, 99, 
      11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0, 
      0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24, 
      25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44, 91, 92, 93, 0, 95, 
      82, 79, 80, 81, 75, 76, 77, 71, 72, 73, 55, 78, 0, 74, 83, 53, 
      59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88, 100, 101, 102, 103, 
      104, 105, 106, 107, 108, 109, 110, 118, 0, 0, 0, 0, 0, 0, 0, 0, 
      69, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      42, 54, 29, 29, 56, 56, 106, 105, 103, 104, 101, 102, 50, 32, 46, 48, 
      25, 16, 36, 34, 108, 109, 107, 33, 0, 0, 39, 13, 51, 12, 52, 53, 
      41, 115, 126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 43, 27, 40, 0, 
      0, 0, 86, 0, 0, 0, 0, 0, 0, 113, 92, 123, 0, 111, 90, 0, 
      0, 91, 0, 95, 0, 94, 0, 0, 0, 93, 0, 98, 0, 0, 0, 0
    ];
    stage.addEventListener(KeyboardEvent.KEY_DOWN,
      function(event:KeyboardEvent)
      {
        var sc:int = (vglKeyMode == 2) ?
          vk2scan[event.keyCode & 0x7f] : event.charCode;

        vglKeys.push(sc);
      });
    stage.addEventListener(KeyboardEvent.KEY_UP,
      function(event:KeyboardEvent)
      {
        var sc:int = (vglKeyMode == 2) ?
          vk2scan[event.keyCode & 0x7f] : event.charCode;
  
        if(vglKeyMode == 2)
        {
          vglKeys.push(sc | 0x80);
        }
      });
    vglKeys.push(69); // push NUMLOCK so SDL thinks we're using the keypad...
    stage.focus = stage;
    vglKeyFirst = false;
  }
  gvglpixels = pixels;
  gvglbmd = new BitmapData(Math.abs(width), Math.abs(height), false);
  if(!gvglbm)
  {
    gvglbm = new Bitmap();
    gsprite.addChild(gvglbm);
  }
  gvglbm.bitmapData = gvglbmd;
  gvglbm.scaleX = gsprite.stage.stageWidth / width;
  gvglbm.scaleY = gsprite.stage.stageHeight / height;
trace("vgl_init done");

  } // Alchemy::NoShell

  return 0;
}

public var vglMouseFirst:Boolean = true;
public var vglMouseButtons:int;

function vgl_mouse_x():int
{
  Alchemy::NoShell {

  var stage:Stage = gsprite.stage;

  return stage.mouseX;

  } // Alchemy::NoShell

  return 0;
}

function vgl_mouse_y():int
{
  Alchemy::NoShell {

  var stage:Stage = gsprite.stage;

  return stage.mouseY;

  } // Alchemy::NoShell

  return 0;
}

function vgl_mouse_buttons():int
{
  Alchemy::NoShell {

  if(vglMouseFirst)
  {
    var stage:Stage = gsprite.stage;

    stage.addEventListener(MouseEvent.MOUSE_DOWN,
      function(event:MouseEvent)
      {
        vglMouseButtons = 1;
      });
    stage.addEventListener(MouseEvent.MOUSE_UP,
      function(event:MouseEvent)
      {
        vglMouseButtons = 0;
      });
    vglMouseFirst = false;
  }

  } // Alchemy::NoShell

  return vglMouseButtons;
}
