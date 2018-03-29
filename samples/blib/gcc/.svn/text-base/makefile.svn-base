CC=g++
CXXFLAGS=-c -O3 -I../include
OBJECTS=error.o FileTools.o socket.o StringTools.o
BINARY=../lib/blib.a

all: $(BINARY)

$(BINARY): $(OBJECTS)
	ar rs $(BINARY) $(OBJECTS)

error.o: ../source/error.cpp
	$(CC) $(CXXFLAGS) ../source/error.cpp

FileTools.o: ../source/FileTools.cpp
	$(CC) $(CXXFLAGS) ../source/FileTools.cpp

socket.o: ../source/socket.cpp
	$(CC) $(CXXFLAGS) ../source/socket.cpp

StringTools.o: ../source/StringTools.cpp
	$(CC) $(CXXFLAGS) ../source/StringTools.cpp

clean:
	rm -f *o $(BINARY)
