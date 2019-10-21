#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int ret, retime, rutime, stime;
  if(fork() == 0) {
    int i;
    for(i=0; i < 100; i++) {
      sleep(1);
    }
    exit();
  } else {
    ret = wait2(&retime, &rutime, &stime);
    printf(1, "retime: %d\nrutime: %d\nstime: %d\nreturn: %d\n", retime, rutime, stime, ret);
    exit();
  }
}
