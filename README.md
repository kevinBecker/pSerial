Serial Message Sender (pSerial)
================
THIS IS STILL A WORK IN PROCESS, DO NOT USE

This is a simple moos app that sends serial messages from the MOOS DB to a given port. Code was adapted from iGPS.


The configuration file must contain:
```C++
ProcessConfig = pSerial
{
   AppTick   	= 4
   CommsTick 	= 4

   PORT  		= /dev/ttyUSB0 // serial port number where the GPS is plugged in
   BAUDRATE 	= 4800 // the baudrate to use
}
```
An example is provided in the `src` folder.

## Installation:
Follow the iGPS documentation for info on how to install.
