#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int c;

  if(argc < 2){
    fprintf(2, "usage: getnumfiles pid...\n");
    exit(1);
  }
  // fprintf(2, (char*)getfilenum(atoi(argv[1])));
  c = getfilenum(atoi(argv[0]));
  if(c == -1){
    fprintf(2, "invalid pid\n");
    exit(1);
  }
  printf("%d\n", c);
  exit(0);
}