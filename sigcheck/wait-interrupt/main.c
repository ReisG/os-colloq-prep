#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handler(int s)
{
    static const char buf[] = "Hello, you have been SIGNALED\n";
    write(STDIN_FILENO, buf, sizeof(buf) - 1);
}

int
main(void)
{
    if (fork() == 0)
    {
        sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART, .sa_handler=SIG_IGN}, NULL);
        while(1);
        printf("cycle terminated\n");
        return 0;
    }

    sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART, .sa_handler=handler}, NULL);

    wait(NULL);
    printf("HAHA");
}