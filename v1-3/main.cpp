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
#define MAX_CONNECT 10
#define BUF_SIZE 255
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
  // unsigned int client_addrLen = sizeof(struct sockaddr_in);

  fd_set all_fd; //文件描述符集的定义
  int maxsock;
  struct timeval tv;
  int conn_amount = 0;
  int sin_size = sizeof(client_addr);
  maxsock = listen_sock;
  int fd_conn[MAX_CONNECT] = {0};
  char buf[255] = {0};
  while (1) {
    //每次select 之前都要刷新 fd_set 并填充
    FD_ZERO(&all_fd);
    FD_SET(listen_sock, &all_fd); // FD_SET = FD_FILL

    //将所有的连接全部加到这个这个集合中，
    for (int i = 0; i < MAX_CONNECT; i++) {
      //如果已经创建出来, >0 则加入
      if (fd_conn[i] != 0) {
        FD_SET(fd_conn[i], &all_fd);
      }
    }
    //如果文件描述符中有连接请求 会做相应的处理，实现I/O的复用 多用户的连接通讯
    // maxsock 取 fd_conn 和 listen_sock 的最大值
    int ret = select(maxsock + 1, &all_fd, NULL, NULL, NULL);
    if (ret < 0) //没有找到有效的连接 失败
    {
      perror("select error!\n");
      break;
    } else if (ret == 0) // 指定的时间到，
    {
      printf("timeout \n");
      continue;
    }
    //下面这个循环是非常必要的，因为你并不知道是哪个连接发过来的数据，所以只有一个一个去找。
    for (int i = 0; i < conn_amount; i++) {
      //如果是需要读取的 fd 变化
      if (FD_ISSET(fd_conn[i], &all_fd)) {
        ret = recv(fd_conn[i], buf, sizeof(buf), 0);
        //如果客户端主动断开连接，会进行四次挥手 recv 会==0 代表 EOF

        if (ret <= 0) //客户端连接关闭，清除文件描述符集中的相应的位
        {
          spdlog::info("client fd {} close", i);
          close(fd_conn[i]);
          FD_CLR(fd_conn[i], &all_fd);
          fd_conn[i] = 0;
          conn_amount--;
        }
        //否则有相应的数据发送过来 ，进行相应的处理
        else {
          if (ret < BUF_SIZE)
            memset(&buf[ret], 0, sizeof(buf));
          spdlog::info("client {} read: {}", i, buf);
        }
      }
    }
    //如果是 listenfd 变化, 则读取说明有新客户端过来
    if (FD_ISSET(listen_sock, &all_fd)) {
      int new_fd = accept(listen_sock, (struct sockaddr *)&client_addr,
                          (socklen_t *)&sin_size);
      if (new_fd <= 0) {
        perror("accept error\n");
        continue;
      }
      //添加新的fd 到数组中
      //判断有效的连接数是否小于最大的连接数，如果小于的话，就把新的连接套接字加入集合
      //
      if (conn_amount < MAX_CONNECT) {
        for (int i = 0; i < MAX_CONNECT; i++) {
          if (fd_conn[i] == 0) {
            fd_conn[i] = new_fd;
            break;
          }
        }
        conn_amount++;
        spdlog::info("new connection client {} {} : {}", conn_amount,
                     inet_ntoa(client_addr.sin_addr),
                     ntohs(client_addr.sin_port));
        if (new_fd > maxsock) {
          maxsock = new_fd;
        }
      }

      else {
        spdlog::info("max connections arrive ,exit");
        send(new_fd, "sorry bye", 10, 0);
        close(new_fd);
        continue;
      }
    }
    // showclient();
  }

  for (int i = 0; i < MAX_CONNECT; i++) {
    if (fd_conn[i] != 0) {
      close(fd_conn[i]);
    }
  }

  return 0;
}

// void process(int accept_fd) {
//  char body[1024] = {0};
//  while (true) {
//    memset(body, 0, 1024);
//    int readed = read(accept_fd, body, 1024);
//    if (readed > 0) {
//      spdlog::info("read data: {}", body);
//    } else if (readed == 0) {
//      spdlog::info("read eof");
//      break;
//    } else {
//      perror("read failed");
//      break;
//    }
//    write(accept_fd, body, 1024);
//  }
//}
