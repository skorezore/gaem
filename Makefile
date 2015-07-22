all:
	$(CXX) -Os Source/Main.cpp -oBinaries/Sandbox -static -std=c++14 -Wall -Wextra -pedantic
