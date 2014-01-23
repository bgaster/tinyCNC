# Copyright 2014 Benedict R. Gaster
# License: See the file license.

CXX=clang++
CXXFLAGS=-Wunused-comparison -O0 -m64 -g -stdlib=libc++ -std=c++0x -F /Library/Frameworks -DNDEBUG -D__DEBUG__=1 -I./include -I../arduino/lib/include -I./src 

CC=clang
CCFLAGS=-O2 -m64 

LIBS=-L/usr/local/lib -lm -lmare -framework OpenGL -framework GLUT -F /Library/Frameworks -framework GLUI

CPP_OBJS=$(addprefix ./, $(CPP_SRC:.cpp=.o))
OBJS=$(CPP_OBJS) 

default: $(EXAMPLE)

all: $(EXAMPLE) 


.depend: ./.depend

.depend: $(CPP_SRC)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ -MF ./.depend

.PHONY: clean

clean:
	rm -f src/*.o *.o $(EXAMPLE)

$(EXAMPLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: ../%.cpp 
	$(CXX) -MMD $< $(CXXFLAGS) -c -o $@

