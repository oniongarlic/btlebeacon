# A Linux Eddystone Beacon

A tiny Eddystone beacon for Linux, in pure C.
It supports broadcasting of URL, UID and TLM Eddystone frames.

See https://github.com/google/eddystone/ for details about Eddystone.

## Building
The only dependency at this time is bluetooth headers and library

## Todo/Bugs/Features:
* NID and BID are used as strings, add support for hex input
* NID and BID defaults to 0123456789 ABCDEF
* UID can not be set
* Add AltBeacon
* Add iBeacon

Bluetooth LE requires root or capabilities set:
* Run "make setcap" to set capabilities flags on the binary so sudo/root is not needed

## Usage:
* ./beacon url [NID BID]

## Examples:
* ./beacon https://www.tal.org/ 
* ./beacon https://www.tal.org/ 0123456789 ABCDEF
