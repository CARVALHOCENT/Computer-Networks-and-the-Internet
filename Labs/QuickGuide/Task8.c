// Task8.c
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
  int fd;
  struct sigaction act;

  memset(&act, 0, sizeof act);
  act.sa_handler = SIG_IGN;
  if (sigaction(SIGPIPE, &act, NULL) == -1) /*error*/
    exit(1);

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
    exit(1);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res) != 0)
    exit(1);
  if (connect(fd, res->ai_addr, res->ai_addrlen) == -1)
    exit(1);

  freeaddrinfo(res);
  close(fd);
  exit(0);
}

/*
 * O que esta Task Resolve:
 *
 * Gestão de Sinais: O sinal SIGPIPE é enviado ao processo quando este tenta
 * escrever num socket que foi fechado pela outra ponta.
 * SIG_IGN: Ao ignorar este sinal, o programa não termina abruptamente,
 * permitindo que o erro seja tratado através do valor de retorno da função
 * write().
 * Robustez: Essencial para programas de rede que devem continuar a correr
 * mesmo quando ocorrem erros de ligação.
 *
 * ANSWERS TO QUESTIONS:
 *
 * Question: Why do we need to ignore SIGPIPE?
 * Answer: To prevent the application from crashing when a connection is
 * unexpectedly closed by the peer. Instead of the process being killed, the
 * write() call will simply return -1 and set errno to EPIPE.
 */