#ifndef KEYB_H
#define KEYB_H

#include "adc.h"
#include "uart.h"
#include "keycodes.h"

enum {
	KB_PRESSED,
	KB_RELEASED,
	KB_WAIT
};

#define GROUPS_Q 4
#define CODES_Q 10
#define DELTA   7

static uint32_t kb_codes[CODES_Q] = { 156, 170, 185, 204, 227, 288, 337, 405, 507, 678 };

static uint32_t kb_layout[CODES_Q * GROUPS_Q] = {
						KB_B, KB_N, KB_M, KB_SHIFT, KB_SPACE,
						KB_H, KB_J, KB_K, KB_L, KB_ENTER, 
						KB_Y, KB_U, KB_I, KB_O, KB_P, 
						KB_6, KB_7, KB_8, KB_9, KB_0, 
						KB_1, KB_2, KB_3, KB_4, KB_5, 
						KB_Q, KB_W, KB_E, KB_R, KB_T, 
					 KB_CAPS, KB_Z, KB_X, KB_C, KB_V, 
						KB_A, KB_S, KB_D, KB_F, KB_G 
				};

typedef struct keyboard_t {
	uint8_t state;  // enum
	uint32_t key;
} keyboard_t;

int kb_key_code(int);
int kb_key_value(int, int);
void kb_pin_report(uint8_t);
void kb_report(void);
int kb_key_pressed(void);

#endif
