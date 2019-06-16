CFLAGS = -dynamiclib -framework AppKit -framework Foundation

all:
	$(CC) $(CFLAGS) sketchtrial.m -o libsketchtrial.dylib

clean:
	rm -rf *.o libsketchtrial.dylib
