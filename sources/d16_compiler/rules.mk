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

sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/d16_compiler.elf)
TARGETS 				+= $(TARGET)
OBJS_$(d)				:= $(call SRC_2_OBJ, $(d)/main.o $(d)/lexer.o $(d)/parser.o \
	$(d)/sym.o $(d)/opt.o $(d)/sem.o)

# Extra dependencies
$(call SRC_2_OBJ, $(d)/main.o): $(call SRC_2_OBJ, $(d)/lexer.o)
$(call SRC_2_OBJ, $(d)/lexer.o): $(call SRC_2_OBJ, $(d)/parser.o)

OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  OBJ_CFLAGS	:= -I$(d) -I$(call SRC_2_OBJ, $(d)) \
	-Isources/includes

$(TARGET)				:  TARGET_LDFLAGS	:= -ly -lfl
$(TARGET)				:  $(OBJS_$(d)) binary/d16_lib/d16_lib.so

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
