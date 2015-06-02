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
        len = isDigit(argv[2]);
        wid = isDigit(argv[3]);
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
