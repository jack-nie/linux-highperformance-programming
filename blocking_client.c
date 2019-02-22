#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define SEND_DATA "Hello World"

int
main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("usage: %s ip_address port_number.\n", argv[1]);
    return 1;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  assert(sock > 0);

  int ret = connect(sock, (struct sockaddr_in*)&address, sizeof(address));
  assert(ret != -1);

  int count = 0;

  while(true)
  {
    int ret = send(sock, SEND_DATA, strlen(SEND_DATA), 0);
    if (ret != strlen(SEND_DATA))
    {
      printf("send data error, error: %d\n", errno);
      break;
    }
    else
    {
      count++;
      printf("send data successfully, count = %d\n", count);
    }

  }
  close(sock);
  return 0;
}
