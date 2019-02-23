#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const int CONTROL_LEN = CMSG_LEN(sizeof(int));

void
send_fd(int fd, int fd_to_send)
{
  struct iovec iov[1];
  struct msghdr msg = {0};
  char buf[2];
#if HAVE_STRUCTMSGHDR_MSG_ACCRIGHTS
  msg.msg_accrights = (void*)&fd_to_send;
  msg.msg_accrightslen = sizeof(int);
#else
  struct cmsghdr *cm;
  char anc_buffer[CMSG_SPACE(sizeof(int))];
  char* d_ptr;
  msg.msg_controllen = sizeof(anc_buffer);
  msg.msg_control = anc_buffer;

  cm = CMSG_FIRSTHDR(&msg);
  cm->cmsg_level = SOL_SOCKET;
  cm->cmsg_len = CMSG_LEN(sizeof(int));
  cm->cmsg_type = SCM_RIGHTS;
  d_ptr = (char*)CMSG_DATA(cm);


  *(int*)d_ptr = fd_to_send;
  msg.msg_controllen = cm->cmsg_len;
#endif
  buf[0] = 0;
  buf[1] = '\0';

  iov[0].iov_base = buf;
  iov[0].iov_len = 2;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  sendmsg(fd, &msg, 0);
}

int
recv_fd(int fd)
{
  struct iovec iov[1];
  struct msghdr msg;
  char buf[0];

  iov[0].iov_base = buf;
  iov[0].iov_len = 1;
  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  struct cmsghdr cm;
  msg.msg_control = &cm;
  msg.msg_controllen = CONTROL_LEN;

  recvmsg(fd, &msg, 0);
  int fd_to_read = *(int*)CMSG_DATA(&cm);
  return fd_to_read;
}

int
main()
{
  int pipefd[2];
  int fd_to_pass = 0;
  int ret = socketpair(PF_UNIX, SOCK_DGRAM, 0, pipefd);
  assert(ret != -1);

  pid_t pid = fork();
  assert(pid >= 0);

  if (pid == 0)
  {
    close(pipefd[0]);
    fd_to_pass = open("test.txt", O_RDWR, 0666);
    send_fd(pipefd[1], (fd_to_pass > 0) ? fd_to_pass : 0);
    close(fd_to_pass);
    exit(0);
  }

  close(pipefd[1]);
  fd_to_pass = recv_fd(pipefd[0]);
  char buf[1024];
  memset(buf, '\0', 1024);
  read(fd_to_pass, buf, 1024);
  printf("I got fd %d and data %s\n", fd_to_pass, buf);
  close(fd_to_pass);
}
