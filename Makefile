CFLAGS = -dynamiclib -lpthread

all:
	$(CC) $(CFLAGS) sketchtrial.c -o libsketchtrial.dylib

clean:
	rm -rf *.o libsketchtrial.dylib
