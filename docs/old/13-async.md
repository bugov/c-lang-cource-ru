Чуть более современная версия асинхронного сервера - использование epoll вместо
select.

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <fcntl.h>

#define DEFAULT_PORT 1337
#define MAX_CONN 16
#define MAX_EVENTS 32
#define BUF_SIZE 64

static char buf[BUF_SIZE];


void epoll_ctl_add(int epoll_fd, int fd, uint32_t events) {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
                perror("An error occurred while adding fd");
                exit(1);
        }
}

void set_sockaddr(struct sockaddr_in *addr) {
	bzero((char *)addr, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(DEFAULT_PORT);
}

static int setnonblocking(int conn_fd) {
	if (fcntl(
                conn_fd,
                F_SETFL,
                fcntl(conn_fd, F_GETFD, 0) | SOCK_NONBLOCK) == -1
        ) {
		return -1;
	}
	return 0;
}

void accept_conn(int listen_fd, int epoll_fd) {
        char addr_buf[16];
        struct sockaddr_in remote_addr;
        int sock_len = sizeof(remote_addr);
        
        int conn_fd = accept(
                listen_fd,
                (struct sockaddr *)&remote_addr,
                &sock_len
        );
        
        inet_ntop(
                AF_INET,
                (char *)&(remote_addr.sin_addr),
                addr_buf,
                sizeof(addr_buf)
        );
        
        printf(
                "Connect accepted from %s:%d\n",
                addr_buf,
                ntohs(remote_addr.sin_port)
        );
        
        setnonblocking(conn_fd);
        epoll_ctl_add(
                epoll_fd,
                conn_fd,
                EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP
        );
}

void handle_conn(conn_fd) {
        int len;
        while (1) {
                len = read(conn_fd, buf, sizeof(buf));
                if (len <= 0) {
                        break;
                }
                write(conn_fd, buf, len);
        }
}

void serve() {
        struct sockaddr_in srv_addr;
        struct epoll_event events[MAX_EVENTS];
        
        int listen_fd = socket(
                AF_INET,        // Семейство адресов "Интернет"
                SOCK_STREAM,    // TCP
                0               // Флаги фроде CLOEXEC
        );
        set_sockaddr(&srv_addr);
        bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
        setnonblocking(listen_fd);
        listen(listen_fd, MAX_CONN);
        
        int epoll_fd = epoll_create(1);     // Параметр `size`
        // Since Linux 2.6.8, the size argument is ignored,
        // but must be greater than zero
        
        epoll_ctl_add(epoll_fd, listen_fd, EPOLLIN | EPOLLOUT | EPOLLET);
        
        int i;
        int count_fds;
        while (1) {
                count_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                for (i = 0; i < count_fds; ++i) {
                        if (events[i].data.fd == listen_fd) {
                                accept_conn(listen_fd, epoll_fd);
                        } else if (events[i].events & EPOLLIN) {
                                handle_conn(events[i].data.fd);
                        } else {
                                printf("Unexpected fd\n");
                        }
                        
                        if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
                                printf("Connection closed\n");
                                epoll_ctl(
                                        epoll_fd,
                                        EPOLL_CTL_DEL,
                                        events[i].data.fd,
                                        NULL
                                );
                                close(events[i].data.fd);
                        }
                }
        }
}

int main() {
        serve();
        return 0;
}
```


# ДЗ

Написать чат

