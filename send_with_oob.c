#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    printf("usage: %s ip_address port_number.\n", argv[0]);
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

  if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0)
  {
    printf("connect failed\n");
    return -1;
  }
  else
  {
    const char *oob_data = "abc";
    const char *normal_data = "123";
    send(sock, normal_data, strlen(normal_data), 0);
    send(sock, oob_data, strlen(oob_data), MSG_OOB);
    send(sock, normal_data, strlen(normal_data), 0);
  }
  close(sock);
  return 0;
}
