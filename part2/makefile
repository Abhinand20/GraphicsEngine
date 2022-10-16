CXX=g++
RM=rm -f
CPPFLAGS=-std=c++14
LDFLAGS=
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

SRCS=driver.cpp mesh.cpp engine.cpp window.cpp
OBJS=$(subst .cc,.o,$(SRCS))

all: object

object: $(OBJS)
	$(CXX) -c $(CPPFLAGS) $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) tool