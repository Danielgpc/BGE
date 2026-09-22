# Engine Makefile

# Detect OS
UNAME := $(shell uname)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -D_THREAD_SAFE \
	-DBGE_SHADER_DIR=\"$(abspath ../build/shaders)\" \
	-I../third_party/imgui \
	-I../third_party/glfw/include \
	-I../third_party/bgfx/include \
	-I../third_party/bx/include \
	-I../third_party/bimg/include \
	-I../third_party/glm

# Directory for object files and library
OBJDIR = ../build/engine

# Source files
SRCS = src/bge.cc shaders.cc

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
	rm -f $(OBJS) $(LIB)

.PHONY: all clean
