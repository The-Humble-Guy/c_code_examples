// Copyright 2022 Team Deadshot, ednastra, columbus
#include <stdio.h>
#include <stdlib.h>

#define PLATFORM_CHARACTER                '%'
#define PLATFORM_LENGHT                    3

#define BALL_CHARACTER                    '*'
#define BALL_NO_SPEED                      0
#define BALL_SPEED_LEFT                   -1
#define BALL_SPEED_RIGHT                   1
#define BALL_SPEED_TOP                    -1
#define BALL_SPEED_BOT                     1

#define MAP_WIDTH                         80
#define MAP_LENGHT                        25
#define MAP_BORDER_SYMBOL                 '#'

#define PLATFORM_STATE_HIGH               10
#define PLATFORM_STATE_BOT                11
#define PLATFORM_STATE_NO_CHANGE          12

#define ALLIGNMENT_LEFT                   13
#define ALLIGNMENT_RIGHT                  14

#define GAME_STATUS_NO_START              60
#define GAME_STATUS_OK                    61
#define GAME_STATUS_NEW_ROUND1             62
#define GAME_STATUS_NEW_ROUND2            63
#define GAME_STATUS_FINISH                64
#define GAME_TOTAL_SCORE                  21


typedef struct {
    int status;
    int player1_score;
    int player2_score;
} game_t;

typedef struct {
    int x_coordinate;
    int y_coordinate;
    int x_movespeed;
    int y_movespeed;
} ball_t;

typedef struct {
    int position_high;
    int state;
    int allignment;
} platform_t;

void show(platform_t left, platform_t right, ball_t ball, game_t game) {
    system("clear");
    printf("%d - %d\n", game.player1_score, game.player2_score);
    for (int x = 0; x < MAP_WIDTH + 3; ++x) {
        printf("%c", MAP_BORDER_SYMBOL);
    }
    printf("\n");
    for (int y = 0; y < MAP_LENGHT; ++y) {
        printf("%c", MAP_BORDER_SYMBOL);
        for (int x = 0; x < MAP_WIDTH; ++x) {
            switch (x) {
                case 0:
                    if ((y >= left.position_high) &&
                        (y <= (left.position_high + PLATFORM_LENGHT - 1))) {
                        printf("%c", PLATFORM_CHARACTER);
                    } else {
                        printf(" ");
                    }
                    if ((x == ball.x_coordinate) && (y == ball.y_coordinate)) {
                        printf("%c", BALL_CHARACTER);
                    } else {
                        printf(" ");
                    }
                    break;
                case MAP_WIDTH - 1:
                    if ((y >= right.position_high) &&
                        (y <= (right.position_high + PLATFORM_LENGHT - 1))) {
                        printf("%c", PLATFORM_CHARACTER);
                    } else if ((x == ball.x_coordinate) &&
                               (y == ball.y_coordinate)) {
                        printf("%c", BALL_CHARACTER);
                    } else {
                        printf(" ");
                    }
                    break;
                default:
                    if ((x == ball.x_coordinate) &&
                        (y == ball.y_coordinate)) {
                        printf("%c", BALL_CHARACTER);
                    } else {
                        printf(" ");
                    }
            }
        }
        printf("%c\n", MAP_BORDER_SYMBOL);
    }
    for (int x = 0; x < MAP_WIDTH + 3; ++x) {
        printf("%c", MAP_BORDER_SYMBOL);
    }
    printf("\n");
}

int game_check_finish(game_t game) {
    if ((game.player1_score == GAME_TOTAL_SCORE) ||
        (game.player2_score == GAME_TOTAL_SCORE)) {
        return GAME_STATUS_FINISH;
    }
    return 0;
}

void platform_change(platform_t *platform, int state) {
    switch (state) {
        case PLATFORM_STATE_HIGH:
            if (platform->position_high - 1 >= 0) {
                platform->position_high -= 1;
                platform->state = PLATFORM_STATE_HIGH;
            }
            break;
        case PLATFORM_STATE_BOT:
            if (platform->position_high + PLATFORM_LENGHT + 1 <= MAP_LENGHT) {
                platform->position_high += 1;
                platform->state = PLATFORM_STATE_BOT;
            }
            break;
        case PLATFORM_STATE_NO_CHANGE:
            platform->state = PLATFORM_STATE_NO_CHANGE;
            break;
        default:
            break;
    }
}

void
ball_move(ball_t *ball, platform_t *left, platform_t *right, game_t *game) {
    if ((ball->x_movespeed == BALL_NO_SPEED) ||
        (ball->y_movespeed == BALL_NO_SPEED)) {
        return;
    }
    switch (ball->y_coordinate) {
        case 0:
            ball->y_movespeed = BALL_SPEED_BOT;
            break;
        case MAP_LENGHT - 1:
            ball->y_movespeed = BALL_SPEED_TOP;
            break;
        default:
            break;
    }
    switch (ball->x_coordinate) {
        case 0:
            game->player2_score += 1;
            if (game_check_finish(*game)) {
                game->status = GAME_STATUS_FINISH;
            } else {
                game->status = GAME_STATUS_NEW_ROUND1;
            }
            return;
        case 1:
            if ((ball->y_coordinate < left->position_high + PLATFORM_LENGHT) &&
                (ball->y_coordinate >= left->position_high)) {
                ball->x_movespeed = BALL_SPEED_RIGHT;
            }
            break;
        case MAP_WIDTH - 1:
            game->player1_score += 1;
            if (game_check_finish(*game)) {
                game->status = GAME_STATUS_FINISH;
            } else {
                game->status = GAME_STATUS_NEW_ROUND2;
            }
            return;
        case MAP_WIDTH - 2:
            if ((ball->y_coordinate < right->position_high + PLATFORM_LENGHT) &&
                (ball->y_coordinate >= right->position_high)) {
                ball->x_movespeed = BALL_SPEED_LEFT;
            }
            break;
        default:
            break;
    }

    ball->x_coordinate += ball->x_movespeed;
    ball->y_coordinate += ball->y_movespeed;
}

void control_btn_change(char control_btn, platform_t *left, platform_t *right,
                        ball_t *ball, game_t *game) {
    switch (control_btn) {
        case 'a':
        case 'A':
            platform_change(left, PLATFORM_STATE_HIGH);
            break;
        case 'z':
        case 'Z':
            platform_change(left, PLATFORM_STATE_BOT);
            break;
        case 'k':
        case 'K':
            platform_change(right, PLATFORM_STATE_HIGH);
            break;
        case 'm':
        case 'M':
            platform_change(right, PLATFORM_STATE_BOT);
            break;
        default:
            platform_change(left, PLATFORM_STATE_NO_CHANGE);
            platform_change(right, PLATFORM_STATE_NO_CHANGE);
            break;
    }
    ball_move(ball, left, right, game);
}

void set_ball_direction(platform_t platform, ball_t *ball) {
    switch (platform.allignment) {
        case ALLIGNMENT_LEFT:
            ball->x_movespeed = BALL_SPEED_RIGHT;
            break;
        case ALLIGNMENT_RIGHT:
            ball->x_movespeed = BALL_SPEED_LEFT;
            break;
    }
    switch (platform.state) {
        case PLATFORM_STATE_BOT:
            ball->y_movespeed = BALL_SPEED_BOT;
            break;
        case PLATFORM_STATE_HIGH:
            ball->y_movespeed = BALL_SPEED_TOP;
            break;
        case PLATFORM_STATE_NO_CHANGE:
            ball->y_movespeed = BALL_NO_SPEED;
            ball->x_movespeed = BALL_NO_SPEED;
            break;
    }
}

void
refresh_screen(platform_t *left, platform_t *right, ball_t *ball, game_t game) {
    if (ball->x_coordinate == MAP_WIDTH - 1) {
        ball->x_coordinate = MAP_WIDTH - 2;
    } else {
        ball->x_coordinate = 1;
    }
    ball->y_coordinate = 12;
    ball->x_movespeed = ball->y_movespeed = BALL_NO_SPEED;

    left->position_high = right->position_high = 11;
    left->state = right->state = PLATFORM_STATE_NO_CHANGE;

    show(*left, *right, *ball, game);
}

int main() {
    char control_btn = 0;
    int set_direction = 0;
    int flag_start = 1;  // 1 - start left, 0 - start right

    game_t game = {GAME_STATUS_NO_START, 0, 0};
    ball_t ball = {1, 12, BALL_NO_SPEED, BALL_NO_SPEED};
    platform_t platform_left = {11, PLATFORM_STATE_NO_CHANGE, ALLIGNMENT_LEFT};
    platform_t platform_right = {11, PLATFORM_STATE_NO_CHANGE,
                                 ALLIGNMENT_RIGHT};
    show(platform_left, platform_right, ball, game);


    while (game.status != GAME_STATUS_FINISH) {
        scanf("%c", &control_btn);


        if (!set_direction) {
            if (flag_start &&
                (platform_left.state != PLATFORM_STATE_NO_CHANGE)) {
                set_ball_direction(platform_left, &ball);
                set_direction = 1;
            }
            if (!flag_start &&
                (platform_right.state != PLATFORM_STATE_NO_CHANGE)) {
                set_ball_direction(platform_right, &ball);
                set_direction = 1;
            }
        }
        control_btn_change(control_btn, &platform_left, &platform_right, &ball,
                           &game);
        if (game.status == GAME_STATUS_NEW_ROUND1) {
            refresh_screen(&platform_left, &platform_right, &ball, game);
            game.status = GAME_STATUS_OK;
            flag_start = 1;
            set_direction = 0;
        }
        if (game.status == GAME_STATUS_NEW_ROUND2) {
            refresh_screen(&platform_left, &platform_right, &ball, game);
            game.status = GAME_STATUS_OK;
            flag_start = 0;
            set_direction = 0;
        } else {
            show(platform_left, platform_right, ball, game);
        }

        if (game.status == GAME_STATUS_FINISH) {
            printf("Win ");
            if (game.player1_score == GAME_TOTAL_SCORE) {
                printf("1");
            } else {
                printf("2");
            }
        }
    }
    return 0;
}
