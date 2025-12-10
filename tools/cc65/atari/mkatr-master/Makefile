#
#  Copyright (C) 2016 Daniel Serpell
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program.  If not, see <http://www.gnu.org/licenses/>
#

# Folder to place compiled programs
PROG_DIR=.

# Folder to place the build files
BUILD_DIR=obj


PROGS=\
 mkatr\
 lsatr\

SOURCES_mkatr=\
 crc32.c\
 compat.c\
 darray.c\
 flist.c\
 mkatr.c\
 msg.c\
 spartafs.c\

SOURCES_lsatr=\
 atr.c\
 compat.c\
 crc32.c\
 lsatr.c\
 lssfs.c\
 lsdos.c\
 lsextra.c\
 lshowfen.c\
 msg.c\

CFLAGS=-O2 -Wall
LDFLAGS=

# Default rule
all: $(PROGS:%=$(PROG_DIR)/%)

# Rule template
define PROG_template
 # Objects from sources
 OBJS_$(1)=$(addprefix $(BUILD_DIR)/,$(SOURCES_$(1):%.c=%.o))
 # All SOURCES/OBJECTS
 SOURCES+=$$(SOURCES_$(1))
 OBJS+=$$(OBJS_$(1))
 # Link rule
$(PROG_DIR)/$(1): $$(OBJS_$(1))
	$$(CC) $$(CFLAGS) $$(LDFLAGS) $$^ $$(LDLIBS) -o $$@
endef

# Generate all rules
$(foreach prog,$(PROGS),$(eval $(call PROG_template,$(prog))))

DEPS=$(OBJS:%.o=%.d)

# Cleanup
.PHONY: clean
clean:
	-rm -f $(OBJS) $(DEPS)
	-rmdir $(BUILD_DIR)

.PHONY: distclean
distclean: clean
	-rm -f $(PROGS:%=$(PROG_DIR)/%)

# Create output dirs
$(BUILD_DIR):
	mkdir -p $@

$(OBJS): | $(BUILD_DIR)
$(DEPS): | $(BUILD_DIR)

# Compilation
$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# Dependencies
$(BUILD_DIR)/%.d: src/%.c
	@$(CC) -MM -MP -MF $@ -MT "$(@:.d=.o) $@" $(CFLAGS) $(CPPFLAGS) $<

ifneq "$(MAKECMDGOALS)" "clean"
 ifneq "$(MAKECMDGOALS)" "distclean"
  -include $(DEPS)
 endif
endif
