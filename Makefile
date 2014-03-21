src = test.cc $(wildcard src/*.cc)
obj = $(src:.cc=.o)
dep = $(obj:.o=.d)

bin = test

CFLAGS = -pedantic -Wall -g -Iinclude -Isrc
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lGL -lGLU -lglut -lpthread -ldl

$(bin): $(obj)
	$(CXX) -o $@ $(obj) $(LDFLAGS)

-include $(dep)


%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.d: %.cc
	@$(CPP) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) $(bin) $(dep)
