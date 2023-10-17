#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

int
main(int argc, char **argv)
{
  struct pstat ps;
  settickets(2); // test settickets
  getpinfo(&ps);
  // iterate over pstat struct
  int i;
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