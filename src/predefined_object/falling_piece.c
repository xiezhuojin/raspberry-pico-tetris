#include "falling_piece.h"

// #
Point point_1_1[] = {{.x = 0, .y = 0}};
FallingPiece falling_piece_1_1 = {
    .point = point_1_1,
    .n_point = sizeof(point_1_1) / sizeof(Point)
};

// ##
Point point_2_1[] = {{.x = 0, .y = 0}, {.x = 1, .y = 0}};
FallingPiece falling_piece_2_1 = {
    .point = point_2_1,
    .n_point = sizeof(point_2_1) / sizeof(Point)
};

// ###
Point point_3_1[] = {{.x = -1, .y = 0}, {.x = 0, .y = 0}, {.x = 1, .y = 0}};
FallingPiece falling_piece_3_1 = {
    .point = point_3_1,
    .n_point = sizeof(point_3_1) / sizeof(Point)
};

// #
// ##
Point point_3_2[] = {{.x = 0, .y = 1}, {.x = 0, .y = 0}, {.x = 1, .y = 0}};
FallingPiece falling_piece_3_2 = {
    .point = point_3_2,
    .n_point = sizeof(point_3_2) / sizeof(Point)
};

// #
// #
// #
// #
Point point_4_1[] = {{.x = 0, .y = 2}, {.x = 0, .y = 1}, {.x = 0, .y = 0}, {.x = 0, .y = -1}};
FallingPiece falling_piece_4_1 = {
    .point = point_4_1,
    .n_point = sizeof(point_4_1) / sizeof(Point)
};

// #
// ###
Point point_4_2[] = {{.x = 0, .y = 1}, {.x = 0, .y = 0}, {.x = 1, .y = 0}, {.x = 2, .y = 0}};
FallingPiece falling_piece_4_2 = {
    .point = point_4_2,
    .n_point = sizeof(point_4_2) / sizeof(Point)
};

//  #
// ###
Point point_4_3[] = {{.x = 0, .y = 1}, {.x = 0, .y = 0}, {.x = 1, .y = 0}, {.x = 0, .y = -1}};
FallingPiece falling_piece_4_3 = {
    .point = point_4_3,
    .n_point = sizeof(point_4_3) / sizeof(Point)
};

// #
// ##
//  #
Point point_4_4[] = {{.x = 0, .y = 1}, {.x = 0, .y = 0}, {.x = 1, .y = 0}, {.x = 1, .y = -1}};
FallingPiece falling_piece_4_4 = {
    .point = point_4_4,
    .n_point = sizeof(point_4_4) / sizeof(Point),
};

// ##
// ##
Point point_4_5[] = {{.x = 0, .y = 1}, {.x = 1, .y = 1}, {.x = 0, .y = 0}, {.x = 1, .y = 0}};
FallingPiece falling_piece_4_5 = {
    .point = point_4_5,
    .n_point = sizeof(point_4_5) / sizeof(Point),
};