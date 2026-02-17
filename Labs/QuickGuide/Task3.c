// Task3.c
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {

  struct addrinfo hints, *res;
  int fd, errcode;
  ssize_t n;

  fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
  if (fd == -1)                        /*error*/
    exit(1);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;      // IPv4
  hints.ai_socktype = SOCK_DGRAM; // UDP socket

  errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints,
                        &res); // Descobrir para onde enviar

  if (errcode != 0) /*error*/
    exit(1);

  n = sendto(fd, "Hello!\n", 7, 0, res->ai_addr,
             res->ai_addrlen); // Enviar a Mensagem

  if (n == -1) /*error*/
    exit(1);

  /*...*/
  freeaddrinfo(res);
  exit(0);
}