#Makefile like Riot-os does

# set undefined variables

CURDIR ?= $(PWD)
PROJECTBASE       ?= $(dir $(lastword $(MAKEFILE_LIST)))
#CCACHE_BASEDIR ?= $(RIOTBASE)
#RIOTCPU        ?= $(RIOTBASE)/cpu
#RIOTBOARD      ?= $(RIOTBASE)/boards
#RIOTPKG        ?= $(RIOTBASE)/pkg
#RIOTPROJECT    ?= $(shell git rev-parse --show-toplevel 2>/dev/null || pwd)
#GITCACHE       ?= $(RIOTBASE)/dist/tools/git/git-cache
APPDIR         ?= $(CURDIR)
BINDIRBASE     ?= $(APPDIR)/bin
#	BINDIR ?= $(BINDIRBASE)/$(BOARD)
DIR_PLATFORM    ?= $(APPDIR)/platform

#Get Only the Internal Structure of SRCDIR Recursively
#Dont include certain file
_REMOVE_FROM_STRUCTURE := Kvaser_USBcan_II_HS-HS .git
REMOVE_FROM_STRUCTURE := $(strip $(foreach src_folder, $(_REMOVE_FROM_STRUCTURE), ! -path "*/$(src_folder)*" ))
STRUCTURE := $(shell find $(APPDIR) -type d $(REMOVE_FROM_STRUCTURE))
#Get All Files inside the STRUCTURE Variable
SRCDIR = $(STRUCTURE)
OBJDIR = obj
CODEFILES := $(addsuffix /*,$(STRUCTURE))
CODEFILES := $(wildcard $(CODEFILES))
#Filter Out Only Specific Files
SRCFILES := $(filter %.c,$(CODEFILES))
HDRFILES := $(filter %.h,$(CODEFILES))


_OBJFILES := $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES:%.c=%.o))
#a amiliorer
OBJFILES := $(strip  $(filter-out %canwatch.o %cansend.o, $(_OBJFILES)))

# Filter Out Function main for Libraries
LIBDEPS := $(filter-out $(OBJDIR)/main.o,$(OBJFILES))


#TODO:Le metre dans un choix fait par un define exterieur
DIR_NATIVE ?= $(APPDIR)/drivers/native

#what we use now for the projetct
#_DEPS = lowLevel_CAN_definition.h
#_DEPS += can_Interface.h



# Add  all include directories
INCLUDES_DIRS += $(PROJECTBASE)/drivers/include $(PROJECTBASE)/include

#all:

__DIRECTORY_VARIABLES := PROJECTBASE APPDIR BINDIRBASE
__ALL_PROJECT_DIRECTORY := $(PROJECTBASE) $(APPDIR) $(BINDIRBASE) $(DIR_PLATFORM) $(DIR_NATIVE)
#__DIRECTORY_VARIABLES := RIOTBASE CCACHE_BASEDIR RIOTCPU RIOTBOARD RIOTPKG GITCACHE RIOTPROJECT APPDIR BINDIRBASE BINDIR

# Make all paths absolute.
override PROJECTBASE       := $(abspath $(PROJECTBASE))
#override CCACHE_BASEDIR := $(abspath $(CCACHE_BASEDIR))
#override RIOTCPU        := $(abspath $(RIOTCPU))
#override RIOTBOARD      := $(abspath $(RIOTBOARD))
#override RIOTPKG        := $(abspath $(RIOTPKG))
#override RIOTPROJECT    := $(abspath $(RIOTPROJECT))
#override GITCACHE       := $(abspath $(GITCACHE))
override APPDIR         := $(abspath $(APPDIR))
override BINDIRBASE     := $(abspath $(BINDIRBASE))
#override BINDIR         := $(abspath $(BINDIR))

# Ensure that all directories are set and don't contain spaces.
ifneq (, $(filter-out 1, $(foreach v,${__DIRECTORY_VARIABLES},$(words ${${v}}))))
  $(info Aborting compilation for your safety.)
  $(info Related variables = ${__DIRECTORY_VARIABLES})
  $(error Make sure no path override is empty or contains spaces!)
endif



#search where
INCLUDES += $(foreach dir, $(INCLUDES_DIRS),-I$(wildcard $(dir)))
#INCLUDES += -I$(PROJECTBASE)/drivers/include -I$(PROJECTBASE)/include
#INCLUDES += -I$(RIOTCPU)/$(CPU)/include
#INCLUDES += -I$(RIOTBOARD)/$(BOARD)/include

#only as a test
CC=g++
CFLAGS=$(INCLUDES)

ODIR=$(CURDIR)/obj
#LDIR =../lib

#LIBS=

#_DEPS = hellomake.h
#_DEPS =
_DEPS = native_can.h
DEPS := $(strip $(foreach incl_file, $(_DEPS), \
																					$(shell find $(APPDIR) -type f -name $(incl_file))))

#_OBJ = hellomake.o hellofunc.o
_OBJ = native_can.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#QUIET ?= 1

#ifeq ($(QUIET),1)
#  AD=@
#  MAKEFLAGS += --no-print-directory
#else
#  AD=
#endif

#file source used for cansend
#CFLAGS += -Wall
#CFLAGS += -g
#used to enable the use of enum in class, like in can_Interface
CFLAGS += -std=c++0x
#CFLAGS += `ncurses5-config --cflags`
#LDFLAGS += `ncurses5-config --libs`

SRC_CANSEND = cansend.c
DIRS_CANSEND := $(strip $(foreach src_file, $(SRC_CANSEND), \
															$(foreach dir, $(__ALL_PROJECT_DIRECTORY), \
																					$(filter %$(src_file), $(wildcard $(dir)/*.c)))))

#_OBJ_CANSEND = $(patsubst %.c,%.o,$(SRC_CANSEND))
#OBJ_CANSEND = $(patsubst %,$(ODIR)/%,$(_OBJ_CANSEND))

SRC_CANWATCH = canwatch.c
DIRS_CANWATCH:= $(strip $(foreach src_file, $(SRC_CANWATCH), \
															$(foreach dir, $(__ALL_PROJECT_DIRECTORY), \
																					$(filter %$(src_file), $(wildcard $(dir)/*.c)))))

#_OBJ_CANREAD = $(patsubst %.c,%.o,$(SRC_CANREAD))
#OBJ_CANREAD = $(patsubst %,$(ODIR)/%,$(_OBJ_CANREAD))

#$(ODIR)/%.o: %.c $(DEPS)
#		@echo "in OBJ"
#		$(CC) -c -o $@ $< $(CFLAGS)

#canwatch:
		#gcc $(DIRS_CANWATCH)  -o $@ $^ $(CFLAGS) $(LIBS)

cansend: compile
		$(CC) $(CFLAGS) $(OBJFILES) /home/debian/workspace/Can_to_ROS/platform/cansend.o -o $@  $(LDFLAGS) $(LIBS)

canwatch: compile
		$(CC) $(CFLAGS) $(OBJFILES) /home/debian/workspace/Can_to_ROS/platform/canwatch.o -o $@  $(LDFLAGS) $(LIBS)

native_test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

#Now it is Time to create the Rules
compile: $(_OBJFILES)

$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c %.h)
	$(CC) -c $< -o $@ $(CFLAGS)

#build:


.PHONY: clean

clean:
	rm -f $(_OBJFILES)
	rm -f canwatch

info-build:
	@echo "==== Info globale ===="
	@echo '__ALL_PROJECT_DIRECTORY: $(__ALL_PROJECT_DIRECTORY)'
	@echo 'CFLAGS: $(CFLAGS)'
	@echo 'LDFLAGS: $(LDFLAGS)'
	@echo 'INCLUDES_DIRS: $(INCLUDES_DIRS)'
	@echo 'LIBS: $(LIBS)'
	@echo 'DEPS: $(DEPS)'
	@echo 'OBJFILES: $(OBJFILES)'
	@echo "======================"



info-build-cansend: info-build
	 @echo "==== Info cansend ===="
	 @echo 'DIRS_CANSEND: $(DIRS_CANSEND)'
	 @echo "======================="

info-build-canwatch: info-build
	@echo "==== Info canread ===="
	@echo 'DIRS_CANWATCH: $(DIRS_CANWATCH)'
	@echo "======================="
