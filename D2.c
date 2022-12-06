#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int readFolderSize(int *totalSize, char *folder)
{
    char fullPath[256];
    struct dirent *dirData;
    struct stat buffer;
    int exists;
    DIR *poDir;
    int resp = EXIT_SUCCESS;

    poDir = opendir(folder);

    if (poDir == NULL)
    {
        perror(folder);

        return EXIT_FAILURE;
    }

    while ((dirData = readdir(poDir)))
    {
        
        if (dirData->d_type == DT_DIR)
        {
            if (dirData->d_name[0] != '.')
            {

                strcpy(fullPath, folder);
                strcat(fullPath, "/");
                strcat(fullPath, dirData->d_name);


                if (readFolderSize(totalSize, fullPath) == EXIT_FAILURE)
                    resp = EXIT_FAILURE;
            }
        }
        else
        {
            strcpy(fullPath, folder);
            strcat(fullPath, "/");
            strcat(fullPath, dirData->d_name);

            exists = stat(fullPath, &buffer);

            if (exists < 0)
            {
                const unsigned int err = errno;

                resp = EXIT_FAILURE;

                continue;
            }
            else
            {
                (*totalSize) += buffer.st_size;

           
            }
        }
    }

    closedir(poDir);

    return resp;
}

int main(int argc, char *argv[])
{
    int folderSize=0;
    struct stat sb;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // if (lstat(argv[1], &sb) == -1)
    // {
    //     perror("lstat");
    //     exit(EXIT_FAILURE);
    // }

    // if (general_getFolderSize(argv[1], &folderSize) == EXIT_FAILURE)
    // {
    //     printf("Error reading folder size!");
    // }
    printf("general_getFolderSize: Start\n");

    //
    if (readFolderSize(&folderSize, argv[1]) == EXIT_FAILURE)
    {
        perror("general_getFolderSize: Call to readFolderSize failed!");

        return EXIT_FAILURE;
    }

    //
    printf("general_getFolderSize: Stop\n");
    printf("Folder size: %d b (%lf kb / %lf Mb)\n", folderSize, folderSize / 1024.0, folderSize / (1024 * 1024.0));
    return EXIT_SUCCESS;
}