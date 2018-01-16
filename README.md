Setup Eddystone LE Beacon
=========================

Advertises a url as an Eddystone beacon

Todo/Bugs:
* Assumes https:// prefix
* Does not encode suffix
* Fix the above

Usage:
* sudo ./beacon <url-without-protocol>

Example:
* sudo ./beacon www.lwn.net
