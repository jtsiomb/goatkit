src = test.cc $(wildcard src/*.cc)
obj = $(src:.cc=.o)
dep = $(obj:.o=.d)

bin = test

CFLAGS = -pedantic -Wall -g -Iinclude -Isrc
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(libgl) -lpthread -ldl

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
	CFLAGS += -Wno-deprecated-declarations
else
	libgl = -lGL -lGLU -lglut
endif

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
