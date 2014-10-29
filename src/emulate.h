/*
		demon
    DCPU-16 emulation suite
    Copyright (C) 2014  Tianyi Ma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef DEMON__EMULATE_H_
#define DEMON__EMULATE_H_
typedef uint16_t word;
typedef int16_t sword;
typedef uint32_t dword;
typedef uint8_t byte;
typedef int8_t sbyte;

typedef struct DCPU {
	word reg[8];
	word pc;
	word sp;
	word ex;
	word ia;
	word memory[0x10000];
} DCPU;

word literal[0x20] = {
	0xffff, 
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e
};

int dcpu_cyc(DCPU *dcpu);
word* get_opr_a(DCPU *dcpu, sbyte *value, byte opcode);
word* get_opr_b(DCPU *dcpu, sbyte *value, byte opcode);
int dcpu_do_inst(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b);
int basic_op(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b);
int spec_op(DCPU *dcpu, word opcode, word *opr_a);
#endif
