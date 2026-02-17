#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  struct addrinfo hints, *res;
  int fd, newfd, errcode;
  ssize_t n, nw;
  struct sockaddr addr;
  socklen_t addrlen;
  char *ptr, buffer[128];
  struct sigaction act;

  memset(&act, 0, sizeof act);
  act.sa_handler = SIG_IGN;
  if (sigaction(SIGPIPE, &act, NULL) == -1)
    exit(1); // error

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    exit(1);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP socket
  hints.ai_flags = AI_PASSIVE;

  if ((errcode = getaddrinfo(NULL, "58001", &hints, &res)) != 0) /*error*/
    exit(1);
  if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) /*error*/
    exit(1);
  if (listen(fd, 5) == -1)
    exit(1);

  while (1) {
    addrlen = sizeof(addr);
    if ((newfd = accept(fd, &addr, &addrlen)) == -1) /*error*/
      exit(1);

    while ((n = read(newfd, buffer, 128)) != 0) {
      if (n == -1)
        exit(1);
      ptr = &buffer[0];
      while (n > 0) {
        if ((nw = write(newfd, ptr, n)) <= 0) /*error*/
          exit(1);
        n -= nw;
        ptr += nw;
      }
    }
    close(newfd);
  }

  // freeaddrinfo(res); close(fd);
  exit(0);
}

/*
 * O que esta Task Resolve:
 *
 * listen(): Transforma o socket num socket passivo que aceita ligações.
 * accept(): Bloqueia o programa até que um cliente se ligue. Cria um novo
 * socket (newfd). Gestão de Sockets: O 'fd' continua a ouvir novos clientes, o
 * 'newfd' fala com o cliente atual. Ciclo de Leitura: O servidor lê do fluxo
 * (stream) até o cliente fechar a ligação (read retorna 0).
 *
 */

/* ANSWERS TO QUESTIONS 11 TO 12:
 *
 * Question 11: What is the purpose of the listen() function?
 * Answer: It defines the socket as passive, allowing it to accept incoming
 * connection requests. It also defines the maximum length of the queue for
 * pending connections.
 *
 * Question 12: What is the difference between the descriptors fd and newfd?
 * Answer: fd is the listening socket used to accept new connections.
 * newfd is a new socket created by accept() specifically for data exchange with
 * a particular client.
 */