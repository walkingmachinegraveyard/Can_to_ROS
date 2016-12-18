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

#what we use now for the projetct
#_DEPS = lowLevel_CAN_definition.h
#_DEPS += can_Interface.h



# Add  all include directories
INCLUDES_DIRS += $(PROJECTBASE)/drivers/include $(PROJECTBASE)/include

#all:

__DIRECTORY_VARIABLES := PROJECTBASE APPDIR BINDIRBASE
__ALL_PROJECT_DIRECTORY := $(PROJECTBASE) $(APPDIR) $(BINDIRBASE) $(DIR_PLATFORM)
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
CC=gcc
CFLAGS=$(INCLUDES)

ODIR=$(CURDIR)/obj
#LDIR =../lib

#LIBS=

#_DEPS = hellomake.h
#_DEPS =
DEPS = $(patsubst %,$(INCLUDES_DIRS)/%,$(_DEPS))


#DIRS_LTO_TEST := $(foreach dir, $(__DIRECTORY_VARIABLES),$(filter-out $(MY_TEST), $(wildcard $(dir)/*.c)))

#_OBJ = hellomake.o hellofunc.o
_OBJ =
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#QUIET ?= 1

#ifeq ($(QUIET),1)
#  AD=@
#  MAKEFLAGS += --no-print-directory
#else
#  AD=
#endif

#file source used for cansend
CFLAGS += -Wall -g
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

$(ODIR)/%.o: %.c $(DEPS)
		@echo "in OBJ"
		$(CC) -c -o $@ $< $(CFLAGS)

cansend:
		gcc $(DIRS_CANSEND)  -o $@ $^ $(CFLAGS) $(LIBS)

canwatch:
		gcc $(DIRS_CANWATCH)  -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

info-build:
	@echo "==== Info globale ===="
	@echo '__ALL_PROJECT_DIRECTORY: $(__ALL_PROJECT_DIRECTORY)'
	@echo 'CFLAGS: $(CFLAGS)'
	@echo 'LDFLAGS: $(LDFLAGS)'
	@echo 'INCLUDES_DIRS: $(INCLUDES_DIRS)'
	@echo 'LIBS: $(LIBS)'
	@echo "======================"

info-build-cansend: info-build
	 @echo "==== Info cansend ===="
	 @echo 'DIRS_CANSEND: $(DIRS_CANSEND)'
	 @echo "======================="

info-build-canwatch: info-build
	@echo "==== Info canread ===="
	@echo 'DIRS_CANWATCH: $(DIRS_CANWATCH)'
	@echo "======================="
