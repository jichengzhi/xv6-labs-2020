#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define DEBUG

char *concat(char *a, char *b)
{
    char *s = malloc(512);

    int alen = strlen(a);

    strcpy(s, a);

    s[alen] = '/';

    strcpy(s + alen + 1, b);

    return s;
}

char *simpleFileName(char *path)
{
    int len = strlen(path);

    char *s = path + len - 1;

    while (s > path && *(s - 1) != '/')
    {
        s--;
    }

    return s;
}

void find(char *tar, char *path)
{
    struct stat st;

    if (stat(path, &st) < 0)
    {
        fprintf(2, "cannot stat %s\n", path);
        return;
    }

    if (st.type == T_FILE)
    {
        char *cur = simpleFileName(path);
        if (strcmp(cur, tar) == 0)
        {
            printf("%s\n", path);
        }
    }
    else if (st.type == T_DIR)
    {
        int fd;
        struct dirent de;

        if ((fd = open(path, 0)) < 0)
        {
            fprintf(2, "cannot open %s\n", path);
            return;
        }

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
            {
                continue;
            }

            char *curPath = concat(path, de.name);

            find(tar, curPath);

            free(curPath);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "path and tar file name required");
        exit(1);
    }

    find(argv[2], argv[1]);

    exit(0);
}