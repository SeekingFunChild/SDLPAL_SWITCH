ifeq ($(strip $(DEVKITA64)),)
$(error "Please set DEVKITA64 in your environment. export DEVKITA64=<path to>devkitA64")
endif

include $(DEVKITA64)/base_tools

TARGET_LIB = libadplug.a

CFILES = $(foreach dir,$(CURDIR),$(wildcard $(dir)/*.c))
CPPFILES = $(foreach dir,$(CURDIR),$(wildcard $(dir)/*.cpp))
OBJS := $(patsubst %.c,%.o, $(CFILES)) $(patsubst %.cpp,%.o, $(CPPFILES))

LIBNX	:= $(DEVKITPRO)/libnx
INCLUDES = -I./include -I$(LIBNX)/include 

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-march=armv8-a -mtp=soft -fPIE
CFLAGS		:=	-g -Wall -O2 -ffast-math $(ARCH)
CFLAGS		+=	$(INCLUDES) -DARM11 -DSWITCH -D__SWITCH__
CXXFLAGS	:=  $(CFLAGS) -fno-rtti -std=gnu++11
ASFLAGS		:=	-g $(ARCH)
LDFLAGS		:=	-specs=switch.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

CFLAGS		+= -Wno-unused-variable

LIBS	:= -lnx

all: $(OBJS) $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	$(AR) -rc $@ $^

install: $(TARGET_LIB)
	@mkdir -p "$(DEVKITPRO)/portlibs/switch/lib"
	@cp  $(TARGET_LIB) "$(DEVKITPRO)/portlibs/switch/lib"
	@mkdir -p "$(DEVKITPRO)/portlibs/switch/include"
	@mkdir -p "$(DEVKITPRO)/portlibs/switch/include/adplug"
	@cp *.h "$(DEVKITPRO)/portlibs/switch/include/adplug"
	@echo "Installed!"

clean:
	@rm -rf $(OBJS) $(TARGET_LIB)