# Main Makefile for VOID_SCAN

# Targets
.PHONY: all third_party engine shaders game run clean

all: third_party engine shaders game

third_party:
	$(MAKE) -C third_party

engine: third_party
	$(MAKE) -C engine

shaders: third_party
	$(MAKE) -C shaders

game: engine third_party
	$(MAKE) -C game

run: game shaders
	$(MAKE) -C game run

clean:
	# $(MAKE) -C third_party clean
	$(MAKE) -C engine clean
	$(MAKE) -C game clean
	$(MAKE) -C shaders clean
