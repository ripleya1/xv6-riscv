#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include "kernel/spinlock.h"

void printPstat(struct pstat ps);
#define NUMPROCS 6 // number of procs we are forking (max 6 for printing the tick differences)

int
main(int argc, char **argv)
{
  struct pstat ps;
  int pid, i, k, j;
  int proc1Prev, proc2Prev, proc3Prev, proc4Prev, proc5Prev, proc6Prev;
  proc1Prev = proc2Prev = proc3Prev = proc4Prev = proc5Prev = proc6Prev = 0;
  // fork 6 times
  for(k = 0; k < NUMPROCS; k++){
    pid = fork();
    // parent case
    if(pid < 0){
      // basically panic
      char *s;
      s = "Parent forked";
      fprintf(2, "%s\n", s);
      exit(1);
    }
    // child case
    else if(pid == 0){
      settickets(10 * (k + 1)); // set tickets based on where we are in the loop
      // do work
      for(i = 0; i < 100000000000; i++){
        // print every so often, the first proc is the one doing the printing
        if(k == 0 && i % 100000 == 0){
          wait(0);
          getpinfo(&ps); // aggregate data
          printPstat(ps); // print data
          // print the differences between the current number of ticks and previous number of ticks for each of the forked procs 
          for(j = 0; j < NPROC; j++){
            if(ps.tickets[j] == 10){
              printf("Tickets: 10 Increased by: %d\n", (ps.ticks[j] - proc1Prev));
              proc1Prev = ps.ticks[j];
            }
            if(ps.tickets[j] == 20){
              printf("Tickets: 20 Increased by: %d\n", (ps.ticks[j] - proc2Prev));
              proc2Prev = ps.ticks[j];
            }
            if(ps.tickets[j] == 30){
              printf("Tickets: 30 Increased by: %d\n", (ps.ticks[j] - proc3Prev));
              proc3Prev = ps.ticks[j];
            }
            if(ps.tickets[j] == 40){
              printf("Tickets: 40 Increased by: %d\n", (ps.ticks[j] - proc4Prev));
              proc4Prev = ps.ticks[j];
            }
            if(ps.tickets[j] == 50){
              printf("Tickets: 50 Increased by: %d\n", (ps.ticks[j] - proc5Prev));
              proc5Prev = ps.ticks[j];
            }
            if(ps.tickets[j] == 60){
              printf("Tickets: 60 Increased by: %d\n", (ps.ticks[j] - proc6Prev));
              proc6Prev = ps.ticks[j];
            }
          }
          printf("\n"); // newline every iteration
        }
      }
      exit(0);
    }
  }

  // wait on everything to finish
  for(i = 0; i < NUMPROCS; i++){
    wait(0);
  }
  exit(0);
}

// prints a pstat struct
void printPstat(struct pstat ps){
  int i;
  for(i = 0; i < NPROC; i++){
    if(ps.tickets[i] > 0){ // only print proc if it might be used, ie it has tickets
      printf("PID: %d In use?: %d Tickets: %d Ticks: %d\n", 
        ps.pid[i],
        ps.inuse[i],
        ps.tickets[i],
        ps.ticks[i]
      );
    }
  }
}