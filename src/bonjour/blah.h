// Copyright (C) 2014 Richard Wang
//
// This file is part of rzhw/airplay.
//
// rzhw/airplay is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// rzhw/airplay is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with rzhw/airplay. If not, see <http://www.gnu.org/licenses/>.

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
int net_get_mac_address(void *mac_buf);

#include <inttypes.h>
extern uint16_t ethutil_htons(unsigned short hostshort);
extern uint32_t ethutil_htonl(unsigned long hostlong);
extern uint16_t ethutil_ntohs(unsigned short netshort);
extern uint32_t ethutil_ntohl(unsigned long netlong);

#endif /* defined(__ArduinoEthernetTest__derp__) */
