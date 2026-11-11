#include "frontend.h"

void show_menu() {
    printf("┏━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ 0 : Quit        ┃\n");
    printf("┃ 1 : Create Maze ┃\n");
    printf("┃ 2 : Load Maze   ┃\n");
    printf("┃ 3 : Play        ┃\n");
    printf("┃ 4 : Score Board ┃\n");
    printf("┗━━━━━━━━━━━━━━━━━┛\n");
}

void display_maze(maze m) {
    printf("\n");
    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            position pos = {i, j};
            monster* monster = get_monster_at_pos(&m, pos);
            if(monster != NULL){
                print_monster(*monster);
            } else {
                print_character(m, pos);
            }
        }
        printf("\n");
    }
}

void print_monster(monster monster){
    switch (monster.monster_type)
    {
    case GHOST:
        printf("⍝");
        break;
    case ORC:
        printf("⍥");
    default:
        break;
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
        printf("⍾");
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

/**
 * @brief Prints a wall character considering every wall in its surrounding
 * this way it gives a nice connected wall looked
 * 
 * @param m displayed maze
 * @param pos position of the wall being displayed
 */
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
    } else {
        printf("▪");
    }
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
    // checking if there're files at all
    while ((de = readdir(dr)) != NULL) {
        if (++n > 2) {
            break;
        }
    }
    if (n <= 2) { // there's only '.' and '..', considering empty
        fprintf(stderr, "Error : there're no %s, please create one and try again\n", list_name);
        closedir(dr);
        return 0;
    }
    rewinddir(dr);

    printf("\nList of available %s : \n", list_name);
    while ((de = readdir(dr)) != NULL){
        if (de->d_name[0] == '.'){
            continue;
        }
        // visually removing extension from the file
        char *last_dot = strrchr(de->d_name, '.');
        *last_dot = '\0';
        printf("%s\n", de->d_name);
    }
    closedir(dr);
    printf("\n");
    return 1;
}