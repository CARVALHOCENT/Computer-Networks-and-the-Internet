// Task11.c
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define max(A, B) ((A) >= (B) ? (A) : (B))

int main(void) {
  struct addrinfo hints, *res;
  int fd, newfd, afd = 0, errcode;
  fd_set rfds;
  enum { idle, busy } state;
  int maxfd, counter;
  ssize_t n;
  char buffer[128];
  struct sockaddr addr;
  socklen_t addrlen;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    exit(1); // error

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP socket
  hints.ai_flags = AI_PASSIVE;

  if ((errcode = getaddrinfo(NULL, "58001", &hints, &res)) != 0) /*error*/
    exit(1);
  if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) /*error*/
    exit(1);
  if (listen(fd, 5) == -1) /*error*/
    exit(1);

  state = idle;
  while (1) {
    FD_ZERO(&rfds);
    switch (state) {
    case idle:
      FD_SET(fd, &rfds);
      maxfd = fd;
      break;
    case busy:
      FD_SET(fd, &rfds);
      FD_SET(afd, &rfds);
      maxfd = max(fd, afd);
      break;
    } // switch(state)

    counter = select(maxfd + 1, &rfds, (fd_set *)NULL, (fd_set *)NULL,
                     (struct timeval *)NULL);
    if (counter <= 0) /*error*/
      exit(1);

    for (; counter; --counter)
      switch (state) {
      case idle:
        if (FD_ISSET(fd, &rfds)) {
          FD_CLR(fd, &rfds);
          addrlen = sizeof(addr);
          if ((newfd = accept(fd, &addr, &addrlen)) == -1) /*error*/
            exit(1);
          afd = newfd;
          state = busy;
        }
        break;
      case busy:
        if (FD_ISSET(fd, &rfds)) {
          FD_CLR(fd, &rfds);
          addrlen = sizeof(addr);
          if ((newfd = accept(fd, &addr, &addrlen)) == -1) /*error*/
            exit(1);
          write(newfd, "busy\n", 5);
          close(newfd);
        } else if (FD_ISSET(afd, &rfds)) {
          FD_CLR(afd, &rfds);
          if ((n = read(afd, buffer, 128)) != 0) {
            if (n == -1) /*error*/
              exit(1);
            write(afd, buffer, n);
          } else {
            close(afd);
            state = idle;
          } // connection closed by peer
        }
        break;
      } // switch(state)
  } // while(1)

  freeaddrinfo(res);
  close(fd);
  exit(0);
}

/*
 * O que esta Task Resolve:
 *
 * select(): Permite monitorizar múltiplos descritores (o de escuta e o de
 * dados) num só ciclo.
 * Máquina de Estados: O servidor alterna entre 'idle' (livre) e 'busy'
 * (ocupado com um cliente).
 * Gestão de Recurso: Se um segundo cliente tenta ligar-se em modo 'busy', o
 * servidor aceita, diz "busy" e fecha, sem bloquear o cliente atual.
 */

/*
 * Question 13: And now, where do you expect the program to block?
 * Answer: This program is only going to block in the select call. It would not
 * block neither in the accept call, neither in the read call, since those are
 * only executed when their sockets are ready to be read (and so they have no
 * reason to block).
 *
 */