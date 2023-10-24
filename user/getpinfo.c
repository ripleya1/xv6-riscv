#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include "kernel/spinlock.h"

void printStuff(struct pstat ps);
#define NUMPROCS 6

int
main(int argc, char **argv)
{
  struct pstat ps;
  int pid, i, k, j;
  int proc1Prev, proc2Prev, proc3Prev, proc4Prev, proc5Prev, proc6Prev;
  proc1Prev = proc2Prev = proc3Prev = proc4Prev = proc5Prev = proc6Prev = 0;
  // fork 6 times
  for(k = 1; k < (NUMPROCS + 1); k++){
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
      settickets(10 * k);
      // do work
      for(i = 0; i < 100000000000; i++){
        // print every so often
        if(k == 1 && i % 100000 == 0){
          wait(0);
          getpinfo(&ps);
          printStuff(ps);
          // if(argc > 2 && strcmp(argv[2], "-c")){
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
          // }
        }
      }

      exit(0);
    }
  }
  // wait on everything to finish
  for(i = 1; i < (NUMPROCS + 1); i++){
    wait(0);
  }
  exit(0);
}

void printStuff(struct pstat ps){
  int j;
  for(j = 0; j < NPROC; j++){
    if(ps.tickets[j] > 0){ // only print proc if it might be used ie has tickets
    // if(ps.inuse[j] == 1){ // only print proc if it is in use
      printf("PID: %d In use?: %d Tickets: %d Ticks: %d\n", 
        ps.pid[j],
        ps.inuse[j],
        ps.tickets[j],
        ps.ticks[j]
      );
    }
  }
}