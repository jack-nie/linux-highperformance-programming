#include <stdio.h>

int
main(int argc, char *argv[])
{
  int i = 0;
  int const *p = &i;
  // read only variable is not assignable.
  // *p = 3;
  int * const  q = &i;
  *q = 3;
  // q = NULL;
}
