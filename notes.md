notes
=====

2023-02-15
----------

it seems that the command prompt you use will set the MSBUILD type to either x86 or x64.
i'm using x86 cmd (i guess??) so i have it linking to VIS_32 right now, but this should
probably be made more robust at some point.

---


static ips are required! i have them set to

  * camera: 192.168.137.2
  * computer: 192.168.137.3
  * gateway: 192.168.137.1

but, thanks to auto-enumeration, they can be whatever. they just need to be consistent.