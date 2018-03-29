COMPTOOL = gcc
SONAME = libdllib.so.1
GOAL = $(SONAME).0.1

OBJ = testfun1.o testfun2.o

$(GOAL) : $(OBJ)
	$(COMPTOOL) -shared -Wl,-soname,$(SONAME) -o $(GOAL) $(OBJ) 

%.o : %.c
	$(COMPTOOL) -fPIC -g -Wall -c $<

.PHONY: clean
clean : 
	rm -f *.o $(GOAL)

