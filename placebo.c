#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <libutil.h>

#define _countof(arg) ( (sizeof arg) / (sizeof arg[0]) )

static void control(int sig)
{
    static const char *labels[] =
    {
        "-", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGEMT", "SIGFPE", "SIGKILL", "SIGBUS", "SIGSEGV", "SIGSYS", "SIGPIPE", "SIGALRM", "SIGTERM"
    };
    if (sig > 0 && sig < (int)_countof(labels))
        fprintf(stderr, "Something bad %s happened.\n", labels[sig]);
    else
        fprintf(stderr, "Something bad %d happened.\n", sig);
    exit(sig);
}


int main()
{
    struct pidfh *pfh = NULL;
    pid_t otherpid = 0;

    signal(SIGABRT, &control);
    signal(SIGALRM, &control);
    signal(SIGILL, &control);
    signal(SIGFPE, &control);
    signal(SIGHUP, &control);
    signal(SIGINFO, &control);
    signal(SIGINT, &control);


    printf("finished registering sigs\n");
    pfh = pidfile_open("/var/run/placebo.pid", 0600, &otherpid);
    if (pfh == NULL)
    {
        if (errno == EEXIST)
        {
	    printf("Daemon already running, pid: %d\n", otherpid);
            fprintf(stderr, "Daemon already running, pid: %d.", otherpid);
            exit(EXIT_FAILURE);
        }
	printf("Cannot open or create pidfile\n");
        fputs("Cannot open or create pidfile", stderr);
    }
    
    printf("pidfile opened\n");


    if (daemon(0, 0) == -1)
    {
        printf("daemon fails\n");
        fputs("daemon() fails.", stderr);
        pidfile_remove(pfh);
        exit(EXIT_FAILURE);

    }

    printf("writing to pid file\n");
    pidfile_write(pfh);
  
    printf("starting endless loop\n"); 
    while (1)
    {
        printf("tick\n");
	sleep(2);
    }
    printf("exit service\n");
    pidfile_remove(pfh);
    return 0;
}
