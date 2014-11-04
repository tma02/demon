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
#ifndef DEMON_EMULATE_EMULATE_H_
#define DEMON_EMULATE_EMULATE_H_
#define REG_A 0
#define REG_B 1
#define REG_C 2
#define REG_X 3
#define REG_Y 4
#define REG_Z 5
#define REG_I 6
#define REG_J 7

typedef uint16_t word;
typedef uint32_t dword;
typedef int16_t sword;
typedef uint8_t byte;
typedef int8_t sbyte;

typedef struct DCPU {
	word reg[8];
	word pc;
	word sp;
	word ex;
	word ia;
	word memory[0x10000];
	word iq[0x100];
	byte iql;
	bool iaq;
	word hwx;
} DCPU;

int dcpu_cyc(DCPU *dcpu);
word* get_opr_a(DCPU *dcpu, sbyte *value);
word* get_opr_b(DCPU *dcpu, sbyte *value);
void dcpu_do_inst(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b);
void basic_op(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b);
void spec_op(DCPU *dcpu, word opcode, word *opr_a);
void queue_interrupt(DCPU *dcpu, word msg);
void interrupt(DCPU *dcpu);
void trigger_interrupt(DCPU *dcpu, word msg);

#endif
