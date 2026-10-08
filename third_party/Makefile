# Third-party libraries: imgui, glfw, bgfx/bimg/bx

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

CXX ?= g++
AR ?= ar
CXXFLAGS ?= -Wall -Wextra -O2

BUILD_DIR := ../build/third_party
OBJDIR := $(BUILD_DIR)/obj

# --- bgfx / genie configuration ---
ifeq ($(UNAME_S),Darwin)
GENIE_OS := darwin
ifeq ($(UNAME_M),arm64)
GENIE_GCC := osx-arm64
BGFX_CONFIG := release
else
GENIE_GCC := osx-x64
BGFX_CONFIG := release
endif
BGFX_PROJ := gmake-$(GENIE_GCC)
BGFX_BIN := bgfx/.build/$(GENIE_GCC)/bin
else
GENIE_OS := linux
GENIE_GCC := linux-gcc
BGFX_CONFIG := release64
BGFX_PROJ := gmake-linux-gcc
BGFX_BIN := bgfx/.build/linux64_gcc/bin
endif

BGFX_PROJ_DIR := bgfx/.build/projects/$(BGFX_PROJ)
BGFX_LIBS := $(BGFX_BIN)/libbgfxRelease.a \
	$(BGFX_BIN)/libbimgRelease.a \
	$(BGFX_BIN)/libbxRelease.a
SHADERC := $(BGFX_BIN)/shadercRelease
BGFX_STAMP := $(OBJDIR)/bgfx.stamp

# --- imgui ---
IMGUI_SRCS := imgui/imgui.cpp \
	imgui/imgui_demo.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_tables.cpp \
	imgui/imgui_widgets.cpp
IMGUI_OBJS := $(patsubst imgui/%.cpp,$(OBJDIR)/imgui/%.o,$(IMGUI_SRCS))
IMGUI_LIB := $(BUILD_DIR)/libimgui.a

# --- glfw ---
GLFW_BUILD := $(OBJDIR)/glfw-build
GLFW_LIB := $(BUILD_DIR)/libglfw.a
GLFW_BUILD_LIB := $(GLFW_BUILD)/src/libglfw3.a
GLFW_SRCS := $(wildcard glfw/src/*.c) $(wildcard glfw/include/GLFW/*.h)

.PHONY: all imgui glfw bgfx shaderc clean

all: imgui glfw bgfx shaderc

imgui: $(IMGUI_LIB)
glfw: $(GLFW_LIB)
bgfx: $(BGFX_LIBS)
shaderc: $(BGFX_STAMP)
	@test -f $(SHADERC)

$(OBJDIR)/imgui/%.o: imgui/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iimgui -c $< -o $@

$(IMGUI_LIB): $(IMGUI_OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(IMGUI_OBJS)

$(GLFW_LIB): $(GLFW_SRCS) glfw/CMakeLists.txt
	@mkdir -p $(dir $@)
	cmake -S glfw -B $(GLFW_BUILD) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=OFF \
		-DGLFW_BUILD_EXAMPLES=OFF \
		-DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_DOCS=OFF \
		-DGLFW_INSTALL=OFF
	cmake --build $(GLFW_BUILD)
	cp $(GLFW_BUILD_LIB) $@

$(BGFX_PROJ_DIR)/Makefile:
	cd bgfx && ../bx/tools/bin/$(GENIE_OS)/genie --with-tools --gcc=$(GENIE_GCC) gmake

$(BGFX_STAMP): $(BGFX_PROJ_DIR)/Makefile
	@mkdir -p $(dir $@)
	$(MAKE) -C $(BGFX_PROJ_DIR) bgfx bimg bx shaderc config=$(BGFX_CONFIG)
	@touch $@

$(BGFX_LIBS): $(BGFX_STAMP)
	@test -f $@

clean:
	rm -rf $(BUILD_DIR)
	-$(MAKE) -C bgfx clean
