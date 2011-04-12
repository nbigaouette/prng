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

.PHONY: validation
$(BIN): validation
validation: static
	$(MAKE) -C validation $(filter-out validation, $(MAKECMDGOALS) )
	ln -sf validation/$(LIB)_validation .

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

# Project is a library. Include the makefile for build and install.
include makefiles/Makefile.library

$(eval $(call Flags_template,stdcout,StdCout.hpp,ssh://optimusprime.selfip.net/git/nicolas/stdcout.git))

############ End of file ########################################
