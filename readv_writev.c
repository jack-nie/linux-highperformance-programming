#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFSIZE 1024

static const char *status_line[2] = {"200 OK!", "500 Internal server error!"};

int
main(int argc, char *argv[])
{
    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number filename.\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    char *file_name = argv[3];

    struct sockaddr_in address;
    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock > 0);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 1);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addr_length = sizeof(client);

    int connfd = accept(sock, (struct sockaddr*)&client, &client_addr_length);
    if (connfd < 0)
    {
        printf("connection failed, errno is: %d", errno);
    }
    else
    {
        char header_buf[BUFSIZE];
        memset(&header_buf, '\0', BUFSIZE);
        char *filebuf;
        static stat file_stat;
        bool valid = true;
        int len = 0;
        if  (stat(file_name, &file_stat) < 0)
        {
            valid = false;
        }
        else
        {
            if (S_ISDIR(file_stat.st_mode))
            {
                valid = false;
            }
            else if (file_stat.st_mode & S_IROTH)
            {
                int fd = open(file_name, O_RDONLY);
                filebuf = (char*)malloc(file_stat.st_size + 1);
                memset(filebuf, '\0', file_stat.st_size +1);
                if (read(fd, filebuf, file_stat.st_size) < 0)
                {
                    valid = false;
                }
            }
            else
            {
                valid = false;
            }
        }
        if (valid)
        {
            ret = snprintf(header_buf, BUFSIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[0]);
            len += ret;
            ret = snprintf(header_buf + len, BUFSIZE -1 -len, "Content-Length: %d\r\n", file_stat.st_size);
            len += ret;
            ret = snprintf(header_buf + len, BUFSIZE -1 -len, "%s", "\r\n");
            struct iovec iv[2];
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = filebuf;
            iv[1].iov_len = file_stat.st_size;
            ret = writev(connfd, iv, 2);
        }
        else
        {
            ret = snprintf(header_buf, BUFSIZE -1, "%s %s\r\n", "HTTP/1.1", status_line[1]);
            len += ret;
            ret = snprintf(header_buf+len, BUFSIZE -1 - len, "%s", "\r\n");
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close(connfd);
        free(filebuf);
    }
    close(sock);
    return 0;
}
