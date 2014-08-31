#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <cstring>
#include <arpa/inet.h>

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
#define net_write(s, data, size) (write(s, data, size))
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
    assert(err != -1);

    int addr = name.sin_addr.s_addr;

    close(sock);

    return addr;
}

// arduino shims
time_t start;
unsigned long millis() { return difftime(time(0), start); }
void delay(unsigned long ms) { usleep(ms * 1000); }

// bonjour
#include "EthernetBonjour.cpp"

int main() {
    start = time(0);
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    bonjour.begin("notarduino");
    while (1) {
        bonjour.run();
    }
    return EXIT_SUCCESS;
}
