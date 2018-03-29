COMPTOOL = gcc

smallPrj : main.o ant.o bee.o 
	$(COMPTOOL) -o smallPrj main.o ant.o bee.o 

main.o : main.c
	$(COMPTOOL) -c main.c

ant.o : ant.c ant.h
	$(COMPTOOL) -c ant.c

bee.o : bee.c bee.h
	$(COMPTOOL) -c bee.c


.PHONY: clean
clean : 
	rm -f *.o smallPrj
