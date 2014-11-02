#ifndef DEMON_EMULATE_HARDWARE_H_
#define DEMON_EMULATE_HARDWARE_H_

typedef struct LEM1802 {
	word pal[16];
	word font[256];
	word vra;
	word fra;
	word pra;
	word bc;
} LEM1802;

void received_hardware_interrupt(DCPU *dcpu, LEM1802 *lem1802);

#endif
