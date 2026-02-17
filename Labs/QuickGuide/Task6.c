// Task6.c
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  struct addrinfo hints, *res;
  int fd;
  ssize_t n;

  fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1)
    exit(1); // error

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP socket

  n = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res);
  if (n != 0) /*error*/
    exit(1);

  n = connect(fd, res->ai_addr, res->ai_addrlen);
  if (n == -1) /*error*/
    exit(1);

  /*...*/

  freeaddrinfo(res);
  close(fd);
  exit(0);
}

/*
 * ANSWERS TO QUESTIONS 6 TO 7 (TASK 6):
 *
 * Question 6: Did you notice that the host name and port number are the same as
 * before? Answer: There is no problem in having two servers on the same port
 * number as long as they are using different protocols. In this case, one is
 * using UDP and the other TCP.
 *
 * Question 7: What do you expect to happen if you type the wrong host name or
 * port number? Answer: If you type the wrong host name, getaddrinfo would give
 * you an error, unless you type a name that also exists. If you type the wrong
 * port number, connect would give you an error, unless there is a TCP server
 * listening on that port.
 */