#ifndef __ArduinoEthernetTest__derp__
#define __ArduinoEthernetTest__derp__

int net_write(int s, const void *data, int size);
#define net_read(s, mem, len) (read(s, mem, len))
#define net_close(s) (close(s))
#define IP4_ADDR(ipaddr, a,b,c,d) (ipaddr)->s_addr = htonl(((int32_t)(a&0xff)<<24)|((int32_t)(b&0xff)<<16)|((int32_t)(c&0xff)<<8)|(int32_t)(d&0xff))

#include <time.h>
extern clock_t start;
unsigned long millis();
void delay(unsigned long ms);
int net_gethostip();

#include <inttypes.h>
extern uint16_t ethutil_htons(unsigned short hostshort);
extern uint32_t ethutil_htonl(unsigned long hostlong);
extern uint16_t ethutil_ntohs(unsigned short netshort);
extern uint32_t ethutil_ntohl(unsigned long netlong);

#endif /* defined(__ArduinoEthernetTest__derp__) */
