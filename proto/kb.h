#ifndef KB_H
#define KB_H

#include "adc.h"
#include "uart.h"

enum {
	KB_PRESSED,
	KB_RELEASED,
	KB_WAIT
};

#define GROUPS_Q 4
#define CODES_Q 10
#define DELTA   6

static uint32_t kb_codes[CODES_Q] = {156, 170, 185, 204, 227, 288, 337, 405, 507, 678 };

static uint32_t kb_layout[CODES_Q * GROUPS_Q] = {
0x04, 0x05, 0x06, 0x07, 0x08,
0x09, 0x0A, 0x0B, 0x0C, 0x0D,
0x04, 0x05, 0x06, 0x07, 0x08,
0x09, 0x0A, 0x0B, 0x0C, 0x0D,
0x04, 0x05, 0x06, 0x07, 0x08,
0x09, 0x0A, 0x0B, 0x0C, 0x0D,
0x04, 0x05, 0x06, 0x07, 0x08,
0x09, 0x0A, 0x0B, 0x0C, 0x0D
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
