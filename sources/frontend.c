#include "frontend.h"

void display_maze(maze m) {
    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            position pos = {i, j};
            print_character(m, pos);
        }
        printf("\n");
    }
}

void print_character(maze m, position pos) {
    switch (m.map[pos.row][pos.column]) {
    case EMPTY:
        printf(" ");
        break;
    case WALL:
        print_wall_character(m, pos);
        break;
    case PLAYER:
        printf("ᐂ");
        break;
    case EXIT:
        printf("-");
        break;
    case LOCKED_DOOR:
        printf("█");
        break;
    case KEY:
        printf("k");
        break;
    case TREASURE:
        printf("+");
        break;
    case TRAP:
        printf("w");
        break;
    default:
        printf("E");
        break;
    }
}

void print_wall_character(maze m, position pos) {
    int up = is_wall_at(m, get_position_after_move(pos, UP));
    int down = is_wall_at(m, get_position_after_move(pos, DOWN));
    int left = is_wall_at(m, get_position_after_move(pos, LEFT));
    int right = is_wall_at(m, get_position_after_move(pos, RIGHT));

    if (up && down && left && right) {
        printf("╋");
    } else if (!up && down && left && right) {
        printf("┳");
    } else if (up && !down && left && right) {
        printf("┻");
    } else if (up && down && !left && right) {
        printf("┣");
    } else if (up && down && left && !right) {
        printf("┫");
    } else if (!up && down && !left && right) {
        printf("┏");
    } else if (!up && down && left && !right) {
        printf("┓");
    } else if (up && !down && !left && right) {
        printf("┗");
    } else if (up && !down && left && !right) {
        printf("┛");
    } else if (up && down) {
        printf("┃");
    } else if (right && left) {
        printf("━");
    } else if (up) {
        printf("╹");
    } else if (down) {
        printf("╻");
    } else if (left) {
        printf("╸");
    } else if (right) {
        printf("╺");
    }
}
