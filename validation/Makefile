#################################################################
# Main makefile
# Please edit this makefile to adapt to your project.
# Type "make help" for usage
#################################################################

# To run with a maximum of 500 MiB:
# softlimit -a 500000000 ./mdgit
# softlimit is part of http://cr.yp.to/daemontools.html

# To check memory usage:
# while [ 1 ]; do pmap -d `pidof project_name` | grep writeable | sed "s|K||g" | awk '{print ""$4" KiB    "$4/1024" MiB    "$4/1024/1024" GiB"}' ; sleep 0.1 ;done 2> /dev/null

# Project options
# Intel ICC sets LIB. Reset it here to make sure it's empty. Else code won't compile.
LIB             :=
BIN              = prng_validation
SRCDIRS          = src
TESTDIRS         = unit_testing
SRCEXT           = cpp
HEADEXT          = hpp
# LANGUAGE         = C
LANGUAGE         = CPP

# Include the generic rules
include makefiles/Makefile.rules

#################################################################
# Project specific options
CFLAGS          +=
LDFLAGS         +=

################################################################
### StdCout library default location (home directory)
LibName         := stdcout
LibIncludes     := $(HOME)/usr/include
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := $(HOME)/usr/lib
else
LibLocation     := $(HOME)/usr/lib/$(DESTDIRCOMPILER)
endif

### Check if the location exist. If not, try the /usr directoy
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
LibIncludes     := /usr/include/$(LibName)
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := /usr/lib
else
LibLocation     := /usr/lib/$(DESTDIRCOMPILER)
endif
endif

### If library is not found, bail out!
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
$(error ERROR: $(LibName) could not be found in "$(LibLocation)"! Please install it from ssh://optimusprime.selfip.net/git/nicolas/$(LibName).git)
endif

### Add library flags
CFLAGS          += -I$(LibIncludes)
ifeq ($(LINK_PREFERED),shared)
LDFLAGS         += -L$(LibLocation) -l$(LibName) $(RPATH)$(LibLocation)
else # static
LDFLAGS         += $(LibLocation)/lib$(LibName).a
endif
################################################################

################################################################
### StdCout library default location (home directory)
LibName         := prng
LibIncludes     := $(HOME)/usr/include
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := $(HOME)/usr/lib
else
LibLocation     := $(HOME)/usr/lib/$(DESTDIRCOMPILER)
endif

### Check if the location exist. If not, try the /usr directoy
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
LibIncludes     := /usr/include
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := /usr/lib
else
LibLocation     := /usr/lib/$(DESTDIRCOMPILER)
endif
endif

### If library is not found, bail out!
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
$(error ERROR: $(LibName) could not be found in "$(LibLocation)"! Please install it from ssh://optimusprime.selfip.net/git/nicolas/$(LibName).git)
endif

# The following lines are needed else gnu make will many times the last library
PRNG_Lib_CFLAGS      := -I$(LibIncludes)
ifeq ($(LINK_PREFERED),shared)
PRNG_Lib_LDFLAGS     := -L$(LibLocation) -l$(LibName) $(RPATH)$(LibLocation)
else # static
PRNG_Lib_LDFLAGS     := $(LibLocation)/lib$(LibName).a
endif

### Add library flags
CFLAGS          += $(PRNG_Lib_CFLAGS)
LDFLAGS         += $(PRNG_Lib_LDFLAGS)
################################################################

################################################################
### Memory library default location (home directory)
LibName         := memory
LibIncludes     := $(HOME)/usr/include
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := $(HOME)/usr/lib
else
LibLocation     := $(HOME)/usr/lib/$(DESTDIRCOMPILER)
endif

### Check if the location exist. If not, try the /usr directoy
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
LibIncludes     := /usr/include/$(LibName)
ifeq ($(DESTDIRCOMPILER),gcc)
LibLocation     := /usr/lib
else
LibLocation     := /usr/lib/$(DESTDIRCOMPILER)
endif
endif

### If library is not found, bail out!
ifeq ($(wildcard $(LibLocation)/lib$(LibName).*),)
$(error ERROR: $(LibName) could not be found in "$(LibLocation)"! Please install it from ssh://optimusprime.selfip.net/git/nicolas/$(LibName).git)
endif

# The following lines are needed else gnu make will many times the last library
Memory_Lib_CFLAGS  := -I$(LibIncludes)
ifeq ($(LINK_PREFERED),shared)
Memory_Lib_LDFLAGS := -L$(LibLocation) -l$(LibName) $(RPATH)$(LibLocation)
else # static
Memory_Lib_LDFLAGS := $(LibLocation)/lib$(LibName).a
endif

### Add library flags
CFLAGS          += $(Memory_Lib_CFLAGS)
LDFLAGS         += $(Memory_Lib_LDFLAGS)
################################################################

CFLAGS          := $(sort $(CFLAGS) )
LDFLAGS         := $(sort $(LDFLAGS) )

# Include the library stuff
# include makefiles/Makefile.library

############ End of file ########################################
