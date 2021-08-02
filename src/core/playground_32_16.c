#include "playground_32_16.h"
#include <string.h>

void playground_32_16_init(Playground_32_16 *playground_32_16) {
	unsigned int i;

	playground_32_16->meta_playground.height = PLAYGROUND_32_16_HEIGHT;
	playground_32_16->meta_playground.width = PLAYGROUND_32_16_WIDTH;

	memset(playground_32_16->fallen_piece, 0, PLAYGROUND_32_16_HEIGHT * PLAYGROUND_32_16_WIDTH * sizeof(unsigned char));

	for (i = 0; i < PLAYGROUND_32_16_HEIGHT; ++i)
		playground_32_16->rows[i] = &playground_32_16->fallen_piece[i * PLAYGROUND_32_16_WIDTH];
	playground_32_16->meta_playground.rows = playground_32_16->rows;
}

unsigned int clear_playground_32_16_contiguous_fallen_piece_row(FallingPiece *reference_falling_piece, 
	Playground_32_16 *playground_32_16)
{
	unsigned int n_contiguous_row;

	RowInfo row_info[PLAYGROUND_32_16_HEIGHT];
	n_contiguous_row = get_playground_contiguous_fallen_piece_row(reference_falling_piece, &playground_32_16->meta_playground, row_info);
	clear_playground_contiguous_fallen_piece_row(&playground_32_16->meta_playground, row_info);

	return n_contiguous_row;
}
