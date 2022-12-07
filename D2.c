// Program care primeste ca argument in linia de comanda un
//  director si calculeaza suma dimensiunilor fisierelor din arborescenta cu
//  originea in el.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int calcul(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *direntry;
    struct stat buf;
    char *dirpath;
    int size = 0;

    if (argc != 2)
    {
        printf("Usage: %s <cale_director>", argv[0]);
        exit(1);
    }

    dirpath = argv[1];
    // remove the last slash if it exists
    if (dirpath[strlen(dirpath) - 1] == '/')
        dirpath[strlen(dirpath) - 1] = '\0';
    if ((dir = opendir(dirpath)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while ((direntry = readdir(dir)) != NULL)
    {
        char *filepath = malloc(strlen(dirpath) + strlen(direntry->d_name) + 2);
        if (filepath == NULL)
        {
            perror("malloc");
            closedir(dir); 
            exit(1);
        }
        strcpy(filepath, dirpath);
        strcat(filepath, "/");
        strcat(filepath, direntry->d_name);

        if (stat(filepath, &buf) < 0)
        {
            perror("stat");
            free(filepath);
            closedir(dir);
            exit(1);
        }
        if (S_ISDIR(buf.st_mode) && strcmp(direntry->d_name, ".") == 0)
        {
            size += buf.st_size;
        }
        if (S_ISREG(buf.st_mode))
        {
            size += buf.st_size;
        }
        else if (S_ISDIR(buf.st_mode) && strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0)
        {
            size += calcul(2, (char *[]){"", filepath, NULL});
        }
    }

    if (closedir(dir) < 0)
    {
        perror("closedir");
        exit(1);
    }
    return size;
}

int main(int argc, char *argv[])
{
    long int folderSize = 0;
    if (argc != 2)
    {
        fprintf(stderr,"Usage: %s <cale_director>", argv[0]);
        exit(1);
    }
    if (strcmp(argv[1],"")==0)
    {
        fprintf(stderr,"Sir %s vid\n", argv[1]);
        exit(1);
    }
    folderSize = calcul(argc, argv);
    printf("Marime folder: %ld b (%lf kb / %lf Mb)\n", folderSize, folderSize / 1024.0, folderSize / (1024 * 1024.0));
}