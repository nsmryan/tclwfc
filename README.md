# tclwfc

This repository contains a toy TCL extension for using the WFC algorithm in TCL using krychu/wfc.

The intent is to play with the WFC algorithm from TCL to explore visualization
dynamically. This extension does not use TEA, but 'make dist' does create a TCL extension
in 'build/tclwfc'.


To try it out, just type:

```
make test
```

This brings up Tkcon and a canvas. 

The command 'redraw' will draw with the default image, while

```
wfc gen wfc/samples/twolines2.png
redraw
```

Will use a different image.


## Dependencies

The test code assumes that you have tkcon.tcl in your PATH and that it is
executable.

