#
# Makefile for testrun
#
WFLAGS = -std=c89 -Wall -Wstrict-prototypes -D_XOPEN_SOURCE=600 -pedantic
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer
LDFLAGS = -s
KICK = java -jar ../KickAss.jar

#CFLAGS = -g $(WFLAGS)
#LDFLAGS = -g

TEST_OBJS = ../../../src/6502emu.o ../../../src/exo_util.o ../../../src/log.o ../../../src/areatrace.o ../../../src/vec.o ../../../src/buf_io.o ../../../src/buf.o ../../../src/table.o ../../../src/perf.o

permutate2 = $(if $(strip $(2)), $(foreach res, $(call permutate2, $(firstword $(2)), $(wordlist 2, $(words $(2)), $(2))), $(join $(res), _) $(join $(res), $(1))), _ $(1))

permutate = $(if $(strip $(1)), $(call permutate2, $(firstword $(1)), $(wordlist 2, $(words $(1)), $(1))))

VARIANTS = $(call permutate, i M c 4 o f)

VARIANT_OPT = c^-define%LITERAL_SEQUENCES_NOT_USED^-c M^-define%MAX_SEQUENCE_LENGTH_256^-M256 i^-define%INLINE_GET_BITS^ o^-define%DONT_REUSE_OFFSET^-P-32 4^-define%EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE^-P+16 f^-define%DECRUNCH_FORWARDS^-f

echo = $(or $(info $(1) $(2)), $(2))

variant = $(suffix $(basename $(1)))

asmopt = -define%ENABLE_SPLIT_ENCODING $(filter-out , $(foreach opt, $(VARIANT_OPT), $(if $(findstring $(word 1, $(subst ^, , $(opt))), $(1)), $(word 2, $(subst ^, , $(opt))))))

exoopt = $(filter-out , $(foreach opt, $(VARIANT_OPT), $(if $(findstring $(word 1, $(subst ^, , $(opt))), $(1)), $(word 3, $(subst ^, , $(opt))))))

.PHONY: assert.data%
.PRECIOUS: %.os %.exo %.prg %.out $(TEST_OBJS)
.SECONDEXPANSION:

build: ../../testrun $$(foreach variant, $$(VARIANTS), test.$$(variant).prg data.$$(variant).prg)
	@$(RM) *.out
	@../../testrun $(foreach variant, $(VARIANTS), test.$(variant).prg data.$(variant).prg)
	@$(MAKE) $(foreach variant, $(VARIANTS), assert.data.$(variant).prg)

assert.data%:
	cmp -i0:2 data$(call variant, $@).prg.out ../../data.bin
	@$(RM) data$(call variant, $@).prg.out

testrun.test%.prg: ../../testrun test%.prg data%.prg
	@../../testrun test$(call variant, $@).prg data$(call variant, $@).prg
	@cmp -i0:2 data$(call variant, $@).prg.out ../../data.bin && $(RM) data$(call variant, $@).prg.out

../../testrun: ../../testrun.o $(TEST_OBJS)
	@$(CC) $(LDFLAGS) -o $@ ../../testrun.o $(TEST_OBJS)

test%.prg: main.asm ../exodecrunch.asm
	@echo "building $@"
	@$(KICK) -o $@ $(subst %, ,$(call asmopt, $(call variant, $@))) main.asm

clean:
	@$(RM) testrun.o *.prg *.out *.os *.exo* testrun

data%.prg: data%.asm
	@$(KICK) -o $@ $(subst %, ,$(call asmopt, $(call variant, $@))) $<

%.exo %.exo.00 %.exo.01: ../../$$(basename $$(basename $$@)).bin
	@../../../src/exomizer mem $(call exoopt, $(call variant, $@)) -q -E $<,0x3000,0,0xb00 $<,0x3b00,0xb00 -o $*.exo

data%.asm: data.asm.template data%.exo data%.exo.00 data%.exo.01
	@sed 's/PLACEHOLDER/data$*.exo/' $< > $@

%.o:	%.c
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

# cancel built in rule that disturb things
%.out: %
