# Compiles shader sources into build/shaders/<api>/ for every rendering API
# bgfx can select on this platform (Metal, Vulkan/SPIR-V, OpenGL, OpenGL ES).

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Darwin)
ifeq ($(UNAME_M),arm64)
BGFX_ARCH := osx-arm64
else
BGFX_ARCH := osx-x64
endif
else
BGFX_ARCH := linux64_gcc
endif

SHADERC := ../third_party/bgfx/.build/$(BGFX_ARCH)/bin/shadercRelease
BGFX_INCLUDE := ../third_party/bgfx/src
OUT := ../build/shaders

VS_SRCS := $(notdir $(wildcard vs_*.sc))
FS_SRCS := $(notdir $(wildcard fs_*.sc))

VS_BINS := $(addprefix $(OUT)/metal/,$(VS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/spirv/,$(VS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/glsl/,$(VS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/essl/,$(VS_SRCS:.sc=.bin))

FS_BINS := $(addprefix $(OUT)/metal/,$(FS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/spirv/,$(FS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/glsl/,$(FS_SRCS:.sc=.bin)) \
	$(addprefix $(OUT)/essl/,$(FS_SRCS:.sc=.bin))

BINS := $(VS_BINS) $(FS_BINS)

all: $(BINS)

# Metal (macOS)
$(OUT)/metal/vs_%.bin: vs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/metal
	$(SHADERC) --platform osx -p metal -i $(BGFX_INCLUDE) --type vertex -o $@ -f $<

$(OUT)/metal/fs_%.bin: fs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/metal
	$(SHADERC) --platform osx -p metal -i $(BGFX_INCLUDE) --type fragment -o $@ -f $<

# SPIR-V (Vulkan)
$(OUT)/spirv/vs_%.bin: vs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/spirv
	$(SHADERC) --platform linux -p spirv -i $(BGFX_INCLUDE) --type vertex -o $@ -f $<

$(OUT)/spirv/fs_%.bin: fs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/spirv
	$(SHADERC) --platform linux -p spirv -i $(BGFX_INCLUDE) --type fragment -o $@ -f $<

# GLSL (desktop OpenGL)
$(OUT)/glsl/vs_%.bin: vs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/glsl
	$(SHADERC) --platform linux -p 430 -i $(BGFX_INCLUDE) --type vertex -o $@ -f $<

$(OUT)/glsl/fs_%.bin: fs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/glsl
	$(SHADERC) --platform linux -p 430 -i $(BGFX_INCLUDE) --type fragment -o $@ -f $<

# ESSL (OpenGL ES)
$(OUT)/essl/vs_%.bin: vs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/essl
	$(SHADERC) --platform android -p 300_es -i $(BGFX_INCLUDE) --type vertex -o $@ -f $<

$(OUT)/essl/fs_%.bin: fs_%.sc varying.def.sc $(SHADERC)
	@mkdir -p $(OUT)/essl
	$(SHADERC) --platform android -p 300_es -i $(BGFX_INCLUDE) --type fragment -o $@ -f $<

clean:
	rm -rf $(OUT)

.PHONY: all clean
