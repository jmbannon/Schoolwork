#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char * concat(const char * str1,
              const char * str2)
{
    char * file_path = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(file_path, str1);
    strcat(file_path, str2);
    return file_path;
}

void print_directory(const char * path,
                     unsigned long long * parent_sum)
{
    DIR * dir_p = opendir(path);
    if (dir_p == NULL)
    {
        printf("Error: Invalid directory %s\n", path);
        return;
    }

    struct dirent * dir_element;
    struct stat file_info;
    unsigned long long local_sum = 0;
    unsigned int iterator = 0;

    while ((dir_element = readdir(dir_p)))
    {
        // Skips parent directory
        if (iterator++ == 1) continue;

        char * file_name = concat(path, dir_element->d_name);
        lstat(file_name, &file_info);

        if (iterator >= 2 
                && S_ISDIR(file_info.st_mode)
                && !S_ISLNK(file_info.st_mode))
        {
            char * dir_name = concat(file_name, "/");
            print_directory(dir_name, &local_sum);
            free(dir_name);
            continue;
        }

        *parent_sum += file_info.st_size;
        local_sum += file_info.st_size;

        // If it's not the current or parent directory
        if (iterator >= 2)
            printf("%12llu %s%s\n", file_info.st_size,
                                    path,
                                    dir_element->d_name);
        free(file_name);
    }
    printf("%12llu %s\n", local_sum, path);
    closedir(dir_p);
}

int main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <path>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    unsigned long long dir_sum = 0;
    print_directory(argv[1], &dir_sum);
    return EXIT_SUCCESS;
}
