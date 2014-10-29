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

#include <stdint.h>
#include <stdio.h>
#include "emulate.h"

int dcpu_cyc(DCPU *dcpu) {
	word inst = dcpu->memory[dcpu->pc++];
	uint8_t opcode = (inst & 0x1F);
	int8_t b = (inst & 0x3E0) >> 5;
	int8_t a = (inst & 0xFC00) >> 10;
	//pointy pointers
	word *opr_a = get_opr_a(dcpu, &a, opcode);
	word *opr_b = get_opr_b(dcpu, &b, opcode);
	printf("OP:%u A:%u B:%u\n", opcode, *opr_a, *opr_b);
	return 0;
}

word* get_opr_a(DCPU *dcpu, int8_t *value, uint8_t opcode) {
	switch (*value) {
		case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
			return &dcpu->reg[opcode];
		case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
			return &dcpu->memory[dcpu->reg[opcode & 0x07]];
		case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
			return &dcpu->memory[dcpu->reg[opcode & 0x0F] + dcpu->memory[dcpu->pc++]];
		case 0x18:
			return &dcpu->memory[dcpu->sp++];
		case 0x19:
			return &dcpu->memory[dcpu->sp];
		case 0x1a:
			return &dcpu->memory[dcpu->sp + dcpu->memory[dcpu->pc++]];
		case 0x1b:
			return &dcpu->sp;
		case 0x1c:
			return &dcpu->pc;
		case 0x1d:
			return &dcpu->ex;
		case 0x1e:
			return &dcpu->memory[dcpu->memory[dcpu->pc++]];
		case 0x1f:
			return &dcpu->memory[dcpu->pc++];
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
		case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
		case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
		case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
			return &literal[*value & 0x1F];
		default:
			printf("ILLEGAL VALUE: %u", *value);
			return 0;
	}
}

word* get_opr_b(DCPU *dcpu, int8_t *value, uint8_t opcode) {
	switch (*value) {
		case 0x18:
			return &dcpu->memory[--dcpu->sp];
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
		case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
		case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
		case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
			printf("ILLEGAL VALUE: %u", *value);
			return 0;
		default:
			return get_opr_a(dcpu, value, opcode);
	}
}

int main() {
	DCPU dcpu;
	for (;;) {
		dcpu_cyc(&dcpu);
	}
	return 0;
}
