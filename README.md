Setup Eddystone LE Beacon
=========================

Advertises a URL as an Eddystone beacon, https://github.com/google/eddystone/

Todo/Bugs:
* Assumes https:// prefix
* Does not encode suffix
* Fix the above

Usage:
* sudo ./beacon <url-without-protocol>

Example:
* sudo ./beacon www.lwn.net
