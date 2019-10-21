#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        printf(1, "usage: %s n\n", argv[0]);
        exit();
    }
    int n = atoi(argv[1]);
    
    int i;
    for (i=0; i<3*n; i++) {
        int pid;
        if ((pid = fork()) < 0) {
            printf(2, "fork error\n");
            break;
        } else if (pid == 0) { // child
            volatile int j,k;
            volatile int t=0;
            pid = getpid();
            if (pid % 3 == 0) {
                for (j=0; j<100; j++) {
                    for (k=0; k<1000000; k++) {
                        t++;
                    }
                }
            } else if (pid % 3 == 1) {
                for (j=0; j<100; j++) {
                    for (k=0; k<1000000; k++) {
                        t++;
                    }
                    yield();
                }
            } else {
                for (j=0; j<100; j++) {
                    sleep(1);
                }
            }
            exit();
        }
    }

    int retime, rutime, stime, pid;
    const char *proc_types[] = {
        "CPU-Bound",
        "S-Bound",
        "IO-Bound"
    };

    int total_rutime[] = {0, 0, 0},
        total_retime[] = {0, 0, 0},
        total_stime[]  = {0, 0, 0};
    
    while (i--) {
        pid = wait2(&retime, &rutime, &stime);
        printf(1, "pid: %d - %s\nretime: %d\nrutime: %d\nstime: %d\n\n", pid, proc_types[pid%3], retime, rutime, stime);
        total_rutime[pid%3] += rutime;
        total_retime[pid%3] += retime;
        total_stime[pid%3] += stime;
    }

    int l;
    for (l=0; l<3; l++) {
        printf(1, "%s\nrutime mean: %d\nretime mean: %d\nstime mean: %d\n\n", proc_types[l], total_rutime[l]/n, total_retime[l]/n, total_stime[l]/n);
    }
    exit();
}
