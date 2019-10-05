# sketchtrial

This is a shared library that spoofs some ObjC method calls to disable Sketch license verification.

Tested on Sketch 58.

Make sure to disable SIP (`csrutil disable` from Recovery), for more information see below.

## Building
```
git clone https://github.com/gch1p/sketchtrial
cd sketchtrial
make
```

## Launching Sketch
You can use `sketch.sh`, it's a wrapper script that sets necessary environment variables to insert the lib and launches Sketch (it assumes that Sketch.app installed to /Applications, edit the script if it differs for you).

## Important
This hack doesn't work on systems with SIP (System Integrity Protection) enabled, and it's enabled by default since El Capitan. It can be easily disabled from Recovery Mode but it's another layer of security which is always good to have so disabling it permanently is not the wisest idea. I'll implement some solution to bypass this restriction when I have time.
