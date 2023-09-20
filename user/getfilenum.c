#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int c;
  int i;

  if(argc < 2){
    fprintf(2, "usage: getfilenum pid...\n");
    exit(1);
  }
  for(i = 1; i < argc; i++){
    c = getfilenum(atoi(argv[i]));
    if(c == -1){
      fprintf(2, "invalid pid\n");
      exit(1);
    }
    printf("%d\n", c);
  }
  exit(0);
}