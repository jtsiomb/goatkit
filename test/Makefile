src = $(wildcard src/*.cc)
obj = $(src:.cc=.o)
dep = $(obj:.o=.d)

bin = test

CFLAGS = -pedantic -Wall -g -I../include
CXXFLAGS = $(CFLAGS)
LDFLAGS = -L.. -lgoatkit $(libgl) -lpthread -ldl

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
	CFLAGS += -Wno-deprecated-declarations
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj)
	$(CXX) -o $@ $(obj) $(LDFLAGS)

.PHONY: goatkit
goatkit:
	$(MAKE) -C .. shared

-include $(dep)


%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.d: %.cc
	@$(CPP) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) $(bin) $(dep)
