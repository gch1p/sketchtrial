#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
export DYLD_INSERT_LIBRARIES=$DIR/libsketchtrial.dylib
/Applications/Sketch.app/Contents/MacOS/Sketch
