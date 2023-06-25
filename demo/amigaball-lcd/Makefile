# Test makefile
TARGETS := $(wildcard *.make)

all:
	@echo Nothing to do

%:
	for t in $(TARGETS); do \
	 $(MAKE) -f $$t $@; \
	done
