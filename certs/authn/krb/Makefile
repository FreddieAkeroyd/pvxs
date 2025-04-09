# This is a Makefile fragment, see cert/authn/Makefile.

SRC_DIRS += $(AUTHN)/krb

PROD += authnkrb
authnkrb_SRCS += authnkrb.cpp
authnkrb_SRCS += authnkrbmain.cpp
authnkrb_SRCS += configkrb.cpp
authnkrb_SRCS += certstatusfactory.cpp
authnkrb_SRCS += certstatus.cpp

pvacms_SRCS += authnkrb.cpp
pvacms_SRCS += configkrb.cpp

# Detect the operating system
ifdef WIN32
UNAME_S := Win
else
UNAME_S := $(shell uname -s)
endif

# Set platform-specific krb libs
ifeq ($(UNAME_S), Darwin)
    # macOS
	authnkrb_LDFLAGS += -framework GSS
	pvacms_LDFLAGS += -framework GSS
else
    # MIT Kerberos
	authnkrb_LIBS += gssapi64 krb5_64 
	pvacms_SYS_LIBS += gssapi64 krb5_64
endif
