src = test.cc $(wildcard src/*.cc)
obj = $(src:.cc=.o)

bin = test

CFLAGS = -pedantic -Wall -g -Iinclude
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lGL -lGLU -lglut

$(bin): $(obj)
	$(CXX) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
