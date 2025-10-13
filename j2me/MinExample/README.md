# J2ME NetBeans minimal example

Source:
- https://web.archive.org/web/20131023000838/http://www.java-tips.org/java-me-tips/midp/how-to-write-a-basic-midlet-2.html

## Install J2ME dev environment

Used tutorial for NetBeans on Linux:

https://microgram.app/blog/004-J2ME-development-in-2024-with-linux.html

## WTK:

`New Project` button

```
Project Name: MinExample
MIDlet Class Name:
```

## NetBeans

`File -> New Project -> Java ME -> Import Wireless Toolkit Project`
WTK location: `/home/d9k/j2mewtk/2.5.2`
Detected Applications: `MinExample`
Try build and run with emulator. For example, [KEmulator](https://github.com/shinovon/KEmulator).

## Polish

Copy imported project from target directory.
Replace `ImportedProject-MinExample1` with `MinExample`.
Try build and run with emulator.

`"Projects" sidebar -> [Your project] -> Application Descriptor -> MIDlets`: check to have only one record. Remove others.

## Autocompletion in IDE

`Tools -> Platforms -> CLDC -> Sun Java(TM) Wireless Toolkit -> Sources -> Add Folder...`.

Select `lib` from WTK distribution (which contains `midpapi10.jar`). 
