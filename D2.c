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
        printf("Usage: %s <directory_path>", argv[0]);
        exit(1);
    }

    dirpath = argv[1];

    if ((dir = opendir(dirpath)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while ((direntry = readdir(dir)) != NULL)
    {
        char *filepath = malloc(strlen(dirpath) + strlen(direntry->d_name) + 2);
        strcpy(filepath, dirpath);
        strcat(filepath, "/");
        strcat(filepath, direntry->d_name);

        if (stat(filepath, &buf) < 0)
        {
            perror("stat");
            exit(1);
        }
        if (S_ISDIR(buf.st_mode) && strcmp(direntry->d_name, ".") == 0)
        {
            size+=buf.st_size;
        }
        if (S_ISREG(buf.st_mode))
        {
            size += buf.st_size;
        }
        else if (S_ISDIR(buf.st_mode) && strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0)
        {
            size += calcul(2, (char *[]){"", filepath});
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
    long int folderSize=0;
    folderSize=calcul(argc, argv);
    printf("Folder size: %ld b (%lf kb / %lf Mb)\n", folderSize, folderSize / 1024.0, folderSize / (1024 * 1024.0));
}

// // Path: D2dela1.c
// // Program care primeste ca argument in linia de comanda un
// //  director si calculeaza suma dimensiunilor fisierelor din arborescenta cu
// //  originea in el.

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <string.h>

// int main(int argc, char *argv[])
// {
//     DIR *dir;
//     struct dirent *direntry;
//     struct stat buf;
//     char *dirpath;
//     int size = 0;

//     if (argc != 2)
//     {
//         printf("Usage: %