#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PIPE_READ_END(p) (p[1])
#define PIPE_WRITE_END(p) (p[0])

int writeByte(int fd)
{
    char payload[] = {'h'};
    return write(fd, payload, sizeof(payload));
}

void closePipe(int *p)
{
    close(p[0]);
    close(p[1]);
}

int main(int argc, char *argv[])
{
    int ping[2], pong[2];

    pipe(ping);
    pipe(pong);

    int pid = fork();
    char buffer[32];

    if (pid > 0)
    {
        // parent
        pid = getpid();
        int childPid;

        writeByte(PIPE_WRITE_END(ping));
        closePipe(ping);

        wait(&childPid);

        // read byte from child through pong
        read(PIPE_READ_END(pong), buffer, sizeof(buffer));
        printf("%d: received pong\n", pid);
        closePipe(pong);

        exit(0);
    }
    else
    {
        // child
        pid = getpid();

        read(PIPE_READ_END(ping), buffer, sizeof(buffer));
        printf("%d: received ping\n", pid);
        closePipe(ping);

        writeByte(PIPE_WRITE_END(pong));
        closePipe(pong);

        exit(0);
    }
}
