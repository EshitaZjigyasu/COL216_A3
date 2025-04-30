all:
	g++ main.cpp globals.cpp bus.cpp cache.cpp core.cpp processor.cpp -o L1simulate

EXEC = ./L1simulate

run: all
	$(EXEC) $(ARGS)