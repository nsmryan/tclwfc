


tclwfc.so: src/tclwfc.c
	$(CC) -shared -ggdb -fPIC -o $@ -DUSE_TCL_STUBS -Istb -Iwfc src/tclwfc.c -ltclstub8.6

.PHONY: test clean
test:
	tclsh test.tcl

clean:
	rm tclwfc.so

