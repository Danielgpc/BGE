# Engine Makefile

# Detect OS
UNAME := $(shell uname)

# Build type: release (default, optimized) or debug (ASan + UBSan)
BUILD ?= release

# Platform-specific shared library settings
ifeq ($(UNAME),Darwin)
  SHARED_LIB_EXT = dylib
  SHARED_LIB_FLAG = -dynamiclib
  SONAME_FLAG = -install_name @rpath/libengine.$(SHARED_LIB_EXT)
  RPATH_FLAG = -Wl,-rpath,@loader_path
  # bgfx arch
  ARCH := $(shell uname -m)
  ifeq ($(ARCH),arm64)
    BGFX_ARCH := osx-arm64
  else
    BGFX_ARCH := osx-x64
  endif
  BGFX_BINDIR = ../third_party/bgfx/.build/$(BGFX_ARCH)/bin
  ENGINE_LDFLAGS = \
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
else ifeq ($(UNAME),Linux)
  SHARED_LIB_EXT = so
  SHARED_LIB_FLAG = -shared
  SONAME_FLAG = -Wl,-soname,libengine.$(SHARED_LIB_EXT)
  RPATH_FLAG = -Wl,-rpath,'$$ORIGIN'
  BGFX_BINDIR = ../third_party/bgfx/.build/linux64_gcc/bin
  ENGINE_LDFLAGS = -lGL -lpthread -ldl
else ifeq ($(UNAME),Windows_NT)
  SHARED_LIB_EXT = dll
  SHARED_LIB_FLAG = -shared
  SONAME_FLAG =
  RPATH_FLAG =
  BGFX_BINDIR = ../third_party/bgfx/.build/win64_vs2017/bin
  ENGINE_LDFLAGS = -lopengl32 -lgdi32
else
  SHARED_LIB_EXT = so
  SHARED_LIB_FLAG = -shared
  SONAME_FLAG =
  RPATH_FLAG =
  ENGINE_LDFLAGS =
endif

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -D_THREAD_SAFE -fPIC \
	-DBGE_SHADER_DIR=\"$(abspath ../build/shaders)\" \
	-I../third_party/imgui \
	-I../third_party/glfw/include \
	-I../third_party/bgfx/include \
	-I../third_party/bx/include \
	-I../third_party/bimg/include \
	-I../third_party/glm \
	-I../third_party/simple-cpp-logger/include \
	-I../third_party/stb \
	-DBGE_ENGINE_EXPORTS

# Separate object dirs per build type so objects never mix
# NOTE: plain -DDEBUG would break simple-cpp-logger (it has an enum member
# Logger::LogLevel::DEBUG), so debug builds define BGE_DEBUG instead.
ifeq ($(BUILD),debug)
CXXFLAGS += -DBGE_DEBUG -g -O0 -fno-omit-frame-pointer \
	-fsanitize=address,undefined -fno-sanitize-recover=all
SAN_LDFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
OBJDIR = ../build/engine-debug
else
CXXFLAGS += -DNDEBUG -O2
SAN_LDFLAGS =
OBJDIR = ../build/engine
endif

# Source files
SRCS = src/bge.cc src/shaders.cc src/image.cc

# Platform-specific source file
ifeq ($(UNAME),Darwin)
  PLATFORM_SRC = src/platform_mac.cc
else ifeq ($(UNAME),Linux)
  PLATFORM_SRC = src/platform_linux.cc
else ifeq ($(UNAME),Windows_NT)
  PLATFORM_SRC = src/platform_win.cc
else
  PLATFORM_SRC = src/platform_linux.cc
endif

SRCS += $(PLATFORM_SRC)

# Object files
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cc=.o)))

# Library name
SHARED_LIB = $(OBJDIR)/libengine.$(SHARED_LIB_EXT)

# Third-party libraries
THIRD_PARTY_LIBS = \
	../build/third_party/libimgui.a \
	../build/third_party/libglfw.a \
	$(BGFX_BINDIR)/libbgfxRelease.a \
	$(BGFX_BINDIR)/libbimgRelease.a \
	$(BGFX_BINDIR)/libbxRelease.a

all: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS) $(THIRD_PARTY_LIBS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(SHARED_LIB_FLAG) $(SONAME_FLAG) -o $(SHARED_LIB) $(OBJS) $(THIRD_PARTY_LIBS) $(ENGINE_LDFLAGS) $(SAN_LDFLAGS) $(RPATH_FLAG)

$(OBJDIR)/%.o: src/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf ../build/engine ../build/engine-debug ../build/engine-asan

.PHONY: all clean
