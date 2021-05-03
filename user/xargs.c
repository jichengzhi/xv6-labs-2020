#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

// #define DEBUG

int isNewline(char c)
{
    return c == '\n';
}

char **readExtraArgs(int *argc)
{
    int i = 0, sz = 1, tot = 512, n = 0;
    char buf[512];
    buf[0] = '\n';

    while((n = read(0, buf + sz, tot - sz)) > 0)
    {
        sz += n;
    }

    #ifdef DEBUG
    printf("read: '%s'\n", buf);
    #endif

    while(sz > 1 && isNewline(buf[sz - 1]))
    {
        sz --;
    }

    while(i + 1 < sz && isNewline(buf[i + 1]))
    {
        i ++;
    }

    char **args = malloc(sizeof(char*) * MAXARG);
    *argc = 0;

    for(int j = -1, cnt = 0; i < sz; i++)
    {
        if(isNewline(buf[i]))
        {
            args[j ++][cnt] = 0;
            args[j] = malloc(64);
            *argc = *argc + 1;
            cnt = 0;
        }
        else
        {
            args[j][cnt ++] = buf[i];
        }
    }

    #ifdef DEBUG
    for(int i = 0; i < *argc; i++)
    {
        printf("arg '%s'\n", args[i]);
    }
    #endif

    return args;
}

int
main(int argc, char *argv[])
{
    int extArgc = 0;
    char **extArgs = readExtraArgs(&extArgc);

    char **args = malloc(MAXARG * sizeof(char*));
    memmove(args, argv + 1, (argc - 1) * sizeof(char*));

    for(int i = 0; i < extArgc; i++)
    {
        if(fork() > 0)
        {
            // parent
            int childPid;
            wait(&childPid);
        }
        else
        {
            // child
            args[argc - 1] = extArgs[i];
            args[argc] = 0;

            exec(argv[1], args);
        }
    }

    free(args);
    free(extArgs);

    exit(0);
}