blib_lib_dir=../../../lib
blib_make_dir=../../../gcc
blib_src_dir=../../../source
blib_inc_dir=../../../include
src_dir=../source
inc_dir=../include
bin_dir=../bin

CC=g++
CFLAGS=-c -O3 -I$(inc_dir) -I$(blib_inc_dir)

all: SocketTest

SocketTest: STApp.o $(blib_lib_dir)/blib.a
	$(CC) STApp.o $(blib_lib_dir)/blib.a -o $(bin_dir)/SocketTest

STApp.o: $(src_dir)/STApp.cpp
	$(CC) $(CFLAGS) $(src_dir)/STApp.cpp

$(blib_lib_dir)/blib.a: 
	make -C $(blib_make_dir)

clean:
	rm -rf *o SocketTest
