#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

int
main(struct pstat* ps)
{
    fprintf(2, "PID         In use?         Tickets         Ticks"); // TODO: figure out how to align later
  // iterate over pstat struct
  for(int i = 0; i < NPROC; i++){
    printf("%d          %d          %d          %d", 
        ps->pid[i],
        ps->inuse[i],
        ps->tickets[i],
        ps->ticks[i]
    );
  }
  exit(0);
}