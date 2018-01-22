Setup Eddystone LE Beacon
=========================

Eddystone beacon. Support URL, UID and TLM frames.

Advertises a URL as an Eddystone beacon with support for telemetry (TLM) and UID frames.

See https://github.com/google/eddystone/ for details about Eddystone.

Todo/Bugs:
* Assumes https:// prefix, url must be without
* Does not encode suffix yet (FIXME)
* NID and BID are used as strings
* NID and BID defaults to 0123456789 ABCDEF
* UID can not be set

Bluetooth LE requires root or capabilities set:
* Run "make setcap" to set capabilities flags on the binary so sudo/root is not needed

Usage:
* ./beacon url [nid bid]

Examples:
* ./beacon www.tal.org 
* ./beacon www.tal.org 0123456789 ABCDEF
