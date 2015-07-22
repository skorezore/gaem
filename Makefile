all:
	@mkdir -p binaries 2>nul || :
	$(CXX) -Os $(foreach src,$(shell ls source | grep .cpp),source/$(src)) -obinaries/Sandbox -static -std=c++14 -Wall -Wextra -pedantic
