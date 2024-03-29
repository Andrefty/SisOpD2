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
        perror("general_getFolderSize: poDir fail!");

        return EXIT_FAILURE;
    }

    while ((dirData = readdir(poDir)))
    {
        if (dirData == NULL)
        {
            const unsigned int err = errno;

            fprintf(stderr, "general_getFolderSize: Failed in readdir (%u)\n", err);

            resp = EXIT_FAILURE;

            continue;
        }

        //
        if (dirData->d_type == DT_DIR)
        {
            if (dirData->d_name[0] != '.')
            {
                printf("general_getFolderSize: Is a directory: %s\n", dirData->d_name);

                strcpy(fullPath, folder);
                strcat(fullPath, "/");
                strcat(fullPath, dirData->d_name);

                printf("general_getFolderSize: Accessing dir: %s\n", fullPath);

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

                // fprintf(stderr, "general_getFolderSize: Failed in stat (file) %s: %u\n", fullPath, err);

                resp = EXIT_FAILURE;

                continue;
            }
            else
            {
                (*totalSize) += buffer.st_size;

                // printf("general_getFolderSize: Item size (file) (%s): %d\n",fullPath,(int)buffer.st_size);
            }
        }
    }

    closedir(poDir);

    return resp;
}

/*!
 * \brief general_getFolderSize returns the size of a folder in bytes.
 * \param folder is the name of the folder (preferentially without a '/' at the end)
 * \param totalSize is a pointer where the result value is given
 * \return
 */
int general_getFolderSize(char *folder, int *totalSize)
{
    printf("general_getFolderSize: Start\n");

    //
    if (readFolderSize(totalSize, folder) == EXIT_FAILURE)
    {
        perror("general_getFolderSize: Call to readFolderSize failed!");

        return EXIT_FAILURE;
    }

    //
    printf("general_getFolderSize: Stop\n");

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int folderSize;
    struct stat sb;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (lstat(argv[1], &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if (general_getFolderSize(argv[1], &folderSize) == EXIT_FAILURE)
    {
        printf("Error reading folder size!");
    }
    else
    {
        printf("Folder size: %d b (%d kb / %d Mb)\n", folderSize, folderSize / 1024, folderSize / (1024 * 1024));
    }

    return EXIT_SUCCESS;
}