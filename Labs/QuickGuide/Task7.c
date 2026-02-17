// Task7.c
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  struct addrinfo hints, *res;
  int fd;
  ssize_t nbytes, nleft, nwritten, nread;
  char *ptr, buffer[128 + 1];

  fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1)
    exit(1);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res) != 0)
    exit(1);
  if (connect(fd, res->ai_addr, res->ai_addrlen) == -1)
    exit(1);

  ptr = strcpy(buffer, "Hello!\n");
  nbytes = 7;
  nleft = nbytes;
  while (nleft > 0) {
    nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0)
      exit(1); /*error*/
    nleft -= nwritten;
    ptr += nwritten;
  }

  nleft = nbytes;
  ptr = buffer;
  while (nleft > 0) {
    nread = read(fd, ptr, nleft);
    if (nread == -1)
      exit(1); /*error*/
    else if (nread == 0)
      break; // closed by peer
    nleft -= nread;
    ptr += nread;
  }

  nread = nbytes - nleft;
  buffer[nread] = '\0';
  printf("echo: %s", buffer);

  freeaddrinfo(res);
  close(fd);
  exit(0);
}

/*
 * O que esta Task resolve:
 *
 * TCP Stream: Os dados são tratados como um fluxo contínuo de bytes.
 * write(): Envia dados através da ligação estabelecida.
 * read(): Recebe dados. Retorna 0 quando o servidor fecha a ligação (EOF).
 * Terminal Output: O uso de write(1, buffer, n) evita problemas com
 * terminadores nulos (\0).
 *
 */

/*
 * ANSWERS TO QUESTIONS 8 TO 10 (TASK 7):
 *
 * Question 8: Why is the write/read cycle needed for TCP?
 * Answer: Because TCP is a stream-oriented protocol, there is no guarantee that
 * a single read() will return all the data sent by a single write().
 *
 * Question 9: What happens if the server closes the connection?
 * Answer: The read() function will return 0, which indicates EOF (End of File).
 *
 * Question 10: Is the TCP echo server the same as the UDP one?
 * Answer: No. Although they share the same port number and perform the same
 * echo function, they are two different server processes using different
 * protocols.
 *
 */