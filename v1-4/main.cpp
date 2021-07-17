#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>

int main() {
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sock < 0) {
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
      bind(listen_sock, (struct sockaddr *)&servera_addr, sizeof(servera_addr));
  if (-1 == bind_ret) {
    spdlog::error("bind socket to address failed");
    return -1;
  }

  // backlog length == 1000
  listen(listen_sock, 1000);

  const int MAX_CONNECT = 10;
  const int BUF_SIZE = 255;
  char buf[BUF_SIZE] = {0}; // echo buff
  int sin_size = sizeof(client_addr);
  struct pollfd client[MAX_CONNECT] = {0};
  int maxfd = listen_sock;

  for (int i = 0; i < MAX_CONNECT; ++i) {
    client[i].fd = -1;
  }
  // 一开始把监听fd加入集合中
  client[0].fd = listen_sock;
  spdlog::info("listen fd {}", listen_sock);
  client[0].events = POLLIN;

  for (;;) {
    // timeout（单位毫秒）参数设置为-1时，表示永远阻塞等待。0表示立即返回，不阻塞。大于0时，表示等待指定数目的毫秒数。

    int nready = poll(client, maxfd + 1, -1);
    for (int i = 1; i < MAX_CONNECT; ++i) {
      spdlog::info("read fd {}, events:{}, revents:{} ", client[i].fd,
                   client[i].events, client[i].revents);
    }
    spdlog::info("poll once {}", nready);
    if (nready == -1) {
      perror("");
      continue;
    } else if (nready == 0) {
      spdlog::info("poll timeout");
      continue;
    }

    for (int i = 1; i < MAX_CONNECT; ++i) {
      int sockfd;
      if ((sockfd = client[i].fd) <= 0)
        continue;
      if (client[i].revents & (POLLIN)) {
        spdlog::info("revents {}", sockfd);
        // 从socket读数据
        int n = recv(sockfd, buf, BUF_SIZE, 0);
        if (n == 0) {
          spdlog::info("client close: {}", sockfd);
          close(sockfd);
          client[i].fd = -1;
        } else if (n == -1 && errno != EINTR) {
          perror("read: ");
          send(sockfd, "sorry bye", 10, 0);
          close(sockfd);
          client[i].fd = -1;
        } else {
          // 打印出来，并写回客户端
          // send(STDOUT_FILENO, buf, n, 0);
          spdlog::info("fd {} received {} ", client[i].fd, buf);
          send(sockfd, buf, sizeof(buf), 0);
        }
      }
    }
    // 是监听fd，说明有客户端连接进来
    if (client[0].revents & POLLIN) {
      int cfd = accept(listen_sock, (struct sockaddr *)&client_addr,
                       (socklen_t *)&sin_size);
      printf("client connect: %d\n", cfd);
      bool over_limit = true;
      //添加一个cfd连接到 client
      for (int i = 1; i < MAX_CONNECT; i++) {
        if (client[i].fd <= 0) {
          spdlog::info("pollfd  add {}", cfd);
          client[i].fd = cfd;
          client[i].events = POLLIN;
          client[i].revents = 0;
          over_limit = false;
          break;
        }
      }
      if (over_limit) {
        spdlog::info("too many client");
        write(cfd, "sorry,bye", 10);
        close(cfd);
      } else {
        if (cfd > maxfd) {
          spdlog::info("maxfd {}", cfd);
          maxfd = cfd;
        }
      }
      // 将新连进来的fd加入集合
    }
  }

  return 0;
}
