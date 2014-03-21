src = $(wildcard *.cc)
obj = $(src:.cc=.o)
lib_so = simple.so

CXXFLAGS = -pedantic -Wall -g -I../../include

$(lib_so): $(obj)
	$(CXX) -o $@ -shared $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
