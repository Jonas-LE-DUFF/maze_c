#include "monsters.h"

void move_monster(maze *m, monster *monst, 
    int (condition)(maze m, position pos), 
    void (behavior)(monster *monster, maze m, position pos)
){
    position nextpos;

    // if monster next to player, monster attacks him
    position player_pos = get_player_position(*m);
    if (get_distance(player_pos, monst->monster_pos) == 1) {
        nextpos = player_pos;
    } else {
        direction possible_directions[] = {UP, RIGHT, DOWN, LEFT};
        shuffle(possible_directions, 4, sizeof(direction));

        int i = 0;
        do {
            nextpos = get_position_after_move(monst->monster_pos, possible_directions[i]);
        } while (
            i++ < 4 && ( 
             !is_in_perimeter(nextpos, monst->center, monst->radius) 
            || is_border(*m, nextpos)
            || is_occupied(*m, nextpos)
            || condition(*m, nextpos))
        );
        if(i > 4){
            // every direction tried... aborting
            fprintf(stderr, "monster cannot move...\n");
            nextpos = monst->monster_pos;
        }
    }
    monst->monster_pos = nextpos;

    behavior(monst, *m, nextpos);

    // checking if player and monster encountered
    if (pos_equals(get_player_position(*m), monst->monster_pos)) {
        m->score -= monst->penality_points;
        remove_monster(m, *monst);
    }
}

// using __attribute__((unused)) to silence warnings
// cannot remove arguments as they are needed as the callback
int ghost_condition(__attribute__((unused)) maze m, __attribute__((unused)) position pos){
    return 0;
}

void ghost_behavior(__attribute__((unused)) monster *monster, __attribute__((unused)) maze m, __attribute__((unused)) position pos){
    // does nothing special
}

void orc_behavior(monster *monster, maze m, position pos){
    // checking if there's a treasure nearby...
    // if so the orc stays around it
    position closest_treasure = get_closest_treasure(m, pos);

    if(get_distance(closest_treasure, pos) <= ORC_DETECTION_RADIUS){
        monster->center = closest_treasure;
        monster->radius = ORC_PATROL_RADIUS;
    }
}

void move_ghost(maze *m, monster *monster) {
    move_monster(m, monster, ghost_condition, ghost_behavior);
}

void move_orc(maze *m, monster *monster) {
    move_monster(m, monster, is_wall_at, orc_behavior);
}

void remove_monster(maze *m, monster monster) {
    int removed = 0;
    for (int i = 0; i < m->monster_amount; i++) {
        if (pos_equals(monster.monster_pos, m->monsters[i]->monster_pos)) {
            free(m->monsters[i]);
            m->monsters[i] = NULL;
            removed = 1;
        }
        if (removed) {
            m->monsters[i] = m->monsters[i + 1];
        }
    }
    m->monster_amount--;
}

void init_monsters(maze *m){
    m->monster_amount = m->height * m->width / MONSTER_FREQUENCY;
    m->monsters = malloc(sizeof(*m->monsters) * m->monster_amount);

    for (int i = 0; i < m->monster_amount; i++)
    {   
        monster_type type = i%2;
        position monster_pos = {(rand() % (m->height-2)) + 1, (rand() % (m->width-2)) + 1};
        m->monsters[i] = malloc(sizeof(monster));
        if(type == GHOST){
            *m->monsters[i] = (monster){type, monster_pos, monster_pos, GHOST_RADIUS, GHOST_PENALITY_POINTS, move_ghost};
        }else{
            *m->monsters[i] = (monster){type, monster_pos, monster_pos, ORC_RADIUS  , ORC_PENALITY_POINTS,   move_orc};
        }
    }
}