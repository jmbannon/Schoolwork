/*
 ====================================================================
 Author:    Jesse Bannon
 Date:      06/01/15
 Class:     TCSS 342: Data Structures
 School:    University of Washington Tacoma
 Desc:      Uses simple recursive graph algorithm to generate a
            random maze.
 Copyright: Use for educational purposes only.
 ====================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "maze.h"
#define get_idx(row, col) ((row) * ((wid)+1) + (col))
#define get_len(idx) ((idx) / ((wid) + 1))
#define get_wid(idx) ((idx) % ((wid) + 1))

/* Maze dependant variables */
static unsigned int len;
static unsigned int wid;
static unsigned int maze_size;
static char * maze;
static char wall = 88;
static int debug;

/* Returns whether the string is a positive integer */ 
int is_digit(const char * str
) {
    long int ret = strtol(str, NULL, 10);
    if (ret <= 0)
        return 0;
    else
        return (int)ret;
}

/* Sets a blank board of the given dimensions to all walls */                   
static void set_blank_board(
) {
    unsigned int i, j;
    maze_size = 1 + len * (wid + 1);
    maze = malloc(maze_size * sizeof(char));

    for (i = 0; i < len; i++) {
        for (j = 0; j < wid; j++) {
            if (i % 2 == 0 || i == len-1 || j % 2 == 0 || j == wid-1)
                maze[get_idx(i, j)] = wall;
            else
                maze[get_idx(i, j)] = ' ';
        }
        maze[get_idx(i, wid)] = '\n';
    }

    maze[len * (wid+1)] = '\0';
    maze[get_idx(1, 0)] = ' ';
    maze[get_idx(len-2, wid-1)] = ' ';
}

/* Checks the maze at the given position and sees which of the four directions
 * it can move to. Then chooses randomly between the possible directions and
 * tears the wall down between those two positions.
 *
 * A position is valid if
 * 1) The path has not traveled to the position
 * 2) It is within the bounds of the board
 */
static unsigned traverse(unsigned int idx
) {
    unsigned int routes[6];
    unsigned int route_count = 0;
    unsigned int pos_len = get_len(idx);
    unsigned int pos_wid = get_wid(idx);
    
    unsigned int north_idx = get_idx(pos_len - 1, pos_wid);
    unsigned int north_pos = get_idx(pos_len - 2, pos_wid);

    unsigned int east_idx = get_idx(pos_len, pos_wid + 1);
    unsigned int east_pos = get_idx(pos_len, pos_wid + 2);

    unsigned int west_idx = get_idx(pos_len, pos_wid - 1);
    unsigned int west_pos = get_idx(pos_len, pos_wid - 2);

    unsigned int south_idx = get_idx(pos_len + 1, pos_wid);
    unsigned int south_pos = get_idx(pos_len + 2, pos_wid);

    if (pos_len != 1 && maze[north_idx] == wall && maze[north_pos] != '.') {
        routes[route_count++] = north_idx;
        routes[route_count++] = north_pos;
    }
    if (pos_wid != wid-2 && maze[east_idx] == wall && maze[east_pos] != '.') {
        routes[route_count++] = east_idx;
        routes[route_count++] = east_pos;
    }
    if (pos_len != len-2 && maze[south_idx] == wall && maze[south_pos] != '.') {
        routes[route_count++] = south_idx;
        routes[route_count++] = south_pos;
    }
    if (pos_wid != 1 && maze[west_idx] == wall && maze[west_pos] != '.') {
        routes[route_count++] = west_idx;
        routes[route_count++] = west_pos;
    }

    if (route_count == 0)
        return 0;

    unsigned int rand_idx = rand() % route_count/2;
    maze[routes[rand_idx*2]] = ' ';
    maze[routes[rand_idx*2+1]] = '.';
    return routes[rand_idx*2+1];
}

/* Returns the path from start to finish. Used when the path reaches the end
 * position.
 */
static int * get_path(unsigned int * route_stack,
                      unsigned int stack_size
) {
    unsigned int i, * path;
    path = malloc(stack_size * sizeof(unsigned int));
    for (i = 0; i < stack_size; i++)
        path[i] = route_stack[i];
    return path;
}

/* Traverses the maze until every possible position is filled. Uses a recursive
 * algorithm and a stack to generate a path. If there are no possible positions
 * it reverts back to the previos position in the stack and sees if a possible
 * path is present. Repeats this until the stack is entirely empty; meaning the
 * entire maze board has been traversed.
 */
static void traverse_maze(
) {
    unsigned int i;

    maze[get_idx(1, 1)] = '.';
    unsigned int end_pos = get_idx(len-2, wid-2);

    unsigned int * route_stack = malloc(len * wid * sizeof(unsigned int));
    unsigned int * path = NULL;
    unsigned int path_size, stack_size = 0;    
    route_stack[stack_size++] = get_idx(1, 1);

    while (stack_size > 0) {
        route_stack[stack_size] = traverse(route_stack[stack_size-1]);
        if (route_stack[stack_size] != 0) {
            ++stack_size;
            if (route_stack[stack_size-1] == end_pos && path == NULL) {
                path = get_path(route_stack, stack_size);
                path_size = stack_size;
            }
            if (debug)
                printf("%s\n", maze);
        } else
            route_stack[stack_size] = route_stack[--stack_size];
    }
    if (debug)
        for (i = 0; i < path_size; i++)
            maze[path[i]] = '\'';
    else
        for (i = 0; i < maze_size; i++)
            if (maze[i] == '.')
                maze[i] = ' ';
}

/* Generates a maze of the given dimensions. Optional debugging mode. */
void generate_maze(unsigned int length,
                   unsigned int width,
                   int debug_mode
) {
    srand(time(NULL));
    len = length % 2 == 0 ? length + 1 : length;
    wid = width % 2 == 0 ? width + 1 : width;
    debug = debug_mode;
    set_blank_board();
    traverse_maze();
    printf("%s", maze);
}
