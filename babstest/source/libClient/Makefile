# Makefile for small  project using a static library

# clear out all suffixes
.SUFFIXES:

# Location of directories
ROOTPATH = /home/wezzel/dev/babstest
BUILDROOT = $(ROOTPATH)/build
MODULE_NAME = libClient
MODULE_BUILDDIR = $(BUILDROOT)/$(MODULE_NAME)
GOAL = $(MODULE_BUILDDIR)/$(MODULE_NAME)

# Build flags
DEPFLAGS = -MMD -MP -MT $@ -MT $(@:.o=.d)
COMPFLAGS          = -g -Wall -Iinc -I../libPrj/inc
#COMPFLAGS          =  -Iinc -Wall -DNDEBUG
LINKFLAGS          = -L$(BUILDROOT)/libPrj
LINKLIBS            = -llibPrj

# Build tools
COMPTOOL = g++
LINKTOOL = $(COMPTOOL)

# Build commands
COMPCMD = $(COMPTOOL) $(DEPFLAGS) $(COMPFLAGS) -o $@ -c $<
#LINKCMD = $(LINKTOOL) $(LINKFLAGS) -o $@ $^ $(LINKLIBS)
LINKCMD = $(LINKTOOL) $(LINKFLAGS) -o $@ $(OBJ) $(LINKLIBS)

# Source files in this project
SRC := $(wildcard src/*.c) $(wildcard src/*.cpp)

# Object files to make
OBJ := \
	$(patsubst %.c,$(MODULE_BUILDDIR)/%.o, $(filter %.c,$(SRC))) \
	$(patsubst %.cpp,$(MODULE_BUILDDIR)/%.o, $(filter %.cpp,$(SRC)))

# Rule for linking the executable
$(GOAL): $(OBJ) $(BUILDROOT)/libPrj/liblibPrj.a
	$(LINKCMD)

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


