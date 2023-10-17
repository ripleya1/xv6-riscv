#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int c;
  int i;

  if(argc < 2){
    fprintf(2, "usage: settickets numtickets...\n");
    exit(1);
  }
  for(i = 1; i < argc; i++){
    c = settickets(atoi(argv[i]));
    if(c == -1){
      fprintf(2, "invalid\n");
      exit(1);
    }
    printf("Tickets: %d\n", atoi(argv[i])); // print the number of tickets
  }
  exit(0);
}