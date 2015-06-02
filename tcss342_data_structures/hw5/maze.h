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

#ifndef __MAZE_H__
#define __MAZE_H__

void generate_maze(unsigned int len,
                   unsigned int wid,
                   int debug_mode);

int is_digit(const char * str);

#endif

