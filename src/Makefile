# Copyright 2007 Las Venturas Playground. All rights reserved.
# Use of this source code is governed by the GPLv2 license, a copy of which can
# be found in the LICENSE file.

# Compile the Echo project on Linux systems. Just run "make"

$(shell [ -d "out" ] || mkdir -p out)

OUTFILE="echo.so"
COMPILE_FLAGS=-c -m32 -fPIC -O3 -w -DLINUX -DHAVE_STDINT_H -I.

all:
	g++ $(COMPILE_FLAGS) sdk/amxplugin.cpp -o out/amxplugin.o
	g++ $(COMPILE_FLAGS) echo.cpp -o out/echo.o
	g++ -O2 -m32 -fshort-wchar -shared -o $(OUTFILE) out/*.o
