#ifndef LAYOUT_H
#define LAYOUT_H
#include "keys.h"
#define NUM_LAYERS 3

const uint16_t layout[3][4][12] = {
	{ // Layer 0 - Standard layout
		{KEY_ESC, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSPACE},
		{KEY_TAB, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE},
		{KEY_MOD_LSHIFT | KEY_MOD_MACRO, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_ENTER},
		{KEY_QUICK_FN | KEY_LS_MACRO, KEY_MOD_LCTRL | KEY_MOD_MACRO, KEY_MOD_LALT | KEY_MOD_MACRO, KEY_MOD_LMETA | KEY_MOD_MACRO, 0, KEY_SPACE, KEY_SPACE, KEY_LAYER_UP | KEY_LS_MACRO, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT}  
	},
	{ // Layer 1 - Numbers and symbols
		{KEY_ESC, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_BACKSPACE},
		{KEY_TAB, KEY_MINUS, KEY_EQUAL, KEY_BACKSLASH, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_GRAVE, 0, 0, 0, 0, 0},
		{KEY_MOD_LSHIFT | KEY_MOD_MACRO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_ENTER},
		{KEY_QUICK_FN | KEY_LS_MACRO, KEY_MOD_LCTRL | KEY_MOD_MACRO, KEY_MOD_LALT | KEY_MOD_MACRO, KEY_MOD_LMETA | KEY_MOD_MACRO, KEY_SPACE, KEY_SPACE, KEY_SPACE, KEY_LAYER_UP | KEY_LS_MACRO, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT}  
	},
	{
		{KEY_ESC, KEY_F7, KEY_F8, KEY_F9, KEY_F11, KEY_F12, KEY_F10, KEY_F1, KEY_F2, 0, 0, KEY_BACKSPACE},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{KEY_QUICK_FN | KEY_LS_MACRO, KEY_MOD_LCTRL | KEY_MOD_MACRO, KEY_MOD_LALT | KEY_MOD_MACRO, KEY_MOD_LMETA | KEY_MOD_MACRO, 0, KEY_SPACE, KEY_SPACE, KEY_LAYER_UP | KEY_LS_MACRO, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT}  
	}

};

#endif
