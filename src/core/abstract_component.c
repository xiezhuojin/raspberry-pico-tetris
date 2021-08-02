#include "abstract_component.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
    signed int matrix[2][2];
} TransformationMatrix;

static void transform(const TransformationMatrix *transform_matrix, FallingPiece *falling_piece);
static int compare_row_info(const void *ptr1, const void *ptr2);

void playground_init(Playground *playground, unsigned int height, unsigned int width, 
    char *fallen_piece, char *rows[])
{
	playground->height = height;
	playground->width = width;
	playground->rows = rows;
}

void falling_piece_reset_pivot(FallingPiece *falling_piece) {
    falling_piece->pivot_relative_position.x = 0;
    falling_piece->pivot_relative_position.y = 0;
}

void falling_piece_rotate_90_degree(FallingPiece *falling_piece) {
    static const TransformationMatrix rotate_90_degree_matrix = {
        .matrix = {
            {0, -1},
            {1, 0}
        }
    };
    transform(&rotate_90_degree_matrix, falling_piece);
}

void falling_piece_rotate_180_degree(FallingPiece *falling_piece) {
    static const TransformationMatrix rotate_180_degree_matrix = {
        .matrix = {
            {-1, 0},
            {0, -1}
        }
    };
    transform(&rotate_180_degree_matrix, falling_piece);
}

void falling_piece_rotate_270_degree(FallingPiece *falling_piece) {
    static const TransformationMatrix rotate_270_degree_matrix = {
        .matrix = {
            {0, 1},
            {-1, 0}
        }
    };
    transform(&rotate_270_degree_matrix, falling_piece);
}

void falling_piece_left_shift(FallingPiece *falling_piece) {
    falling_piece->pivot_relative_position.x -= 1;
}

void falling_piece_right_shift(FallingPiece *falling_piece) {
    falling_piece->pivot_relative_position.x += 1;
}

void falling_piece_up_shift(FallingPiece *falling_piece) {
    falling_piece->pivot_relative_position.y += 1;
}

void falling_piece_down_shift(FallingPiece *falling_piece) {
    falling_piece->pivot_relative_position.y -= 1;
}

unsigned char falling_piece_above_playground(FallingPiece *falling_piece, 
    Playground *playground)
{
    unsigned int i;
    signed int y;

    for (i = 0; i < falling_piece->n_point; ++i) {
        y = falling_piece->point[i].y + falling_piece->pivot_relative_position.y;
        if (y < playground->height)
            return 0;
    }
    return 1;
}

unsigned char falling_piece_collide_with_playground(FallingPiece *falling_piece,  
    Playground *playground)
{
    unsigned int i;
    signed int x, y;

    for (i = 0; i < falling_piece->n_point; ++i) {
        x = falling_piece->point[i].x + falling_piece->pivot_relative_position.x;
        y = falling_piece->point[i].y + falling_piece->pivot_relative_position.y;

        if (y <= (signed long long) playground->height) {
            if (x < 0)
                return collide_playground_left;
            else if (x >= playground->width)
                return collide_playground_right;
            else if (y < 0)
                return collide_playground_bottom;
            else if ( *(*(playground->rows + y) + x) )
                return collide_fallen_piece;
        }
    }
    return not_collide;
}

void project_falling_piece_to_playground_fallen_piece(FallingPiece *falling_piece, 
    Playground *playground)
{
    unsigned int i;
    unsigned int x, y;

    for (i = 0; i < falling_piece->n_point; ++i) {
        x = falling_piece->point[i].x + falling_piece->pivot_relative_position.x;
        y = falling_piece->point[i].y + falling_piece->pivot_relative_position.y;
        
        playground->rows[y][x] = 1;
	}
}

unsigned int get_playground_contiguous_fallen_piece_row(FallingPiece *reference_falling_piece, 
    Playground *playground, RowInfo row_info[])
{
	unsigned int n_contiguous_fallen_piece_row = 0;
    unsigned int current_point_row;
    unsigned int previous_point_row;
    unsigned int i, j;

    for (i = 0; i < playground->height; ++i) {
		row_info[i].row = &playground->rows[i][0];
		row_info[i].order = i;
    }

    for (i = 0; i < reference_falling_piece->n_point; ++i) {
        current_point_row = reference_falling_piece->point[i].y + reference_falling_piece->pivot_relative_position.y;
        if (i == 0)
            previous_point_row = current_point_row;
        else
            if (previous_point_row == current_point_row)
                continue;
            else
                previous_point_row = current_point_row;

        for (j = 0; j < playground->width; ++j) {
            if (!playground->rows[current_point_row][j])
                break;
            else
                if (j == playground->width - 1) {
                    row_info[current_point_row].order = playground->height;
                    ++n_contiguous_fallen_piece_row;
                }
        }
    }

    return n_contiguous_fallen_piece_row;
}

extern void clear_playground_contiguous_fallen_piece_row(Playground *playground, 
    RowInfo row_info[])
{
    unsigned int i, j;

    for (i = 0; i < playground->height; ++i) {
		if (row_info[i].order == playground->height)
            memset(row_info[i].row, 0, playground->width * sizeof(char));
    }

    qsort(row_info, playground->height, sizeof(RowInfo), compare_row_info);
    for (i = 0; i < playground->height; ++i)
		*(playground->rows + i) = row_info[i].row;
}

static void transform(const TransformationMatrix *transform_matrix, FallingPiece *falling_piece) {
    unsigned int i;
    Point tmp_point;

    for (i = 0; i < falling_piece->n_point; ++i) {
        tmp_point.x = falling_piece->point[i].x * transform_matrix->matrix[0][0] + falling_piece->point[i].y * transform_matrix->matrix[1][0];
        tmp_point.y = falling_piece->point[i].x * transform_matrix->matrix[0][1] + falling_piece->point[i].y * transform_matrix->matrix[1][1];

        falling_piece->point[i].x = tmp_point.x;
        falling_piece->point[i].y = tmp_point.y;
    }
}

int compare_row_info(const void *ptr1, const void *ptr2) {
    RowInfo *row_a = (RowInfo *) ptr1;
    RowInfo *row_b = (RowInfo *) ptr2;

    return (row_a->order - row_b->order) - (row_b->order - row_a->order);
}