#include "player_actions.h"

void show_menu() {
    printf("┏━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ 0 : Quit        ┃\n");
    printf("┃ 1 : Create Maze ┃\n");
    printf("┃ 2 : Load Maze   ┃\n");
    printf("┃ 3 : Play        ┃\n");
    printf("┃ 4 : Score Board ┃\n");
    printf("┗━━━━━━━━━━━━━━━━━┛\n");
}

void build_file_path(char *maze_name, char *question, char *path) {
    ask_for_string(MAX_MAZE_NAME_SIZE, question, maze_name);

    snprintf(path, MAX_FILE_PATH_SIZE, "mazes/%s.cfg", maze_name);
}

int player_interact(maze *m) {
    int action_number = ask_for_number(0, 4, "Choose your action : ");
    switch ((action)action_number) {
    case QUIT:
        return 0;
    case CREATE_MAZE:
        create_maze(m);
        return 1;
    case LOAD_MAZE:
        load_maze(m);
        return 1;
    case PLAY:
        if (m == NULL) {
            fprintf(stderr, "Please load or create a maze you want to play on\n");
            return 1;
        }
        play(m);
        return 1;
    case SCORE_BOARD:
        show_score_board();
        return 1;
    default:
        fprintf(stderr, "Unknown action, quitting...\n");
        return 0;
    }
}

void save_maze(maze *m, char *path) {
    FILE *maze_file = fopen(path, "wb");
    if (maze_file == NULL) {
        fprintf(stderr, "Error while creating maze file please try again...\n");
        return;
    }

    fwrite(&m->height, 1, sizeof(int), maze_file);
    fwrite(&m->width, 1, sizeof(int), maze_file);
    for (int i = 0; i < m->height; i++) {
        fwrite(m->map[i], sizeof(tile), m->width, maze_file);
    }
    fwrite(m->maze_name, sizeof(char), MAX_MAZE_NAME_SIZE, maze_file);

    printf("Maze saved to it's file\n");
    fclose(maze_file);
}

void create_maze(maze *m) {
    int height = ask_for_number(3, MAX_MAZE_HEIGHT, "Height of your maze (must be odd and > 3) : ");
    while (height % 2 == 0) {
        fprintf(stderr, "Given height isn't odd...\n");
        height = ask_for_number(3, MAX_MAZE_HEIGHT, "Height of your maze (must be odd and > 3) : ");
    }
    int width = ask_for_number(3, MAX_MAZE_WIDTH, "Width of your maze (must be odd and > 3) : ");
    while (width % 2 == 0) {
        fprintf(stderr, "Given width isn't odd...\n");
        width = ask_for_number(3, MAX_MAZE_WIDTH, "Width of your maze (must be odd and > 3) : ");
    }

    generate_perfect_maze(m, height, width);

    display_maze(*m);

    char maze_name[MAX_MAZE_NAME_SIZE];
    char maze_path_name[MAX_FILE_PATH_SIZE];
    build_file_path(maze_name, "Enter the name of your maze : ", maze_path_name);

    // checking if the file already exists
    FILE *f = fopen(maze_path_name, "r");
    while (f != NULL) {
        fclose(f);
        fprintf(stderr, "This maze name is already used please enter another one...\n");
        build_file_path(maze_name, "Enter the name of your maze : ", maze_path_name);

        f = fopen(maze_path_name, "r");
    }
    strcpy(m->maze_name, maze_name);

    save_maze(m, maze_path_name);
}

int show_file_list(char *list_name) {
    struct dirent *de;
    char path[MAX_FILE_PATH_SIZE];
    snprintf(path, MAX_FILE_PATH_SIZE, "./%s/", list_name);

    DIR *dr = opendir(path);

    int n = 0;
    if (dr == NULL) {
        printf("Could not open current directory");
        return 0;
    }
    while ((de = readdir(dr)) != NULL) {
        if (++n > 2) {
            break;
        }
    }
    if (n <= 2) { // there's only . and .. considering empty
        fprintf(
            stderr,
            "Error there's no maze available, please create one before trying to load a maze\n");
        closedir(dr);
        return 0;
    }
    rewinddir(dr);

    printf("\nList of available %s : \n", list_name);
    while ((de = readdir(dr)) != NULL)
        if (de->d_name[0] != '.')
            printf("%s\n", de->d_name);
    closedir(dr);
    printf("\n");
    return 1;
}

void load_maze(maze *m) {
    if (!show_file_list("mazes")) {
        return;
    }

    char maze_name[MAX_MAZE_NAME_SIZE];
    char maze_path_name[MAX_FILE_PATH_SIZE];
    build_file_path(maze_name, "Enter the name of the maze you want to open : ", maze_path_name);

    FILE *maze_file = fopen(maze_path_name, "r");
    while (maze_file == NULL) {
        fprintf(stderr, "File doesn't exist please try again\n");
        build_file_path(maze_name,
                        "Enter the name of the maze you want to open : ", maze_path_name);

        maze_file = fopen(maze_path_name, "rb");
    }
    strcpy(m->maze_name, maze_name);

    file_maze_load(m);
}

void file_maze_load(maze *m) {
    char path[MAX_FILE_PATH_SIZE];
    snprintf(path, MAX_FILE_PATH_SIZE, "mazes/%s.cfg", m->maze_name);

    FILE *maze_file = fopen(path, "r");
    while (maze_file == NULL) {
        fprintf(stderr, "Maze file couldn't be found\n");
        return;
    }

    int height;
    int width;
    fread(&height, 1, sizeof(int), maze_file);
    fread(&width, 1, sizeof(int), maze_file);

    init_maze(m, height, width);
    for (int i = 0; i < height; i++) {
        fread(m->map[i], sizeof(m->map[i][0]), width, maze_file);
    }
    fread(m->maze_name, sizeof(char), MAX_MAZE_NAME_SIZE, maze_file);

    display_maze(*m);
    fclose(maze_file);
}

void play(maze *m) {

    direction dir;
    char movement_code;

    char movement[3];
    do {
        display_maze(*m);
        printf("score : %d\n", m->score);
        fprintf(stdout, "  z  \n");
        fprintf(stdout, "q s d\n");
        ask_for_string(sizeof(movement), "Enter movement : ", movement);
        movement_code = toupper(movement[0]);
        dir = get_direction(movement_code);
    } while (!handle_move(m, dir));
}

void show_score_board() {
    if (!show_file_list("score")) {
        return;
    }

    char maze_name[MAX_MAZE_NAME_SIZE];
    char maze_score_path_name[MAX_FILE_PATH_SIZE];
    ask_for_string(MAX_MAZE_NAME_SIZE,
                   "Enter the name of the maze you want to open the score : ", maze_name);
    snprintf(maze_score_path_name, MAX_FILE_PATH_SIZE, "score/%s.score", maze_name);
    FILE *score_file = fopen(maze_score_path_name, "r");
    while (score_file == NULL) {
        fprintf(stderr, "File doesn't exist please try again\n");
        ask_for_string(MAX_MAZE_NAME_SIZE,
                       "Enter the name of the maze you want to open the score : ", maze_name);
        snprintf(maze_score_path_name, MAX_FILE_PATH_SIZE, "score/%s.score", maze_name);
        score_file = fopen(maze_score_path_name, "r");
    }
    printf("\n");
    char c;
    while ((c = fgetc(score_file)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
}

int handle_move(maze *m, direction dir) {
    if (dir == ERROR) {
        fprintf(stderr, "Please enter a valid position\n");
        return 0;
    }

    position player_pos = get_player_position(*m);
    if (player_pos.row == -1 || player_pos.column == -1) {
        fprintf(stderr, "Position of player can not be found\n");
        return 0;
    }
    tile *player_tile = get_tile(*m, player_pos);

    position next_pos = get_position_after_move(player_pos, dir);
    tile *next_tile = get_tile(*m, next_pos);
    if (*next_tile == WALL) {
        fprintf(stderr, "Wall in the way\n");
        return 0;
    }

    if (*next_tile == EMPTY || *next_tile == KEY || *next_tile == TREASURE || *next_tile == TRAP) {
        if (*next_tile == KEY) {
            *get_door_tile(*m) = EXIT;
        }
        if (*next_tile == TREASURE) {
            m->score += TREASURE_BONUS;
        }
        if (*next_tile == TRAP) {
            m->score -= TRAP_MALUS;
        }

        m->score -= SCORE_LOSS;
        *player_tile = EMPTY;
        *next_tile = PLAYER;
        return 0;
    }

    if (*next_tile == EXIT) {
        printf("YOU WON !\n");
        printf("Your score : %d\n", m->score);
        if (strcmp(m->maze_name, "") == 0) {
            return 1;
        }

        handle_score(*m);
        file_maze_load(m);
        return 1;
    }
    return 0;
}
