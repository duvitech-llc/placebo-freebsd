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

    pfh = pidfile_open("/var/run/placebo.pid", 0600, &otherpid);
    if (pfh == NULL)
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "Daemon already running, pid: %d.", otherpid);
            exit(EXIT_FAILURE);
        }
        fputs("Cannot open or create pidfile", stderr);
    }

    if (daemon(0, 0) == -1)
    {
        fputs("daemon() fails.", stderr);
        pidfile_remove(pfh);
        exit(EXIT_FAILURE);
    }

    pidfile_write(pfh);
   
    while (1)
    {
        struct timespec snooze, ignore;
        snooze.tv_nsec = 10000000L;
        snooze.tv_sec = 0;
        ignore = snooze;
        nanosleep(&snooze, &ignore);
    }
    pidfile_remove(pfh);
    return 0;
}
