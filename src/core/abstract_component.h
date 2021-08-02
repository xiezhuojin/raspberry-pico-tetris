#ifndef ABSTRACT_COMPONENT_H
#define ABSTRACT_COMPONENT_H

enum collide_type {
    not_collide, 
    collide_playground_left, 
    collide_playground_right, 
    collide_playground_bottom, 
    collide_fallen_piece
};

typedef struct
{
    signed int x;
    signed int y;
} Point;

typedef struct
{
    Point *point;
    unsigned int n_point;
    Point pivot_relative_position;
} FallingPiece;

typedef struct
{
    unsigned int height, width;
    char **rows;
} Playground;

typedef struct
{
    char *row;
    unsigned int order;
} RowInfo;

extern void playground_init(Playground *playground, unsigned int height, unsigned int width, 
    char *fallen_piece, char *rows[]);

extern void falling_piece_reset_pivot(FallingPiece *falling_piece);

extern void falling_piece_rotate_90_degree(FallingPiece *falling_piece);
extern void falling_piece_rotate_180_degree(FallingPiece *falling_piece);
extern void falling_piece_rotate_270_degree(FallingPiece *falling_piece);

extern void falling_piece_left_shift(FallingPiece *falling_piece);
extern void falling_piece_right_shift(FallingPiece *falling_piece);
extern void falling_piece_up_shift(FallingPiece *falling_piece);
extern void falling_piece_down_shift(FallingPiece *falling_piece);

extern unsigned char falling_piece_above_playground(FallingPiece *falling_piece, 
    Playground *playground);
extern unsigned char falling_piece_collide_with_playground(FallingPiece *falling_piece,  
    Playground *playground);

extern void project_falling_piece_to_playground_fallen_piece(FallingPiece *falling_piece, 
    Playground *playground);
extern unsigned int get_playground_contiguous_fallen_piece_row(FallingPiece *reference_falling_piece, 
    Playground *playground, RowInfo row_info[]);
extern void clear_playground_contiguous_fallen_piece_row(Playground *playground, 
    RowInfo row_info[]);

#endif