#################################################################
# Main makefile
# Type "make help" for usage
#
# To compile (optimized) and install static and shared library
# with each avaible compilers, type:
# for c in pgi intel gcc 'sun studio12'; do make c $c optimized static shared install; done
#################################################################

# Pseudo-Random Number Generator library (PRNG)

# Project options
LIB             := prng
BIN              = $(LIB)_test
SRCDIRS          = src
SRCEXT           = cpp
HEADEXT          = hpp
HEADERS          = $(wildcard $(addsuffix *.$(HEADEXT),$(addsuffix /, $(SRCDIRS)) ) )
LANGUAGE         = CPP

# Include the generic rules
include makefiles/Makefile.rules

#################################################################
# Project specific options

LIB_OBJ          = $(OBJ)

### Select which random number generator. #####################
### If RAND_DSFMT is not defined, use C/C++ standard (crappy) rand()
# Use dSFMT's (http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/)
CFLAGS      += -DRAND_DSFMT
## dSFMT flags
# Default period: (2^DSFMT_MEXP) - 1
CFLAGS      += -DDSFMT_MEXP=19937
#CFLAGS      += -DHAVE_SSE2
###############################################################

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


### Just build "full" and install
.PHONY: f
f:
	$(MAKE) shared static install $(filter-out f, $(MAKECMDGOALS) )

.PHONY: shared lib_shared
shared: lib_shared
lib_shared: $(build_dir)/lib$(LIB).so
$(build_dir)/lib$(LIB).so: $(LIB_OBJ)
	############################################################
	######## Building shared library... ########################
	#
	$(LD) $(LIB_OPTIONS) -o $(build_dir)/lib$(LIB).so $(LIB_OBJ)
	######## Done ##############################################

.PHONY: static lib_static
static: lib_static
lib_static: $(build_dir)/lib$(LIB).a
$(build_dir)/lib$(LIB).a: $(LIB_OBJ)
	############################################################
	######## Building static library... ########################
	#
	ar rc $(build_dir)/lib$(LIB).a $(LIB_OBJ)
	######## Done ##############################################
#   ranlib $(build_dir)/lib$(LIB).a

### Install #####################################################
INSTALL          = $(GNU)install -m644 -D
INSTALL_EXEC     = $(GNU)install -m755 -D

DEFAULT_DESTDIR := $(HOME)/usr
ifeq ($(DESTDIR),)
DESTDIR         := $(DEFAULT_DESTDIR)
endif

### If installing to "/home/...", we don't need sudo
ifneq ($(findstring $(HOME),$(DESTDIR) ),)
    SUDO        :=
else
    SUDO        := sudo
endif

# Find which cluster we run on
# For this, we iterate through the machines list: for each one of the list,
# we check if it is part of the $(host). If any match is found, then the
# corresponding possible_host_*_match is not empty.
possible_host_HPCVL_match    := $(strip $(foreach possible_host, $(HPCVL_MACHINES),    $(findstring $(possible_host), $(host) ) ) )
# Do specific stuff for different clusters
ifneq ($(possible_host_HPCVL_match), )
INSTALL := /usr/ucb/$(subst -D,,$(INSTALL) )
INSTALL_EXEC := /usr/ucb/$(subst -D,,$(INSTALL_EXEC) )
endif

### Add compiler to library directory, but only if not GCC
DESTDIR_INC      = $(DESTDIR)/include
ifeq ($(DESTDIRCOMPILER),gcc)
DESTDIR_LIB      = $(DESTDIR)/lib
else
    DESTDIR_LIB := $(DESTDIR)/lib/$(DESTDIRCOMPILER)
endif

HEADERS_NOTESTING=$(filter-out $(wildcard testing/*.$(HEADEXT)), $(HEADERS) )
HEADERS_NOTESTING_NOSRC=$(subst src/,,$(HEADERS_NOTESTING) )
HEADERS_NOTESTING_NOSRC_NOMEM=$(filter-out Memory.hpp, $(HEADERS_NOTESTING_NOSRC))
INSTALLED_HEADERS=$(addprefix $(DESTDIR)/include/, $(HEADERS_NOTESTING_NOSRC_NOMEM))
###############################################################


### Install only the build (static,shared) stated as target
TO_INSTALL       = install_headers
ifeq ($(filter shared, $(MAKECMDGOALS)), shared)
TO_INSTALL      += install_shared
endif
ifeq ($(filter static, $(MAKECMDGOALS)), static)
TO_INSTALL      += install_static
endif
.PHONY: install
install: $(TO_INSTALL)

.PHONY: install_static
install_static: $(DESTDIR_LIB)/lib$(LIB).a install_create_folders
$(DESTDIR_LIB)/lib$(LIB).a: $(build_dir)/lib$(LIB).a
	############################################################
	######## Installing static library to $(DESTDIR_LIB)... ####
	$(SUDO) $(INSTALL) $< $@
	############################################################
	######## Done ##############################################

.PHONY: install_shared
install_shared: $(DESTDIR_LIB)/lib$(LIB).so install_create_folders
$(DESTDIR_LIB)/lib$(LIB).so: $(build_dir)/lib$(LIB).so
	############################################################
	######## Installing shared library to $(DESTDIR_LIB)... ####
	$(SUDO) $(INSTALL_EXEC) $< $@
	############################################################
	######## Done ##############################################

.PHONY: install_headers install_headers_print install_headers_print_done
ifeq ($(wildcard $(INSTALLED_HEADERS)),)
install_headers_print: install_create_folders
	############################################################
	######## Installing library headers to $(DESTDIR_INC)... ###
install_headers_print_done:
	############################################################
	######## Done ##############################################
else
install_headers_print:
install_headers_print_done:
endif
install_headers: install_headers_print $(INSTALLED_HEADERS) install_headers_print_done
$(DESTDIR)/include/%.$(HEADEXT): src/%.$(HEADEXT)
	$(SUDO) $(INSTALL) $< $@

install_create_folders:
ifneq (,$(filter $(host), $(HPCVL_MACHINES) ))
	$(SUDO) mkdir -p $(DESTDIR_LIB)
endif

test:
	# INSTALLED_HEADERS = $(INSTALLED_HEADERS)
	# HEADERS_NOTESTING_NOSRC = $(HEADERS_NOTESTING_NOSRC)
	# (HEADERS) = $(HEADERS)
	# INSTALLED_HEADERS= $(INSTALLED_HEADERS)

.PHONY: uninstall
uninstall: force
	############################################################
	######## Removing library from $(DESTDIR_LIB)... ###
	$(SUDO) $(RM) $(DESTDIR_LIB)/lib$(LIB)*
	######## Removing library headers from $(DESTDIR_INC)... ###
	$(SUDO) $(RM) -r $(INSTALLED_HEADERS)

############ End of file ########################################
