#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void process(int);
int main(int argc, char *argv[]) {
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
  unsigned int client_addrLen = sizeof(struct sockaddr_in);

  for (;;) {

    int accept_fd =
        accept(listen_sock, (struct sockaddr *)&client_addr, &client_addrLen);

    pid_t fork_pid = fork();
    if (fork_pid == -1) {
      perror("fork failed");
      return 1;
    } else if (fork() == 0) {
      // child
      close(listen_sock);
      process(accept_fd);
      close(accept_fd);
    } else {
      // parent
      //
      close(accept_fd);
    }
  }
  return 0;
}

void process(int accept_fd) {
  char body[1024] = {0};
  while (true) {
    memset(body, 0, 1024);
    int readed = read(accept_fd, body, 1024);
    if (readed > 0) {
      spdlog::info("read data: {}", body);
    } else if (readed == 0) {
      spdlog::info("read eof");
      break;
    } else {
      perror("read failed");
      break;
    }
    write(accept_fd, body, 1024);
  }
}
