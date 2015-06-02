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
#include "maze.h"

void print_commands(
) {
    printf("      ./Maze p <len> <wid>   Prints maze\n");
    printf("      ./Maze d <len> <wid>   Debugs maze and prints creation\n");
}

int main(int argc,
         char * argv[]
) {
    int len = 0, wid = 0;
    if (argc != 4)
        print_commands();
    else {
        len = is_digit(argv[2]);
        wid = is_digit(argv[3]);
        if (len != 0 && wid != 0)
        {
            if (argv[1][0] == 'p')
                generate_maze(len, wid, 0);
            else if (argv[1][0] == 'd')
                generate_maze(len, wid, 1);
            else
                print_commands();
        }
        else
            print_commands();
    }
    return 0;
}
