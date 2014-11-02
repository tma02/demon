#include <stdint.h>
#include <stdbool.h>
#include "emulate.h"
#include "hardware.h"

void received_hardware_interrupt(DCPU *dcpu, LEM1802 *lem1802) {
	switch (dcpu->reg[REG_A]) {
		case 0:
			//MEM_MAP_SCREEN
			lem1802->vra = dcpu->reg[REG_B];
			break;
		case 1:
			//MEM_MAP_FONT
			lem1802->fra = dcpu->reg[REG_B];
			break;
		case 2:
			//MEM_MAP_PALETTE
			lem1802->pra = dcpu->reg[REG_B];
			break;
		case 3:
			//SET_BORDER_COLOR
			lem1802->bc = dcpu->reg[REG_B];
			break;
	}
}
