COMPTOOL = gcc

OBJ = main.o ant.o bee.o 

MyVar := general

ant.o : MyVar:=specific

smallPrj : $(OBJ)
	@echo 
	$(COMPTOOL) -o smallPrj $(OBJ) 

%.o : InfoString = Value of MyVar is: '$(MyVar)'

%.o : %.c
	@echo 
	@echo MyVar=$(MyVar)
	@echo InfoString=$(InfoString)
	$(COMPTOOL) -c $<

.PHONY: clean
clean : 
	rm -f *.o smallPrj

