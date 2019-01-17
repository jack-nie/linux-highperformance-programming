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

  int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
  assert(ret != -1);

  ret = listen(sock, 1);
  assert(ret != -1);

  sleep(20);

  while (true)
  {
    struct sockaddr_in client;
    socklen_t client_addr_length = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client, &client_addr_length);
    if (connfd < 0)
    {
      printf("connect errpr: %d", errno);
    }
  }

  close(sock);
  return 1;
}
