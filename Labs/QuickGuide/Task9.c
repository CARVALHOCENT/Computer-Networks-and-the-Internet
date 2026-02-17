#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  struct addrinfo hints, *res;
  int fd, errcode;
  struct sockaddr addr;
  socklen_t addrlen;
  ssize_t n, nread;
  char buffer[128];

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    exit(1); // error

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;      // IPv4
  hints.ai_socktype = SOCK_DGRAM; // UDP socket
  hints.ai_flags = AI_PASSIVE;

  if ((errcode = getaddrinfo(NULL, "58001", &hints, &res)) != 0) /*error*/
    exit(1);

  if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) /*error*/
    exit(1);

  while (1) {
    addrlen = sizeof(addr);

    nread = recvfrom(fd, buffer, 128, 0, &addr, &addrlen);
    if (nread == -1) /*error*/
      exit(1);

    n = sendto(fd, buffer, nread, 0, &addr, addrlen);
    if (n == -1) /*error*/
      exit(1);
  }

  // freeaddrinfo(res);
  // close(fd);
  // exit(0);
}

/*
 * O que esta Task Resolve:
 *
 * Servidor Iterativo: Processa um pedido de cada vez num ciclo infinito.
 * bind(): O servidor reserva o porto 58001 para si, permitindo que clientes o
 * encontrem. AI_PASSIVE: Permite que o servidor aceite pacotes destinados a
 * qualquer IP da máquina local. Recvfrom/Sendto: O servidor descobre quem é o
 * cliente no momento em que recebe o pacote (via &addr).
 *
 * ANSWERS TO QUESTIONS 11 TO 13:
 *
 * Question 11: What is the purpose of the bind() function?
 * Answer: It associates the socket with a specific local port so the system
 * knows that data arriving at that port belongs to this program.
 *
 * Question 12: What happens if you try to bind to a port that is already in
 * use? Answer: The function returns -1 (error). Only one process can bind to a
 * specific protocol/port pair at a time.
 *
 * Question 13: Can multiple clients send messages to this server?
 * Answer: Yes. Since it's UDP and the server uses a loop, it will process each
 * datagram as it arrives, regardless of which client sent it.
 */