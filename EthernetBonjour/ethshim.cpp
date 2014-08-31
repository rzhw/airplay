// Shim for stuff. Only supports one active socket.

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "ethshim.h"

#define MAX_BUFFER (65535)

char *received_buf = NULL;
ssize_t received_len;

void ethshim_init() {
    received_buf = (char *)malloc(MAX_BUFFER); // max udp ipv6 size
}

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

//void ethernet_compat_write_data(int socket, uint8_t* data, uint8_t* dst, uint16_t len);
