#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include "kernel/spinlock.h"

int
main(int argc, char **argv)
{
  struct pstat ps;
  // TODO: problem 1: there are other procs (PIDs 1 and 2)
  // that take up at least 1 ticket each so how does that math work
  // problem 2: printing (and presumably everything else) 
  // gets screwy if I do the test this way
  // problem 3: actually it's already screwy because with just one 
  // settickets the proc with 2 tickets consistently has less ticks
  settickets(2); // test settickets
  int pid, i;
  pid = fork();
  // parent
  if(pid < 0){
    // same as panic()
    char * s;
    s = "aaa";
    fprintf(2, "%s\n", s);
    exit(1);
  }
  /*
  for()
  fork
  if pid == 0
  settickets(10*count)
  */
  // child
  else if(pid == 0){
    int j;
    settickets(4);
    for(i = 0; i < 1000000; i++){
      if(i % 1000 == 0){
        getpinfo(&ps);
        for(j = 0; j < NPROC; j++){
          if(ps.tickets[j] > 0){ // only print proc if it might be used ie has tickets
            printf("PID: %d In use?: %d Tickets: %d Ticks: %d\n", 
              ps.pid[j],
              ps.inuse[j],
              ps.tickets[j],
              ps.ticks[j]
            );
          }
        }
      }
    }
    exit(0);
  }
  // settickets(3);
  // fork();
  wait(0);
  getpinfo(&ps);
  // iterate over pstat struct
  i = 0;
  // int i;
  // TODO: infinite for loop with sleep every ~5 secs
  for(i = 0; i < NPROC; i++){
    if(ps.tickets[i] > 0){ // only print proc if it might be used ie has tickets
      printf("PID: %d In use?: %d Tickets: %d Ticks: %d\n", 
        ps.pid[i],
        ps.inuse[i],
        ps.tickets[i],
        ps.ticks[i]
      );
    }
  }
  exit(0);
}