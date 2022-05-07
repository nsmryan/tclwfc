

CFLAGS += -Istb -Iwfc 
CFLAGS += -shared -fPIC 
CFLAGS += -DUSE_TCL_STUBS 
CFLAGS += -ggdb 

tclwfc.so: src/tclwfc.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ src/tclwfc.c -ltclstub8.6

.PHONY: test clean dist
dist: tclwfc.so
	-@rm tclwfc -rf
	mkdir build/tclwfc -p
	cp src/*.tcl build/tclwfc/
	cp tclwfc.so build/tclwfc/

test: dist
	tkcon.tcl test.tcl

clean:
	rm tclwfc.so tclwfc -rf

