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
#include <string.h>
#include <stdbool.h>
#include "emulate.h"
#include "hardware.h"

word literal[0x20] = {
	0xffff, 
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e
};

int dcpu_cyc(DCPU *dcpu) {
	word inst = dcpu->memory[dcpu->pc++];
	byte opcode = (inst & 0x1f);
	sbyte b = (inst & 0x3e0) >> 5;
	sbyte a = (inst & 0xfc00) >> 10;
	//pointy pointers
	word *opr_a = get_opr_a(dcpu, &a, opcode);
	word *opr_b = get_opr_b(dcpu, &b, opcode);
	dcpu_do_inst(dcpu, opcode, opr_a, opr_b);
	printf("OP:%u A:%u B:%u\n", opcode, *opr_a, *opr_b);
	return 0;
}

word* get_opr_a(DCPU *dcpu, sbyte *value, byte opcode) {
	switch (*value) {
		case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
			return &dcpu->reg[opcode];
		case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
			return &dcpu->memory[dcpu->reg[opcode & 0x07]];
		case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
			return &dcpu->memory[dcpu->reg[opcode & 0x0f] + dcpu->memory[dcpu->pc++]];
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
			return &literal[*value & 0x1f];
		default:
			printf("ILLEGAL VALUE: %u", *value);
			return 0;
	}
}

word* get_opr_b(DCPU *dcpu, sbyte *value, byte opcode) {
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

void dcpu_do_inst(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b) {
	if (opcode == 0x0) {
		spec_op(dcpu, *opr_b, opr_a);
	}
	else {
		basic_op(dcpu, opcode, opr_a, opr_b);
	}
}

void basic_op(DCPU *dcpu, byte opcode, word *opr_a, word *opr_b) {
	sword *s_opr_a = opr_a;
	sword *s_opr_b = opr_b;
	switch (opcode) {
		case 0x01:
			//SET
			*opr_b = *opr_a;
			break;
		case 0x02:
			//ADD
			*opr_b = *opr_b + *opr_a;
			if (*opr_b < *opr_a) {
				//overflow
				dcpu->ex = 0x01;
			}
			break;
		case 0x03:
			//SUB
			*opr_b = *opr_b - *opr_a;
			if (*opr_b > *opr_a) {
				//underflow
				dcpu->ex = 0xffff;
			}
			break;
		case 0x04:
			//MUL
			*opr_b = *opr_b * *opr_a;
			dcpu->ex = ((*opr_b * *opr_a) >> 16) & 0xffff;
			break;
		case 0x05:
			//MLI
			*opr_b = *s_opr_b * *s_opr_a;
			dcpu->ex = ((*s_opr_b * *s_opr_a) >> 16) & 0xffff;
			break;
		case 0x06:
			//DIV
			if (*opr_a == 0) {
				*opr_b = 0;
				dcpu->ex = 0;
			}
			else {
				*opr_b = *opr_b / *opr_a;
				dcpu->ex = ((*opr_b << 16) / *opr_a) & 0xffff;	
			}
			break;
		case 0x07:
			//DVI
			if (*s_opr_a == 0) {
				*s_opr_b = 0;
				dcpu->ex = 0;
			}
			else {
				*s_opr_b = *s_opr_b / *s_opr_a;
				dcpu->ex = ((*s_opr_b << 16) / *s_opr_a) & 0xffff;	
			}
			break;
		case 0x08:
			//MOD
			*opr_b = *opr_a == 0 ? 0 : *opr_b % *opr_a;
			break;
		case 0x09:
			//MDI
			*opr_b = *opr_a == 0 ? 0 : *s_opr_b % *s_opr_a;
			break;
		case 0x0a:
			//AND
			*opr_b = *opr_b & *opr_a;
			break;
		case 0x0b:
			//BOR
			*opr_b = *opr_b | *opr_a;
			break;
		case 0x0c:
			//XOR
			*opr_b = *opr_b ^ *opr_a;
			break;
		case 0x0d:
			//SHR
			*opr_b = *opr_b >> *opr_a;
			dcpu->ex = ((*opr_b << 16) >> *opr_a) & 0xffff;
			break;
		case 0x0e:
			//AHR
			*opr_b = *s_opr_b >> *opr_a;
			dcpu->ex = ((*opr_b << 16) >> *opr_a) & 0xffff;
			break;
		case 0x0f:
			//SHL
			*opr_b = *s_opr_b << *opr_a;
			dcpu->ex = ((*opr_b << *opr_a) >> 16) & 0xffff;
			break;
		case 0x10:
			//IFB
			dcpu->pc += !((*opr_b & *opr_a) != 0);
			break;
		case 0x11:
			//IFC
			dcpu->pc += !((*opr_b & *opr_a) == 0);
			break;
		case 0x12:
			//IFE
			dcpu->pc += !(*opr_b == *opr_a);
			break;
		case 0x13:
			//IFN
			dcpu->pc += !(*opr_b != *opr_a);
			break;
		case 0x14:
			//IFG
			dcpu->pc += !(*opr_b > *opr_a);
			break;
		case 0x15:
			//IFA
			dcpu->pc += !(*s_opr_b > *s_opr_a);
			break;
		case 0x16:
			//IFL
			dcpu->pc += !(*opr_b < *opr_a);
			break;
		case 0x17:
			//IFU
			dcpu->pc += !(*s_opr_b < *s_opr_b);
			break;
		case 0x18: case 0x19:
			//NULL
			break;
		case 0x1a:
			//ADX
			*opr_b = *opr_b + *opr_a + dcpu->ex;
			if (*opr_b < *opr_a) {
				//overflow
				dcpu->ex = 0x1;
			}
			else {
				dcpu->ex = 0x0;
			}
			break;
		case 0x1b:
			//SBX
			*opr_b = *opr_b - *opr_a + dcpu->ex;
			if (*opr_b > *opr_a) {
				//underflow
				dcpu->ex = 0xffff;
			}
			else {
				dcpu->ex = 0x0;
			}
			break;
		case 0x1c: case 0x1d:
			//NULL
			break;
		case 0x1e:
			//STI
			*opr_b = *opr_a;
			dcpu->reg[REG_I]++;
			dcpu->reg[REG_J]++;
			break;
		case 0x1f:
			//STD
			*opr_b = *opr_a;
			dcpu->reg[REG_I]--;
			dcpu->reg[REG_J]--;
			break;
	}
}

void spec_op(DCPU *dcpu, word opcode, word *opr_a) {
	switch (opcode) {
		case 0x01:
			//JSR
			dcpu->memory[--dcpu->sp] = ++dcpu->pc;
			dcpu->pc = *opr_a;
			break;
		case 0x08:
			//INT
			//TODO
			break;
		case 0x09:
			//IAG
			*opr_a = dcpu->ia;
			break;
		case 0x0a:
			//IAS
			dcpu->ia = *opr_a;
			break;
	}
}

void queue_interrupt(DCPU *dcpu, word msg) {
	if (dcpu->iql >= 0xff) {
		printf("interrupt queue larger than 256");
	}
	else {
		//push
		dcpu->iq[dcpu->iql++] = msg;
	}
}

void interrupt(DCPU *dcpu) {
	if (dcpu->iql != 0) {
		trigger_interrupt(dcpu, dcpu->iq[0]);
		//shift
		memmove(&dcpu->iq[0], &dcpu->iq[1], sizeof(dcpu->iq) - sizeof(*dcpu->iq));
		dcpu->iql--;
	}
}

void trigger_interrupt(DCPU *dcpu, word msg) {

}

int main() {
	DCPU dcpu;
	dcpu.pc = 0;
	dcpu.sp = 0;
	dcpu.ex = 0;
	dcpu.ia = 0;
	memset(dcpu.memory, 0, sizeof dcpu.memory);
	memset(dcpu.iq, 0, sizeof dcpu.iq);
	dcpu.iql = 0;
	dcpu.iaq = false;
	dcpu.hwx = 0;
	LEM1802 lem1802;
	for (;;) {
		dcpu_cyc(&dcpu);
	}
	return 0;
}
