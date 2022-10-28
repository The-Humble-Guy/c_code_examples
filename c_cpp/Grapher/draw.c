#include "draw.h"

void plot(int x, int y, char matrix[HEIGHT][WIDTH]) {
    matrix[Y - y][X + x] = '*';
}

void init_matrix(char matrix[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            matrix[y][x] = '.';
}

void output(char matrix[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", matrix[i][j]);
            if (j == WIDTH - 1) printf("\n");
        }
    }
}

void draw_enter(token_node_t *polish) {
    double x = 0;
    char matrix[HEIGHT][WIDTH];
    init_matrix(matrix);
    for (x = 0; x < (MAX_X); x += 0.1 * (MAX_X) / WIDTH) {
        double y = evaluate(polish, x);
        y = -y;  // переверну ось - по заданию
        if (fabs(y - MAX_Y) < 0.001) {
            plot(round(x / (MAX_X) * WIDTH), -Y, matrix);
        }
        if (fabs(y - MIN_Y) < 0.001) {
            plot(round(x / (MAX_X) * WIDTH), Y, matrix);
        }

        if (y > MIN_Y && y < MAX_Y) {
            plot(round(x / (MAX_X) * WIDTH), round(y / (abs(MAX_Y) + abs(MIN_Y)) * HEIGHT), matrix);
        }
    }
    output(matrix);
}
