# Makefile for small testproject building a static library

# clear out all suffixes
.SUFFIXES:

# Location of directories
ROOTPATH = /home/wezzel/dev/babstest
BUILDROOT = $(ROOTPATH)/build
MODULE_NAME = libPrj
MODULE_BUILDDIR = $(BUILDROOT)/$(MODULE_NAME)
GOAL = $(MODULE_BUILDDIR)/lib$(MODULE_NAME).a

# Build flags
DEPFLAGS = -MMD -MP -MT $@ -MT $(@:.o=.d)
COMPFLAGS          = -g -Wall -Iinc
#COMPFLAGS          =  -Iinc -Wall -DNDEBUG
LINKFLAGS          = 
LINKLIBS            =

# Build tools
COMPTOOL = g++
LINKTOOL = $(COMPTOOL)

# Build commands
COMPCMD = $(COMPTOOL) $(DEPFLAGS) $(COMPFLAGS) -o $@ -c $<
LINKCMD = $(LINKTOOL) $(LINKFLAGS) -o $@ $^ $(LINKLIBS)
#STATICLIBCMD = ar rcs $@ $?
STATICLIBCMD = ar rcs $@ $?

# Source files in this project
SRC := $(wildcard src/*.cpp) $(wildcard src/*.c)

# Object files to make
OBJ := \
	$(patsubst %.c,$(MODULE_BUILDDIR)/%.o, $(filter %.c,$(SRC))) \
	$(patsubst %.cpp,$(MODULE_BUILDDIR)/%.o, $(filter %.cpp,$(SRC)))

# Rule for building the library
$(GOAL): $(OBJ)
	$(STATICLIBCMD)

# Pattern rules for making object files from source files
$(MODULE_BUILDDIR)/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(COMPCMD)
$(MODULE_BUILDDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(COMPCMD)


# Customization for testfun2
#$(MODULE_BUILDDIR)/src/testfun2.o : src/testfun2.c
#	@mkdir -p $(dir $@)
#	$(COMPCMD) -DMORE_HEADERS

# Include source file dependencies
-include $(OBJ:.o=.d)

.PHONY:		clean
clean:
	rm -rf $(MODULE_BUILDDIR)

.PHONY:		dotest
dotest:
	echo This is a test


