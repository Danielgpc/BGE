# Game Makefile

# Detect OS
UNAME := $(shell uname)

# Build type: release (default, optimized) or debug (ASan + UBSan)
BUILD ?= release

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -D_THREAD_SAFE \
	-I../engine/src \
	-I../third_party/imgui \
	-I../third_party/glfw/include \
	-I../third_party/bgfx/include \
	-I../third_party/bx/include \
	-I../third_party/bimg/include

# Separate object/lib dirs per build type; debug gets sanitizer link flags
# and runtime options
SAN_FLAGS =
RUN_ENV =
# NOTE: plain -DDEBUG would break simple-cpp-logger (it has an enum member
# Logger::LogLevel::DEBUG), so debug builds define BGE_DEBUG instead.
ifeq ($(BUILD),debug)
CXXFLAGS += -DBGE_DEBUG -g -O0 -fno-omit-frame-pointer \
	-fsanitize=address,undefined -fno-sanitize-recover=all
SAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
RUN_ENV = ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1
OBJDIR = ../build/game-debug
ENGINE_LIB = ../build/engine-debug/libengine.a
else
CXXFLAGS += -DNDEBUG -O2
OBJDIR = ../build/game
ENGINE_LIB = ../build/engine/libengine.a
endif

# bgfx arch (libs built by third_party/Makefile)
ARCH := $(shell uname -m)
ifeq ($(ARCH),arm64)
BGFX_ARCH := osx-arm64
else
BGFX_ARCH := osx-x64
endif
BGFX_BINDIR = ../third_party/bgfx/.build/$(BGFX_ARCH)/bin

LDFLAGS = \
	$(ENGINE_LIB) \
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

# Source files
SRCS = src/main.cc

# Object files
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cc=.o)))

# Executable name
TARGET = $(OBJDIR)/bgeTestGame

all: $(TARGET)

$(TARGET): $(OBJS) $(ENGINE_LIB) ../build/third_party/libimgui.a ../build/third_party/libglfw.a $(BGFX_BINDIR)/libbgfxRelease.a $(BGFX_BINDIR)/libbimgRelease.a $(BGFX_BINDIR)/libbxRelease.a
	$(CXX) $(OBJS) -o $(TARGET) $(SAN_FLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	$(RUN_ENV) ./$(TARGET)

clean:
	rm -rf ../build/game ../build/game-debug ../build/game-asan

.PHONY: all clean run
