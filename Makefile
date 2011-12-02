CXX=g++
CFLAGS=-g -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
LDFLAGS=-lGL -lGLU -lglut -lGLEW

all: tanks

tanks: libtanks/libtanks.a libk3d/libk3d.a tanks.o
	$(CXX) $(CFLAGS) -o tanks tanks.o libtanks/libtanks.a libk3d/libk3d.a $(LDFLAGS)

tanks.o: tanks.cpp
	$(CXX) $(CFLAGS) -I. -c $<

libtanks/libtanks.a:
	make -C libtanks

libk3d/libk3d.a:
	make -C libk3d

clean:
	rm -rf tanks *.o

cleanlibs:
	make -C libtanks clean
	make -C libk3d clean
