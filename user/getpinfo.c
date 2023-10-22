#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"
#include "kernel/spinlock.h"

void printStuff(struct pstat ps);

int
main(int argc, char **argv)
{
  struct pstat ps;
  int pid, i, k;
  // fork 6 times
  for(k = 0; k < 6; k++){
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
      for(i = 0; i < 1000000; i++){
        // print every so often
        // if(i == (1000000 - 1)){
        //   wait(0);
        //   getpinfo(&ps);
        //   printStuff(ps);
        // }
      }
      // wait(0);
      // getpinfo(&ps);
      // printStuff(ps);
      exit(0);
    }
    wait(0);
    getpinfo(&ps);
    printStuff(ps);
  }
  // wait(0);
  // getpinfo(&ps);
  // printStuff(ps);
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