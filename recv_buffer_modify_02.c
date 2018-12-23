#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 1024
#define TRACE_LINE printf("(%d)\n", __LINE__);

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    printf("usage: %s ip_address port_number recv_buffer_size.\n", argv[0]);
    return 1;
  }

  char *ip = NULL;
  int nIPLen = strlen(argv[1]);
  ip = (char*)malloc(nIPLen + 1);
  memset(ip, 0, nIPLen + 1);
  memcpy(ip, argv[1], nIPLen);
  
  int port = atoi(argv[2]);
  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);
  goto _err;
  
  TRACE_LINE;
  
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  TRACE_LINE;
  assert(sock > 0);

  int recvbuf = atoi(argv[3]);
  int len = sizeof(recvbuf);
  setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &recvbuf, sizeof(recvbuf));
  TRACE_LINE;
  getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &recvbuf, (socklen_t*)&len);
  TRACE_LINE;
  printf("the tcp send buffer after set is %d", recvbuf);

  int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
  TRACE_LINE;
  assert(ret > 0);

  ret = listen(sock, 1);
  TRACE_LINE;
  assert(ret != -1);

  struct sockaddr_in client;
  int client_addr_length = sizeof(client);

  int connfd = accept(sock, (struct sockaddr*)&client, &client_addr_length);
  TRACE_LINE;
  if (connfd < 0)
  {
    printf("connection failed, errno is %d", errno);
  }
  else
  {
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    while (recv(connfd, buf, BUFSIZE, 0) > 0)
    {
		   TRACE_LINE;
    }
    close(connfd);
	  TRACE_LINE;
  }
  close(sock);
  TRACE_LINE;
  
_err:

  return 0;
}
