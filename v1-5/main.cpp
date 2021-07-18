#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>

int main() {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    spdlog::error("create socket failed");
    return -1;
  }

  //给 socket 绑定地址
  struct sockaddr_in servera_addr, client_addr;
  memset(&servera_addr, 0, sizeof(struct sockaddr_in));
  servera_addr.sin_family = AF_INET;
  servera_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  servera_addr.sin_port = htons(8080);

  auto bind_ret =
      bind(listen_fd, (struct sockaddr *)&servera_addr, sizeof(servera_addr));
  if (-1 == bind_ret) {
    spdlog::error("bind socket to address failed");
    return -1;
  }

  // backlog length == 1000
  listen(listen_fd, 1000);

  int epfd = epoll_create(100); // 参数大0即可
  if (epfd == -1) {
    perror("create epoll failed");
    return -1;
  }

  const int MAX_EVENT = 10;
  int n_add = ep_add_listen(epfd, listen_fd);
  if (n_add == -1) {
    return -1;
  }

  struct epoll_event events[MAX_EVENT] = {0};
  for (;;) {

    epoll_wait(epfd, ev, MAX_EVENT, -1);
    // 添加 listen_fd
    //
  }

  return 0;
}

int ep_add_listen(int epfd, int listen_fd) {
  struct epoll_event ev;
  ev.ev.events = EPOLLIN;
  ev.data.fd = listen_fd;
  int add_ = epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);
  if (add_ == -1) {
    perror("epoll add listen failed");
    return -1;
  }
}
