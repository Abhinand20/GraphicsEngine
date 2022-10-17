CXX=g++
RM=rm -f
CPPFLAGS=-std=c++14
LDFLAGS=
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
SRCS=driver.cpp mesh.cpp engine.cpp window.cpp
OBJS=$(SRCS:.cpp=.o)
SFMLPATH=

all: object

object: $(OBJS)
	$(CXX) -c $(CPPFLAGS) $(SRCS) $(LDLIBS)
	$(CXX) -o object.exe  $(OBJS) $(LIBFLAGS)

clean:
	$(RM) $(OBJS)