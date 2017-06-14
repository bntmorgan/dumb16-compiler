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

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

dir	:= $(d)/d16_lib
include	$(dir)/rules.mk
dir	:= $(d)/d16_compiler
include	$(dir)/rules.mk
dir	:= $(d)/d16_assembler
include	$(dir)/rules.mk
dir	:= $(d)/d16_disassembler
include	$(dir)/rules.mk

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))