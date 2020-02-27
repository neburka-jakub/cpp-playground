CXXFLAGS += -Wall -pedantic -ggdb -O0 -std=c++17 -I src -L .
VPATH += src
LINK.o = $(LINK.cc) # c->c++ for implicit %: %.o

.DEFAULT_GOAL = none
.PHONY: none clean $(patsubst %_test.cc,%_check,$(wildcard *_test.cc))

clean:
	@rm -f *.d
	@find . -maxdepth 1 -exec file {} ';' | grep ELF | cut -d':' -f 1 | xargs rm -fv

%.d: %.cc
	@$(COMPILE.cc) -MM $< -o $@

ifneq (clean,$(MAKECMDGOALS))
-include $(patsubst %.cc,%.d,$(notdir $(wildcard src/*.cc)))
endif

%.so: CXXFLAGS += -shared -fPIC
lib%.so: %.o
	$(LINK.cc) -o $@ $^

%_test: LDLIBS += $(shell pkg-config --libs gtest_main)
%_check: %_test
	@LD_LIBRARY_PATH=. ./$<

include Goals.mk

Makefile: ;
%.mk: ;
%.h: ;
%.cc: ;