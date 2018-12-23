#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
  struct in_addr addr;
  inet_aton("1.2.3.4", &addr);
  char* szValue1 = inet_ntoa(addr);
  printf("address 1: %s", szValue1);
  return(0);
}
