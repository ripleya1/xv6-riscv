#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int c;
  c = settickets(atoi(argv[1]));
  if(c == -1){
    exit(1);
  }
  exit(0);
}