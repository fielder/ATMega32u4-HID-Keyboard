#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdint.h>

/* sequence coming from controller */
#define SNES_B		0
#define SNES_Y		1
#define SNES_SELECT	2
#define SNES_START	3
#define SNES_UP		4
#define SNES_DOWN	5
#define SNES_LEFT	6
#define SNES_RIGHT	7
#define SNES_A		8
#define SNES_X		9
#define SNES_L		10
#define SNES_R		11

extern uint8_t snes_pressed[12];
#define SNES_CNT (sizeof(snes_pressed) / sizeof(snes_pressed[0]))

void
matrix_init ();

void
do_matrix_scan ();

#endif /* __MATRIX_H__ */
