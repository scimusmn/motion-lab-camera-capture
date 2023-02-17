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


2023-02-16
----------

looks like, for `VwOpenCameraByIndex`, the `pUserPointer` element is **NOT ALLOWED TO BE NULL**.
i'm not sure why this is -- null user data is a pretty normal thing -- but setting it to point
to something made it stop throwing an error. Oh well.

---

CameraSnap doesn't work very well -- it can't grab more than 255 frames before it just hangs. Better to use
CameraGrab and count how many have come in.
