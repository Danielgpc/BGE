# Game Makefile

# Detect OS
UNAME := $(shell uname)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -D_THREAD_SAFE \
	-I../engine/src \
	-I../third_party/imgui \
	-I../third_party/glfw/include \
	-I../third_party/bgfx/include \
	-I../third_party/bx/include \
	-I../third_party/bimg/include

# bgfx arch (libs built by third_party/Makefile)
ARCH := $(shell uname -m)
ifeq ($(ARCH),arm64)
BGFX_ARCH := osx-arm64
else
BGFX_ARCH := osx-x64
endif
BGFX_BINDIR = ../third_party/bgfx/.build/$(BGFX_ARCH)/bin

LDFLAGS = \
	../build/engine/libengine.a \
	../build/third_party/libimgui.a \
	../build/third_party/libglfw.a \
	$(BGFX_BINDIR)/libbgfxRelease.a \
	$(BGFX_BINDIR)/libbimgRelease.a \
	$(BGFX_BINDIR)/libbxRelease.a \
	-framework Cocoa \
	-framework IOKit \
	-framework CoreFoundation \
	-framework CoreVideo \
	-framework CoreMedia \
	-framework VideoToolbox \
	-framework Metal \
	-framework QuartzCore \
	-framework Foundation \
	-framework OpenGL

# Directory for object files and executable
OBJDIR = ../build/game

# Source files
SRCS = src/main.cc

# Object files
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cc=.o)))

# Executable name
TARGET = $(OBJDIR)/bgeTestGame

all: $(TARGET)

$(TARGET): $(OBJS) ../build/engine/libengine.a ../build/third_party/libimgui.a ../build/third_party/libglfw.a $(BGFX_BINDIR)/libbgfxRelease.a $(BGFX_BINDIR)/libbimgRelease.a $(BGFX_BINDIR)/libbxRelease.a
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run
