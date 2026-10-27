#include "maze.h"

int generate_perfect_maze(maze *m, int height, int width) {
    if (height % 2 == 0 || width % 2 == 0) {
        fprintf(stderr, "height and width must be odd : %d %d\n", height, width);
        return 0;
    }

    init_maze(m, height, width);
    node **room_values = allocate_matrix(height, width);
    int rm_walls_size = height * width / 2;
    position removable_walls[rm_walls_size];
    int rm_wall_i = 0;

    int value = 0;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (m->map[i][j] == EMPTY) {
                room_values[i][j] = (node){value++, 0, &room_values[i][j]};
            }
            if ((i + j) % 2 == 1) {
                removable_walls[rm_wall_i++] = (position){i, j};
            }
        }
    }

    rm_walls_size = rm_wall_i;

    shuffle(removable_walls, rm_walls_size, sizeof(position));

    for (int i = 0; i < rm_walls_size; i++) {
        position wall_to_remove = removable_walls[i];

        if (!treat_wall(wall_to_remove, m, room_values, UP, DOWN)) {
            treat_wall(wall_to_remove, m, room_values, LEFT, RIGHT);
        }
    }

    *get_start_position(*m) = PLAYER;
    *get_door_tile(*m) = LOCKED_DOOR;
    m->height = height;
    m->width = width;

    free_matrix(room_values, height);

    add_items(m);

    return 1;
}

int init_maze(maze *m, int height, int width) {
    if (height % 2 == 0 || width % 2 == 0) {
        fprintf(stderr, "init maze doesn't support even height or width : %d %d", height, width);
        return 0;
    }
    m->height = height;
    m->width = width;
    m->score = SCORE_START;
    m->map = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        m->map[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            if (i % 2 == 0 || j % 2 == 0) {
                m->map[i][j] = WALL;
            } else {
                m->map[i][j] = EMPTY;
            }
        }
    }
    return 1;
}

int treat_wall(position wall_to_remove, maze *m, node **room_values, direction dir,
               direction opposed_dir) {
    position pos_dir = get_position_after_move(wall_to_remove, dir);
    position pos_opp = get_position_after_move(wall_to_remove, opposed_dir);
    tile *t_up = get_tile(*m, pos_dir);
    tile *t_down = get_tile(*m, pos_opp);
    node v_up = room_values[pos_dir.row][pos_dir.column];
    node v_down = room_values[pos_opp.row][pos_opp.column];

    if (*t_down == EMPTY && *t_up == EMPTY && merge(v_up, v_down)) {
        tile *wall = get_tile(*m, wall_to_remove);
        *wall = EMPTY;
        return 1;
    }
    return 0;
}

void add_items(maze *m) {
    position empty_tiles[m->height * m->width];
    int e_t_index = 0;
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (*get_tile(*m, (position){i, j}) == EMPTY) {
                empty_tiles[e_t_index++] = (position){i, j};
            }
        }
    }

    shuffle(empty_tiles, e_t_index, sizeof(position));
    int key_position_index = rand() % e_t_index;

    for (int i = 0; i < e_t_index - 1; i++) {
        if (i == key_position_index) {
            *get_tile(*m, empty_tiles[i]) = KEY;
        } else if (i % TREASURE_FREQUENCY == 0) {
            *get_tile(*m, empty_tiles[i]) = TREASURE;
        } else if (i % TRAP_FREQUENCY == 0) {
            *get_tile(*m, empty_tiles[i]) = TRAP;
        }
    }
}

void free_maze(maze *maze) {
    if (maze == NULL || maze->map == NULL) {
        return;
    }

    for (int i = 0; i < maze->height; i++) {
        free(maze->map[i]);
    }
    free(maze->map);
    maze->map = NULL;
    maze->height = 0;
    maze->width = 0;
}

// from  :  https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle(void *array, int n, int size) {
    srand(time(NULL));
    char tmp[size];
    char *arr = array;
    int stride = size * sizeof(char);

    if (n > 1) {
        int i;
        for (i = 0; i < n - 1; ++i) {
            int rnd = (int)rand();
            int j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}