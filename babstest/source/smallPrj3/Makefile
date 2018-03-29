COMPTOOL = g++

OBJ = main.o

smallPrj : $(OBJ)
	$(COMPTOOL) -o smallPrj $(OBJ) -ldl

%.o : %.c
	$(COMPTOOL) -c $<

.PHONY: clean
clean : 
	rm -f *.o smallPrj

