# Main Makefile for VOID_SCAN

# Targets
.PHONY: all third_party engine game run clean

all: third_party engine game

third_party:
	$(MAKE) -C third_party

engine: third_party
	$(MAKE) -C engine

game: engine third_party
	$(MAKE) -C game

run: game
	$(MAKE) -C game run

clean:
	$(MAKE) -C third_party clean
	$(MAKE) -C engine clean
	$(MAKE) -C game clean
