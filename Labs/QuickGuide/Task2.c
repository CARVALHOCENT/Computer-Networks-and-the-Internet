// Task2.c
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {

  struct addrinfo hints, *res, *p;
  int errcode;
  char buffer[INET_ADDRSTRLEN];
  struct in_addr *addr;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;      // IPv4
  hints.ai_socktype = SOCK_DGRAM; // Tipo UDP
  hints.ai_flags = AI_CANONNAME;

  /* getaddrinfo(ip/name, port, constraints, &result); */
  if ((errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", NULL, &hints, &res)) !=
      0)
    fprintf(stderr, "error: getaddrinfo: %s\n", gai_strerror(errcode));
  else {
    printf("canonical hostname: %s\n",
           res->ai_canonname); // Flag canonname --> Nome "oficial" do server
    for (p = res; p != NULL; p = p->ai_next) { // Um server pode ter vários IPs
                                               // corremos todos e imprimimo-los
      struct sockaddr_in *ip = (struct sockaddr_in *)p->ai_addr;
      addr = &(ip->sin_addr);
      printf("internet address: %s (%08lX)\n",
             inet_ntop(
                 p->ai_family, addr, buffer,
                 sizeof(buffer)), // inet_ntop --> Converte o endereço binário
             (long unsigned int)ntohl(
                 addr->s_addr)); // ntohl --> Inverte ordem do bytes da Rede
                                 // para a do Host para permitir a correta
                                 // impressão do valor Hexadecimal
    }
    freeaddrinfo(res);
  }
  exit(0);
}

/* Nota Importante

    0xC1==193
    0x88==136
    0x8A==138
    0x8E==142

*/