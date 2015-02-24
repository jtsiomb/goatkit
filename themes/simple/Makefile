PREFIX ?= /usr/local

src = $(wildcard *.cc)
obj = $(src:.cc=.o)
lib_so = simple.gtheme

CXXFLAGS = -pedantic -Wall $(pic) -g -I../../include
LDFLAGS = $(libgl)

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
	shared = -dynamiclib
	CXXFLAGS += -Wno-deprecated-declarations
else
	libgl = -lGL -lGLU -lglut
	shared = -shared
	pic = -fPIC
endif

$(lib_so): $(obj)
	$(CXX) -o $@ $(shared) $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)

.PHONY: install
install: $(lib_so)
	mkdir -p $(DESTDIR)$(PREFIX)/share/goatkit
	cp $(lib_so) $(DESTDIR)$(PREFIX)/share/goatkit/$(lib_so)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/share/goatkit/$(lib_so)
