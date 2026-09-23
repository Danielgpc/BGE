# Main Makefile for VOID_SCAN

# Targets
.PHONY: all third_party engine shaders game debug release run run-debug asan clean help

# Bare `make` still builds (help is only via `make help`)
.DEFAULT_GOAL := all

# Build type: release (default, optimized, no sanitizers) or debug
# (unoptimized, ASan + UBSan always on).
BUILD ?= release

help:
	@echo "BGE - available targets:"
	@echo ""
	@echo "  make            Build everything (release, no sanitizers)"
	@echo "  make run        Build and run (release)"
	@echo ""
	@echo "  make debug      Build everything with sanitizers (ASan + UBSan)"
	@echo "  make run-debug  Build with sanitizers and run"
	@echo "  make asan       Alias for run-debug"
	@echo ""
	@echo "  make release    Alias for the default release build"
	@echo "  make clean      Remove all build output (keeps third_party)"
	@echo "  make help       Show this help"
	@echo ""
	@echo "Sub-targets: third_party, engine, shaders, game"
	@echo "Override build type anywhere with BUILD=debug or BUILD=release,"
	@echo "e.g. make BUILD=debug engine"

all: third_party engine shaders game

third_party:
	$(MAKE) -C third_party

engine: third_party
	$(MAKE) -C engine BUILD=$(BUILD)

shaders: third_party
	$(MAKE) -C shaders

game: engine third_party
	$(MAKE) -C game BUILD=$(BUILD)

run: game shaders
	$(MAKE) -C game run BUILD=$(BUILD)

# Build with sanitizers (equivalent to: make BUILD=debug)
debug: BUILD=debug
debug: all

# Optimized build without sanitizers (equivalent to: make BUILD=release)
release: BUILD=release
release: all

# Build debug and run with sanitizer runtime options
run-debug: BUILD=debug
run-debug: game shaders
	$(MAKE) -C game run BUILD=$(BUILD)

# Alias kept for convenience
asan: run-debug

clean:
	# $(MAKE) -C third_party clean
	$(MAKE) -C engine clean
	$(MAKE) -C game clean
	$(MAKE) -C shaders clean
