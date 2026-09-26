#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

char *filetype(unsigned char type)
{
    char *str;
    
    switch(type)
    {
        case DT_BLK:     str = "block device"; break;
        case DT_CHR:     str = "character device"; break;
        case DT_DIR:     str = "directory"; break;
        case DT_FIFO:    str = "named pipe (FIFO)"; break;
        case DT_LNK:     str = "symbolic link"; break;
        case DT_REG:     str = "regular file"; break;
        case DT_SOCK:    str = "UNIX domain socket"; break;
        case DT_UNKNOWN: str = "unknown file type"; break;
        default:         str = "UNKNOWN";
    }

    return str;
}

void traverseDirectory(const char *path)
{
    struct dirent *dirent;
    DIR *parentDir;
    char fullPath[1024];

    parentDir = opendir(path);

    if (parentDir == NULL)
    {
        printf("Error opening directory '%s'\n", path);
        return;
    }

    while ((dirent = readdir(parentDir)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") == 0 ||
            strcmp(dirent->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(fullPath, sizeof(fullPath), "%s/%s",
                path, dirent->d_name);
        
        printf("%s (%s)\n",
               fullPath, filetype(dirent->d_type));
        
        if (dirent->d_type == DT_DIR)
        {
            traverseDirectory(fullPath);
        }
    }

    closedir(parentDir);

}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }

    traverseDirectory(argv[1]);

    return 0;
}