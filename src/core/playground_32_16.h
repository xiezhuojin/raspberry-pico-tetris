#ifndef PLAYGROUND_32_16_H
#define PLAYGROUND_32_16_H

#include "abstract_component.h"

#define PLAYGROUND_32_16_HEIGHT	16
#define PLAYGROUND_32_16_WIDTH	32

typedef struct
{
	Playground meta_playground;
	char fallen_piece[PLAYGROUND_32_16_HEIGHT * PLAYGROUND_32_16_WIDTH];
	char *rows[PLAYGROUND_32_16_HEIGHT];
} Playground_32_16;

extern void playground_32_16_init(Playground_32_16 *playground_32_16);

extern unsigned int clear_playground_32_16_contiguous_fallen_piece_row(FallingPiece *reference_falling_piece, 
	Playground_32_16 *playground_32_16);

#endif