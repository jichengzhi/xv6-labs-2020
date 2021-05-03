#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PIPE_READ_END(p) (p[1])
#define PIPE_WRITE_END(p) (p[0])

#define NUMBER_SIZE(list) (list[0])
#define NO_MORE_NUMBERS(list) (NUMBER_SIZE(list) == 1)

// Protocal: <tot> <num> ... <num>

const int N = 37;

void printList(int *list)
{
    #ifdef DEBUG
    for(int i = 0; i < N; i++)
    {
        printf("%d ", list[i]);
    }
    printf("\n");
    #endif
}

void writeNumbers(int fd, int *list)
{
    write(fd, list, sizeof(int) * N);
}

void readNumbers(int fd, int *list)
{
    char buffer[sizeof(int) * N];
    int n = 0, nxt = 0;
    char *tar = (char *)list;

    while ((n = read(fd, buffer, sizeof(buffer)) > 0))
    {
        for(int i = 0; i < n; i++){
            tar[nxt ++] = buffer[i];
        }
    }
}

void filter(int *list)
{
    int n = NUMBER_SIZE(list), x = list[1];
    NUMBER_SIZE(list) = 1;

    for (int i = 2, j = 1; i < n; i++)
    {
        if (list[i] % x != 0)
        {
            list[j++] = list[i];
            NUMBER_SIZE(list)
            ++;
        }
    }
}

void closePipe(int *p)
{
    close(p[0]);
    close(p[1]);
}

void initList(int *list)
{
    NUMBER_SIZE(list) = 1;
    for (int i = 1, j = 2; j <= 35; i++, j++)
    {
        list[i] = j;
        NUMBER_SIZE(list) ++;
    }
}

void printPrime(int *list)
{
    if(NO_MORE_NUMBERS(list))
    {
        return;
    }

    printf("prime %d\n", list[1]);
    filter(list);

    int p[2];
    pipe(p);

    if(fork() > 0)
    {
        writeNumbers(PIPE_WRITE_END(p), list);
        closePipe(p);

        int childPid;
        wait(&childPid);
    }else
    {
        readNumbers(PIPE_READ_END(p), list);
        closePipe(p);

        printPrime(list);
    }

    exit(0);
}

int main(int argc, char *argv[])
{

    int list[N];
    initList(list);

    printPrime(list);

    exit(0);
}
