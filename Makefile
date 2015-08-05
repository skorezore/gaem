# The MIT License (MIT)

# Copyright (c) 2015 Skorezore

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


ifeq "$(OS)" "Windows_NT"
	CURSES_VARIANT := pd
	DEVNULL := nul
	FIND := busybox find
	EXE := .exe
else
	CURSES_VARIANT := n
	DEVNULL := /dev/null
	FIND := find
	EXE :=
endif

ifeq "$(TRAVIS)" "true"
	CXXCIAR := -static-libstdc++ -static-libgcc
else
	CXXCIAR :=
endif


.PHONY : all deps clean


all :
	@mkdir -p binaries 2>$(DEVNULL) || :
	@cp Cpponfiguration/out/*cpponfig.* binaries 2>$(DEVNULL) || :
	$(CXX) $(CXXCIAR) -Os $(foreach src,$(shell ls source | grep .cpp),source/$(src)) -obinaries/gaem$(EXE) -l$(CURSES_VARIANT)curses -std=c++14 -Wall -Wextra -pedantic -Idependencies -Lbinaries -lcpponfig
	strip --strip-all --remove-section=.comment --remove-section=.note binaries/gaem$(EXE)
	@rm -r binaries/assets 2>$(DEVNULL) || :
	@cp -r assets binaries/assets 2>$(DEVNULL) || :

deps :
	@rm -r dependencies 2>$(DEVNULL) || :
	@mkdir -p dependencies 2>$(DEVNULL) || :
	curl -s https://raw.githubusercontent.com/cxong/tinydir/master/tinydir.h -o dependencies/tinydir.h
	git submodule update --recursive --init
	$(MAKE) -CCpponfiguration dll
	cp -r Cpponfiguration/src dependencies/cpponfig && rm `$(FIND) dependencies/cpponfig -name *.cpp`

clean :
	rm -rf binaries dependencies
	$(MAKE) -CCpponfiguration clean
