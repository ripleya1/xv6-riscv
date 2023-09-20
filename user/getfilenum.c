#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int i;
  int c;

  if(argc < 2){
    fprintf(2, "usage: getnumfiles pid...\n");
    exit(1);
  }
  for(i=1; i<argc; i++){
    c = getfilenum(atoi(argv[i]))
    fprintf(2, c);
  }
  exit(0);
}