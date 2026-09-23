# Engine Makefile

# Detect OS
UNAME := $(shell uname)

# Build type: release (default, optimized) or debug (ASan + UBSan)
BUILD ?= release

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -D_THREAD_SAFE \
	-DBGE_SHADER_DIR=\"$(abspath ../build/shaders)\" \
	-I../third_party/imgui \
	-I../third_party/glfw/include \
	-I../third_party/bgfx/include \
	-I../third_party/bx/include \
	-I../third_party/bimg/include \
	-I../third_party/glm \
	-I../third_party/simple-cpp-logger/include \
	-I../third_party/stb

# Separate object dirs per build type so objects never mix
# NOTE: plain -DDEBUG would break simple-cpp-logger (it has an enum member
# Logger::LogLevel::DEBUG), so debug builds define BGE_DEBUG instead.
ifeq ($(BUILD),debug)
CXXFLAGS += -DBGE_DEBUG -g -O0 -fno-omit-frame-pointer \
	-fsanitize=address,undefined -fno-sanitize-recover=all
OBJDIR = ../build/engine-debug
else
CXXFLAGS += -DNDEBUG -O2
OBJDIR = ../build/engine
endif

# Source files
SRCS = src/bge.cc src/shaders.cc src/image.cc

# Object files
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cc=.o)))

# Library name
LIB = $(OBJDIR)/libengine.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(OBJDIR)/%.o: src/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf ../build/engine ../build/engine-debug ../build/engine-asan

.PHONY: all clean
