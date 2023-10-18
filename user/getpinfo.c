#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

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
  fork();
  // settickets(3);
  // fork();
  getpinfo(&ps);
  // iterate over pstat struct
  int i;
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