// Task4.c
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int fd;
  struct addrinfo hints, *res;
  struct sockaddr addr;
  socklen_t addrlen;
  ssize_t n;
  char buffer[128 + 1];

  /* socket(ipv4/ipv6, udp/tcp, 0) */
  fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
  if (fd == -1)                        /*error*/
    exit(1);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;      // IPv4
  hints.ai_socktype = SOCK_DGRAM; // UDP socket

  if (getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res) !=
      0) /*error*/
    exit(1);

  /* bytes_sent = sendto(fd, message, message_size, 0, receiver_adress,
   * address_size) */
  n = sendto(fd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
  if (n == -1) /*error*/
    exit(1);

  addrlen = sizeof(addr);
  /* bytes_received = recvdrom(fd, buffer, buffer_size, 0, sender_adress,
   * adress_size) */
  n = recvfrom(fd, buffer, 128, 0, &addr,
               &addrlen); // blocks until datagram received
  if (n == -1)            /*error*/
    exit(1);

  buffer[n] = '\0';
  printf("echo: %s\n", buffer);

  freeaddrinfo(res);
  close(fd);
  exit(0);
}

/*
 * O que resolvemos:
 *
 * Comunicação UDP: Utiliza datagramas que não garantem a entrega.
 * Bloqueio: O programa fica parado no recvfrom até que o servidor responda.
 * Echo: O servidor recebe os 7 bytes ("Hello!\n") e envia-os de volta.
 * Terminação: Como a rede não envia o \0, adicionamos manualmente para o printf
 * funcionar.
 *
 */

/*
 * ANSWERS TO QUESTIONS 1 TO 5 (TASK 5):
 *
 * Question 1: What happens if the messages do not arrive at the destination?
 * Answer: No message will be received back at the client and it will block in
 * recvfrom. No error will be detected unless timeouts are used. In UDP there
 * are no guarantees that messages will be delivered or that the order will be
 * the same as transmitted.
 *
 * Question 2: How do you know the message you received came from the UDP echo
 * server on tejo:58001?
 * Answer: You have to check the recvfrom addr output
 * argument. You can use getnameinfo for that purpose.
 *
 * Question 3: Which port number is your UDP client listening to when it is
 * waiting for the echo reply?
 * Answer: The system assigned some unused port when
 * you first called sendto. If you want to use a specific port number
 * you have to use bind.
 *
 * Question 4: How many bytes do you expect to receive from recvfrom?
 * Answer: In this particular case, you should expect to receive 7 bytes.
 *
 * Question 5: Do you expect buffer content to be a NULL terminated string?
 * Answer: In this particular case, you should not expect the buffer to be NULL
 * terminated. The '\0' was not sent.
 */