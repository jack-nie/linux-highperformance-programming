#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

int
main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    printf("usage: %s ip_address port_number send_buffer_size.\n", argv[0]);
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

  int sendbuf = atoi(argv[3]);
  int len = sizeof(sendbuf);
  setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
  getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
  printf("the tcp send buffer after set is %d", sendbuf);

  if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0)
  {
    printf("connect failed\n");
    return -1;
  }
  else
  {
    char buf[BUFSIZE];
    memset(buf, 'a', BUFSIZE);
    send(sock, buf, BUFSIZE, 0);
  }
  close(sock);
  return 0;
}
