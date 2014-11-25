// Shim for stuff. Only supports one active socket.

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "ethshim.h"
#include "blah.h"

#define Sock_SEND (42)
//const uint8_t ECSockClosed       = SnSR::CLOSED;
const uint8_t ECSnCrSockSend     = Sock_SEND;
//const uint8_t ECSnCrSockRecv     = Sock_RECV;
//const uint8_t ECSnMrUDP          = SnMR::UDP;
//const uint8_t ECSnMrMulticast    = SnMR::MULTI;

#define MAX_BUFFER (65535)

char *received_buf = NULL;
ssize_t received_len = 0;

char *send_buf = NULL;
uint16_t sn_tx_wr_difference = 0;

void ethshim_init() {
    received_buf = (char *)malloc(MAX_BUFFER); // max udp ipv6 size
    send_buf = (char *)malloc(MAX_BUFFER);
}

// READ

// Used to check if we've received anything
uint16_t ethernet_compat_read_SnRX_RSR(int socket) {
    if (received_buf == NULL) {
        ethshim_init();
    }

    int err;

    err = fcntl(socket, F_SETFL, O_NONBLOCK, 1);
    assert(!err);

    struct sockaddr_in senderaddr;
    socklen_t addrlen = sizeof(senderaddr);
    received_len = recvfrom(socket, received_buf, MAX_BUFFER, 0, (struct sockaddr *)&senderaddr, &addrlen);

    if (received_len == -1) {
        return 0;
    }

    return 1;
}
// Used to get a pointer to received data, which is used with read_data. Just return 0 since everything will be relative
uint16_t ethernet_compat_read_SnRX_RD(int socket) {
    return 0;
}
void ethernet_compat_read_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len) {
    memcpy(dst, received_buf+(intptr_t)src, len);
}

// WRITE

// When send is set into Sn_CR then it sends the difference that Sn_TX_WR changed by
// Also, used to get a pointer to where to start writing to, which is used with write_data. 0 is fine since all relative
uint16_t ethernet_compat_read_SnTX_WR(int socket) {
    if (received_buf == NULL) {
        ethshim_init();
    }
    sn_tx_wr_difference = 0;
    return 0;
}
void ethernet_compat_write_SnTX_WR(int socket, uint16_t ptr) {
    sn_tx_wr_difference = ptr;
}
void ethernet_compat_write_data(int socket, uint8_t* data, uint8_t* dst, uint16_t len) {
    memcpy(send_buf+(intptr_t)dst, data, len);
}
void ethernet_compat_write_SnCR(int socket, uint8_t cmd) {
    if (cmd == ECSnCrSockSend) {
        //net_send(socket, send_buf, sn_tx_wr_difference);
        sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(5353);
        address.sin_addr.s_addr = inet_addr("224.0.0.251");
        int socklen = sizeof(struct sockaddr_in);

        int blah = sendto(socket, send_buf, sn_tx_wr_difference, 0, (struct sockaddr *)&address, socklen);
        printf("send returned %d (errno = %d, s = %d)\n", blah, errno, socket);
    }
}
uint8_t ethernet_compat_read_SnCR(int socket) {
    return 0;
}

// GET IP
void ethernet_compat_read_SIPR(uint8_t* dst) {
    int ip = net_gethostip();
    memcpy(dst, &ip, 4);
}
