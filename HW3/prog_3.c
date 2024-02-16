#define MAX 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// return 1 if filename is a directory
int is_dir(char *filename)
{
    struct stat path_stat;
    stat(filename, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// return 1 if filename is a regular file
int is_reg_file(char *filename)
{
    struct stat path_stat;
    stat(filename, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// return 1 if filename has a .txt extension
int is_txtfile(char *filename)
{
    char *ext = strrchr(filename, '.');
    if (ext == NULL)
    {
        return 0;
    }
    return strcmp(ext, ".txt") == 0;
}

// return number of lines in file
int count_lines(char *filename)
{
    int count = 0;
    char ch;
    FILE *infile;

    infile = fopen(filename, "r");
    while ((ch = fgetc(infile)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    fclose(infile);

    return count;
}

int count_lines_in_dir(char *path)
{
    DIR *dir;
    struct dirent *entry;
    char new_path[MAX];
    int count = 0;

    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, entry->d_name);

        if (is_dir(new_path))
        {
            count += count_lines_in_dir(new_path);
        }
        else if (is_reg_file(new_path) && is_txtfile(new_path))
        {
            count += count_lines(new_path);
        }
    }
    closedir(dir);

    return count;
}

int main()
{

    char path[MAX];
    int count;

    printf("Enter a directory path: ");
    scanf("%s", path);

    if (!is_dir(path))
    {
        printf("Invalid directory path.\n");
        return 1;
    }

    count = count_lines_in_dir(path);
    printf("Total number of lines in %s: %d\n", path, count);

    return 0;
}
