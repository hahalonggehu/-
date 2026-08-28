#---------------------------------------------------------------------------------
# 金庸群侠传水月碧影 v1.12 - Switch (devkitA64 + libnx) build
#---------------------------------------------------------------------------------
DEVKITPRO ?= /opt/devkitpro

export DEVKITPRO
export DEVKITARM

include $(DEVKITPRO)/libnx/switch_rules

TARGET   := jysdl
BUILD    := build

SOURCES  := src vendor/minizip vendor/minizip/compat minizip_config vendor/lua/src
INCLUDES := src minizip_config vendor/minizip vendor/minizip/compat vendor/minizip/extra vendor/lua/src

APP_TITLE   := JYSKY 1.12
APP_AUTHOR  := jysdl-switch
APP_VERSION := 1.12

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH := -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIC -ftls-model=local-exec

CFLAGS   := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) \
            -D__SWITCH__ \
            -DMZ_COMPAT_VERSION=111 -DHAVE_ZLIB=1 -DZLIB_COMPAT \
            -DHAVE_ICONV=0 -DMZ_BUILD -DMZ_ZIP_NO_ENCRYPTION -DMZ_ZIP_NO_CRYPTO \
            -I$(LIBNX)/include \
            -I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL2 \
            $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
            $(DEFINES)

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=c++11

ASFLAGS  := -g $(ARCH)

LDFLAGS  := -specs=$(DEVKITPRO)/libnx/switch.specs $(ARCH)

LIBS     := -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lSDL2 \
            -lfreetype -lharfbuzz -lbz2 \
            -lpng -ljpeg -lz -lEGL -lGLESv2 -lglapi -ldrm_nouveau \
            -lm -lnx

#---------------------------------------------------------------------------------
# list of directories containing libraries
#---------------------------------------------------------------------------------
LIBDIRS := $(PORTLIBS) $(LIBNX)

#---------------------------------------------------------------------------------
# rules
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT  := $(CURDIR)/$(TARGET)
export VPATH   := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES   := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
CPPFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
SFILES   := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.s))

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

export OFILES  := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(PORTLIBS)/include/SDL2 \
                   -I$(CURDIR)/$(BUILD)

$(info INCLUDE=$(INCLUDE))

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: clean all

all: $(OUTPUT).nro

$(OUTPUT).nro: $(OUTPUT).elf
	@$(PREFIX)elf2nro $< $@ $(NROFLAGS)
	@echo built ... $(notdir $@)

$(OUTPUT).elf: $(OFILES)
	@echo linking $(notdir $@)
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
	@echo built ... $(notdir $@)

$(DEPSDIR):
	@mkdir -p $(DEPSDIR)

%.o: %.c | $(DEPSDIR)
	@echo C $<
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$(notdir $*).d $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp | $(DEPSDIR)
	@echo C++ $<
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$(notdir $*).d $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s | $(DEPSDIR)
	@echo ASM $<
	@$(AS) -MMD -MP -MF $(DEPSDIR)/$(notdir $*).d $(ASFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nro

else
.PHONY: all

DEPENDS := $(OFILES:.o=.d)

$(OFILES): $(DEPSDIR)

$(DEPSDIR):
	@mkdir -p $(DEPSDIR)

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s
	@echo $(notdir $<)
	@$(AS) -MMD -MP -MF $(DEPSDIR)/$*.d $(ASFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(DEPSDIR)

-include $(DEPENDS)

endif