DIRS=$(filter-out Makefile assignment, $(wildcard *))

all clean install:
	for dir in $(DIRS); do $(MAKE) $@ -C $$dir; done

