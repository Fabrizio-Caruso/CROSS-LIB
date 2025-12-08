#
# Makefile for testrun
#
WFLAGS = -std=c89 -Wall -Wstrict-prototypes -D_XOPEN_SOURCE=600 -pedantic
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer
LDFLAGS = -s

#CFLAGS = -g $(WFLAGS)
#LDFLAGS = -g

TEST_OBJS = ../src/6502emu.o ../src/exo_util.o ../src/log.o ../src/areatrace.o ../src/vec.o ../src/buf_io.o ../src/buf.o ../src/table.o ../src/perf.o

SOURCES1 = main1.os exostreamdecr1.os
SOURCES2 = main2.os exostreamdecr2.os

NAME1 = test.1stream.prg
NAME2 = test.2stream.prg

permutate2 = $(if $(strip $(2)), $(foreach res, $(call permutate2, $(firstword $(2)), $(wordlist 2, $(words $(2)), $(2))), $(join $(res), _) $(join $(res), $(1))), _ $(1))

permutate = $(if $(strip $(1)), $(call permutate2, $(firstword $(1)), $(wordlist 2, $(words $(1)), $(1))))

VARIANTS = $(call permutate, i e M c 4 o f)

VARIANT_OPT = c^-DLITERAL_SEQUENCES_NOT_USED=1^-c M^-DMAX_SEQUENCE_LENGTH_256=1^-M256 i^-DINLINE_GET_BITS=1^ e^-DENABLE_SPLIT_ENCODING=1^ o^-DDONT_REUSE_OFFSET=1^-P-32 4^-DEXTRA_TABLE_ENTRY_FOR_LENGTH_THREE=1^-P+16 f^-DDECRUNCH_FORWARDS=1^-f

echo = $(or $(info $(1) $(2)), $(2))

variant = $(suffix $(basename $(1)))

asmopt = $(filter-out , $(foreach opt, $(VARIANT_OPT), $(if $(findstring $(word 1, $(subst ^, , $(opt))), $(1)), $(word 2, $(subst ^, , $(opt))))))

exoopt = $(filter-out , $(foreach opt, $(VARIANT_OPT), $(if $(findstring $(word 1, $(subst ^, , $(opt))), $(1)), $(word 3, $(subst ^, , $(opt))))))

.PHONY: assert.data%
.PRECIOUS: %.exo %stream.exo %.os test%.prg $(TEST_OBJS)
.SECONDEXPANSION:

build: testrun $$(foreach variant, $$(VARIANTS) 1stream 2stream, test.$$(variant).prg data.$$(variant).exo)
	@$(RM) *.out
	@./testrun $(foreach variant, $(VARIANTS), test.$(variant).prg data.$(variant).exo)
	@$(MAKE) $(foreach variant, $(VARIANTS), assert.data.$(variant).exo)
	@./testrun $(foreach variant, 1stream 2stream, test.$(variant).prg data.$(variant).exo@0x3000)
	@$(MAKE) $(foreach variant, 1stream 2stream, assert.data.$(variant).exo)

assert.data%:
	cmp -i0:2 data$(call variant, $@).exo.out data.bin
	@$(RM) data$(call variant, $@).exo.out

testrun.test%.prg: testrun test%.prg data%.exo
	@./testrun test$(call variant, $@).prg data$(call variant, $@).exo
	@cmp -i0:2 data$(call variant, $@).exo.out data.bin && $(RM) data$(call variant, $@).exo.out

testrun.test%stream.prg: testrun test%stream.prg data%stream.exo
	@./testrun test$(call variant, $@).prg data$(call variant, $@).exo@0x3000
	@cmp -i0:2 data$(call variant, $@).exo.out data.bin && $(RM) data$(call variant, $@).exo.out

testrun: testrun.o $(TEST_OBJS)
	@$(CC) $(LDFLAGS) -o $@ testrun.o $(TEST_OBJS)

test%.prg: main%.os exodecrunch%.os
	@echo "building $@"
	@ld65 main$(call variant, $@).os exodecrunch$(call variant, $@).os -o $@ -Cc64.cfg

$(NAME1): $(SOURCES1)
	@ld65 $(SOURCES1) -o $@ -Cc64.cfg

$(NAME2): $(SOURCES2)
	@ld65 $(SOURCES2) -o $@ -Cc64.cfg

%.os: $$(basename $$(basename $$@)).s
	@ca65 $(call asmopt, $(call variant, $@)) $< -o $@

clean:
	@$(RM) testrun.o test*.prg *.os *.exo *.exo.out testrun

%.exo: $$(basename $$(basename $$@)).bin
	@../src/exomizer mem $(call exoopt, $(call variant, $@)) -q $<,0x3000 -o $@

%stream.exo: $$(basename $$(basename $$@)).bin
	@../src/exomizer raw -P0 -b -c -m 1024 -q -C $<,2 -o $@

%.o:	%.c
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

# cancel built in rule that disturb things
%.out: %
