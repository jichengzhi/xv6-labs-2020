#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char *msg = "time required";
        write(2, msg, strlen(msg));
    }

    int t = atoi(argv[1]);

    sleep(t);

    exit(0);
}
