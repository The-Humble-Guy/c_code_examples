#ifndef SRC_DRAW_H_
#define SRC_DRAW_H_

#include <stdio.h>
#include <math.h>
#include "polish_notation.h"
#include "definitions.h"

#define WIDTH 80
#define HEIGHT 25
#define MAX_Y 1
#define MIN_Y -1
#define MAX_X 4*M_PI
#define X 0  // центр координат
#define Y HEIGHT/2  // центр координат

void draw_enter(token_node_t *polish);

void plot(int x, int y, char matrix[HEIGHT][WIDTH]);
void init_matrix(char matrix[HEIGHT][WIDTH]);
void output(char matrix[HEIGHT][WIDTH]);

#endif  // SRC_DRAW_H_
