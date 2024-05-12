#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdint.h>

extern uint8_t key_pressed;

void
matrix_init ();

void
do_matrix_scan ();

#endif /* __MATRIX_H__ */
