#include <stdio.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  uid_t uid = getuid();
  uid_t euid = geteuid();
  printf("userid is %d, effective useris is %d\n", uid, euid);
  return 0;
}
