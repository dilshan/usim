SUBDIRS = hexconv usim
FILE = Makefile

.PHONY: $(SUBDIRS) 

all:$(SUBDIRS)

clean:$(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -f $(FILE) -C $@ $(MAKECMDGOALS)