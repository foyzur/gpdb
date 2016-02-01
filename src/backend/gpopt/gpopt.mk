##-------------------------------------------------------------------------------------
##
## Copyright (C) 2012 EMC Corp.
##
## @doc: GP optimizer build 
##
## @author: solimm1
##
##-------------------------------------------------------------------------------------

UNAME = $(shell uname)
UNAME_M = $(shell uname -m)

# Use canonical name for 64-bit x86.
ifeq (amd64, $(UNAME_M))
	UNAME_M = x86_64
endif

ARCH_OS = GPOS_$(UNAME)
ARCH_CPU = GPOS_$(UNAME_M)

ifeq "$(BLD_TYPE)" "opt"
	GPOPT_flags = -O3 -fno-omit-frame-pointer -g3
else
	GPOPT_flags = -g3 -DGPOS_DEBUG
endif

# i386/i686 is 32-bit. Assume other CPU architectures are 64-bit.
ifeq (i386, $(UNAME_M))
	ARCH_BIT = GPOS_32BIT
else ifeq (i686, $(UNAME_M))
	ARCH_BIT = GPOS_32BIT
else
	ARCH_BIT = GPOS_64BIT
endif

BLD_FLAGS = -D$(ARCH_BIT) -D$(ARCH_CPU) -D$(ARCH_OS) $(GPOPT_flags)
override CPPFLAGS := -fPIC $(CPPFLAGS)
override CPPFLAGS := $(BLD_FLAGS)  $(CPPFLAGS)
override CPPFLAGS := -DGPOS_VERSION=\"$(LIBGPOS_VER)\" $(CPPFLAGS)
override CPPFLAGS := -DGPOPT_VERSION=\"$(OPTIMIZER_VER)\" $(CPPFLAGS)
override CPPFLAGS := -DXERCES_VERSION=\"$(XERCES_VER)\" $(CPPFLAGS)
override CPPFLAGS := -I $(XERCES)/include $(CPPFLAGS)
override CPPFLAGS := -I $(LIBGPOS)/include $(CPPFLAGS)
override CPPFLAGS := -I $(OPTIMIZER)/libgpopt/include $(CPPFLAGS)
override CPPFLAGS := -I $(OPTIMIZER)/libnaucrates/include $(CPPFLAGS)
override CPPFLAGS := -I $(OPTIMIZER)/libgpdbcost/include $(CPPFLAGS)
