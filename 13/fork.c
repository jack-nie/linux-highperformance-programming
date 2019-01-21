#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char* argv)
{
  pid_t p = fork();
  if (p == 0) {
    printf("return from child\n");
  }
  else
  {
    printf("return from parent, child pid is %d\n", p);
  }
}
