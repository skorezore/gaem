all:
	@md Binaries 2>nul
	$(CXX) -Os Source/Main.cpp -oBinaries/Sandbox -static -std=c++14 -Wall -Wextra -pedantic
