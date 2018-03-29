# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp .c

# Build flags
COMPFLAGS          = -g -Wall -Iinc
#COMPFLAGS          =  -Iinc -DNDEBUG
LINKFLAGS          = 
LINKLIBS            =

# Build tools
COMPTOOL = Y:\dev\tcc\tcc.exe
LINKTOOL = $(COMPTOOL)

# Build commands
COMPCMD = $(COMPTOOL) $(COMPFLAGS) -o $@ -c $<
LINKCMD = $(LINKTOOL) $(LINKFLAGS) -o $@ $^ $(LINKLIBS)

# Source files in this project
SRC := $(wildcard src/*.cpp)$(wildcard src/*.c)

# Object files to make
OBJ := \
  $(patsubst %.c,%.o, $(filter %.c,$(SRC))) \
  $(patsubst %.cpp,%.o, $(filter %.cpp,$(SRC)))

# Suffix rules for making object files from source files
.cpp.o :
	$(COMPCMD)
.c.o :
	$(COMPCMD)

# Rule for linking the executable
clapitest.exe: $(OBJ)
	$(LINKCMD)

.PHONY:		clean
clean:
		rm -f clapitest src/*.o

.PHONY:		dotest
dotest:
		echo MAKEFILE_LIST='$(MAKEFILE_LIST)'

