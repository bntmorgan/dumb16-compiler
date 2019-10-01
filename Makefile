# Copyright (C) 2017  Benoît Morgan
#
# This file is part of dumb16-compiler.
#
# dumb16-compiler is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# dumb16-compiler is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with dumb16-compiler.  If not, see <http://www.gnu.org/licenses/>.

################################## FUNCTIONS ###################################

define SRC_2_OBJ
  $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
  $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

################################## STARTING RULE ###############################

all: targets

################################## GLOBALS  ####################################

CC=gcc
CFLAGS=-Wall -g -Werror
YFLAGS=-v -d
LD=gcc
LEX=lex
YACC=yacc
TFTPY = ../tftpy

################################## INCLUDES ####################################

# Overriden in rules.mk
TARGETS :=
OBJECTS :=

dir	:= sources
include	$(dir)/rules.mk

################################## RULES #######################################

targets: $(TARGETS)

%.elf:
	@mkdir -p $(dir $@)
	@echo [LD] $@
	@$(LD) -o $@ $^ $(TARGET_LDFLAGS)

%.so:
	@mkdir -p $(dir $@)
	@echo [LD] $@
	@$(LD) -shared -o $@ $^ $(TARGET_LDFLAGS)

build/%.o: build/%.c
	@mkdir -p $(dir $@)
	@echo [CC] $@
	@$(CC) -o $@ -c $< $(CFLAGS) $(OBJ_CFLAGS)

build/%.c: sources/%.l
	@mkdir -p $(dir $@)
	@echo [LX] $@
	@$(LEX) -o $@ $<

build/%.c: sources/%.y
	@mkdir -p $(dir $@)
	@echo [YC] $@
	@$(YACC) $(YFLAGS) -o $@ $<

build/%.o: sources/%.c
	@mkdir -p $(dir $@)
	@echo [CC] $@
	@$(CC) -o $@ -c $< $(CFLAGS) $(OBJ_CFLAGS)

info:
	@echo TARGETS [$(TARGETS)]
	@echo OBJECTS [$(OBJECTS)]

clean:
	@echo [CLR] $(TARGETS)
	@echo [CLR] $(OBJECTS)
	@rm -fr $(dir $(TARGETS)) $(OBJECTS) sample/*.bin

%.bin: %.s
	./binary/d16_assembler/d16_assembler.elf $^ -o $@
	./binary/d16_disassembler/d16_disassembler.elf $@

../dumb16/sources/dumb/rtl/rom.hex: sample/rom.bin
	./gen_hex.sh $^ $@

rom: ../dumb16/sources/dumb/rtl/rom.hex

mr-proper: mr-proper-vim clean

mr-proper-vim:
	@echo [CLR] *.swp
	@find . | grep .swp | xargs rm -f

binary/fibo.s: ./binary/d16_compiler/d16_compiler.elf sample/fibo.c
	$< sample/fibo.c -o $@

binary/fibo.bin: ./binary/d16_assembler/d16_assembler.elf binary/fibo.s
	$< binary/fibo.s -o $@

disass_fibo: ./binary/d16_disassembler/d16_disassembler.elf binary/fibo.bin
	$< binary/fibo.bin
