#include "blah.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdio>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>

// ethernetutil.c

#define __ETHERNET_UTIL_BONJOUR__ (1)

#if defined(__ETHERNET_UTIL_BONJOUR__)

uint16_t ethutil_swaps(uint16_t i);
uint32_t ethutil_swapl(uint32_t l);

extern uint16_t ethutil_htons(unsigned short hostshort)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return ethutil_swaps(hostshort);
#else
    return hostshort;
#endif
}

extern uint32_t ethutil_htonl(unsigned long hostlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return ethutil_swapl(hostlong);
#else
    return hostlong;
#endif
}

extern uint16_t ethutil_ntohs(unsigned short netshort)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return ethutil_swaps(netshort);
#else
    return netshort;
#endif
}

extern uint32_t ethutil_ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return ethutil_swapl(netlong);
#else
    return netlong;
#endif
}

#pragma mark -
#pragma mark Private

uint16_t ethutil_swaps(uint16_t i)
{
    uint16_t ret=0;
    ret = (i & 0xFF) << 8;
    ret |= ((i >> 8)& 0xFF);
    return ret;
}

uint32_t ethutil_swapl(uint32_t l)
{
    uint32_t ret=0;
    ret = (l & 0xFF) << 24;
    ret |= ((l >> 8) & 0xFF) << 16;
    ret |= ((l >> 16) & 0xFF) << 8;
    ret |= ((l >> 24) & 0xFF);
    return ret;
}

#endif // __ETHERNET_UTIL_BONJOUR__


// devkit shims
int net_write(int s, const void *data, int size) {
    //printf("send hello!\n");

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(5353);
    address.sin_addr.s_addr = inet_addr("224.0.0.251");
    int socklen = sizeof(struct sockaddr_in);

    int blah = sendto(s, data, size, 0, (struct sockaddr *)&address, socklen);
    printf("send returned %d (errno = %d, s = %d)\n", blah, errno, s);
    return blah;
}
#define net_read(s, mem, len) (read(s, mem, len))
#define net_close(s) (close(s))
#define IP4_ADDR(ipaddr, a,b,c,d) (ipaddr)->s_addr = htonl(((int32_t)(a&0xff)<<24)|((int32_t)(b&0xff)<<16)|((int32_t)(c&0xff)<<8)|(int32_t)(d&0xff))
// https://stackoverflow.com/questions/212528/get-the-ip-address-of-the-machine
int net_gethostip() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sock != -1);

    const char* kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);

    int err = connect(sock, (const sockaddr*) &serv, sizeof(serv));
    assert(err != -1);

    sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (sockaddr*) &name, &namelen);
    assert(err != -1); // Fails if no internet connection

    int addr = name.sin_addr.s_addr;

    close(sock);

    return addr;
}
// u8 mac[6]
int net_get_mac_address(void *mac_buf) {
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else { /* handle error */ }
    }

    if (success) memcpy(mac_buf, ifr.ifr_hwaddr.sa_data, 6);

    return 0;
}

// arduino shims

unsigned long millis() {
    clock_t diff = clock() - start;
    long ms = diff * 1000;
    return diff;
}
void delay(unsigned long ms) { usleep(ms * 100); } // should actually be 1000
