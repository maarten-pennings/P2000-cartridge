import serial
import sys
import time
import datetime


print( "P2000 cartridge burner")


class AFPException(Exception):
    def __init__(self,*args,**kwargs):
        Exception.__init__(self,*args,**kwargs)


class AFP:
    """Send AFP commands over a serial port and captures response."""
    def __init__(self):
        """Creates an instance. """
        self.serial= None
        self.logfile= None
        self.logtime0= time.time()
    def logstart(self,filename,filemode="w"):
        """Starts logging to file 'filename' (for write - may overrule for append) of all commands and responses."""
        self.logfilename= filename
        self.logfile= open(filename,filemode)
        self.log(f"Log start {filename} {datetime.datetime.today()}" )
    def logstop(self):
        if self.logfile!=None:
            """Stops logging."""
            self.log('Log stop')
            self.logfile.close()
            self.logfile= None
    def log(self,msg,tag="!"):
        """Add message 'msg' to the log, tagging the message with character 'tag'."""
        # Prepends time stamp and tag
        # Make NLs visible by rendering them as "«"
        rep= f"«\n{'':8} {tag} " # Spaces are for indent (size of time)
        msg= msg.replace("\n",rep)
        if msg.endswith(rep): msg= msg[0:len(msg)-len(rep)+1]
        self.logfile.write( f"{time.time()-self.logtime0:8.3f} {tag} {msg}\n" )
    def open(self,port):
        """Opens serial port 'port' towards the AFP."""
        if self.logfile!=None: self.log('open port '+port)
        if self.serial!=None: raise AFPException("open(): COM port already open - Tip: either call e=AFP();e.open(\"COM4\") or e=AFP(\"COM4\").")
        self.serial= serial.Serial(port,baudrate=115200,timeout=0.01)
        self.rxbuf=b"" # Clear the receive buffer.
        res=self.exec("") # Clear any pending command.
        res=self.exec("echo disable","echo: echoing disabled\n>> ") # Explicitly sync.
    def close(self, nice=True):
        """Closes the serial port. When 'nice', first re-enables echoing."""
        if nice : res=self.exec("echo enable\n") # Switch echo back on
        if self.logfile!=None: self.log("close port")
        self.serial.close()
        self.serial= None
    def isopen(self):
        """Returns true if the serial port is open."""
        return not self.serial is None
    def exec(self,icmd,isync=">> ", timeout_sec = 1.5 ):
        """Send command 'icmd'; waiting for response to have 'isync'. Returns response up to 'isync'."""
        cmd= icmd.encode(); 
        sync= isync.encode() # Convert strings to bytes
        self.serial.write(cmd)
        self.serial.write(b"\n")
        if self.logfile!=None: self.log(icmd+"\n",">")
        pos,time0 = -1,time.time();
        while (pos<0) and (time.time()-time0<timeout_sec):
            self.rxbuf+= self.serial.read(1000)
            pos= self.rxbuf.find(sync)
        if pos<0 : 
            raise AFPException("exec(): sync not received ["+self.rxbuf.decode()+"]")
        if self.logfile!=None: self.log(self.rxbuf[:pos+len(sync)].decode(),"<")
        res=self.rxbuf[:pos]
        self.rxbuf=self.rxbuf[pos+len(sync):]
        return res.decode() # Convert bytes to strings


if len(sys.argv) != 4 :
  print("SYNTAX python[3] burn.py <infile> <serialport> <logfile>")
  sys.exit(20)
inname= sys.argv[1]
serialport= sys.argv[2]
logname= sys.argv[3]

try:
  afp = AFP()
  print(f"log  '{logname}'")
  afp.logstart(logname)
  
  print(f"port '{serialport}'")
  afp.open(serialport)
  
  afp.log(f"opening '{inname}'")
  print(f"file '{inname}'")
  print("burn ")
  with open(inname,'r') as infile:
    for ix,line in enumerate(infile):
      afp.exec(line[:-1]) # strip \n from text line
      if ix%100==0 : print(f"  {ix//10}",flush=True)

except serial.SerialException as ex: 
  print( f"Could not open port {serialport}. Is AFP plugged in? Is no other app using it? Is the VCOM driver installed?" )
  
finally:
  if afp.isopen(): afp.close()
  print("done")




