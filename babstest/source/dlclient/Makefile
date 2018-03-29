COMPTOOL = g++
GOAL = dlclient

OBJ = main.o

smallPrj : $(OBJ)
	$(COMPTOOL) -o $(GOAL) $(OBJ) -ldl

%.o : %.c
	$(COMPTOOL) -c $<

.PHONY: clean
clean : 
	rm -f *.o $(GOAL)

