#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "EthernetBonjour.h"

#include "blah.h"
#include <time.h>
clock_t start;

#define PAGE2 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"\
"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\""\
" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"\
"<plist version=\"1.0\">"\
" <dict>"\
"  <key>height</key>"\
"  <integer>720</integer>"\
"  <key>overscanned</key>"\
"  <true/>"\
"  <key>refreshRate</key>"\
"  <real>0.016666666666666666</real>"\
"  <key>version</key>"\
"  <string>130.14</string>"\
"  <key>width</key>"\
"  <integer>1280</integer>"\
" </dict>"\
"</plist>"
#include <microhttpd.h>
#include <string.h>

#include "airplay.hpp"

static int mirroring_handler(void * cls,
            struct MHD_Connection * connection,
            const char * url,
            const char * method,
                    const char * version,
            const char * upload_data,
            size_t * upload_data_size,
                    void ** ptr);

int main() {
    printf("hello world!\n");

    AirPlayInfo info;

    unsigned char mac[6];
    net_get_mac_address(mac);
    char macstr[18];
    snprintf(macstr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    info.deviceid = macstr;
    info.features = 0x77;
    info.model = "uwotm8,1";
    info.protovers = "1.0";
    info.srcvers = "101.28";

    // Build the TXT record
    char txt_buffer[128] = {0};
    int txt_ptr = 0;
    int len;

    len = snprintf(txt_buffer+txt_ptr+1, sizeof(txt_buffer)-txt_ptr-1,
        "deviceid=%s", info.deviceid);
    assert(len <= 255);
    txt_buffer[txt_ptr] = len;
    txt_ptr += 1 + len;

    len = snprintf(txt_buffer+txt_ptr+1, sizeof(txt_buffer)-txt_ptr-1,
        "features=0x%x", info.features);
    assert(len <= 255);
    txt_buffer[txt_ptr] = len;
    txt_ptr += 1 + len;

    len = snprintf(txt_buffer+txt_ptr+1, sizeof(txt_buffer)-txt_ptr-1,
        "model=%s", info.model);
    assert(len <= 255);
    txt_buffer[txt_ptr] = len;
    txt_ptr += 1 + len;

    len = snprintf(txt_buffer+txt_ptr+1, sizeof(txt_buffer)-txt_ptr-1,
        "srcvers=%s", info.srcvers);
    assert(len <= 255);
    txt_buffer[txt_ptr] = len;
    txt_ptr += 1 + len;

    printf("txt is:\n%s\ntxt length is: %d\n", txt_buffer, strlen(txt_buffer));

    // Build the TXT record 2
    char raopname_buffer[128] = {0};
    snprintf(raopname_buffer, sizeof(raopname_buffer),
        "%02X%02X%02X%02X%02X%02X@%s._raop",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
        "uwotm9");

    char txt_buffer2[512] = {0};
    txt_ptr = 0;

    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "am=uwotm8,1");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "ch=2");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "cn=0,1");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "da=true");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "et=0,1");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "md=0,1,2");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "pw=false");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "sm=false");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "sr=44100");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "ss=16");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "sv=false");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "tp=UDP");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "txtvers=1");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "vn=3");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;
    len = snprintf(txt_buffer2+txt_ptr+1, sizeof(txt_buffer2)-txt_ptr-1,
        "vs=130.14");
    txt_buffer2[txt_ptr] = len; txt_ptr += 1 + len;

    // Broadcast the Bonjour service
    start = clock();
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    int err = bonjour.begin("wot");
    bonjour.addServiceRecord("uwotm9._airplay", 7000, MDNSServiceTCP, txt_buffer);
    bonjour.addServiceRecord(raopname_buffer, 49152, MDNSServiceTCP, txt_buffer2);
    assert(!err);

    AirPlayHTTPServer airPlayServer(&info);
    airPlayServer.begin();

  struct MHD_Daemon * d2;
  d2 = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
               7100,
               NULL,
               NULL,
               &mirroring_handler,
               (void *)PAGE2,
               MHD_OPTION_END);
  if (d2 == NULL)
    return 1;

    printf("about to start loop\n");
    while (1) {
        //printf("millis is %d\n", millis());
        bonjour.run();
        delay(100);
    }

  MHD_stop_daemon(d2);

    return EXIT_SUCCESS;
}

static int mirroring_handler(void * cls,
            struct MHD_Connection * connection,
            const char * url,
            const char * method,
                    const char * version,
            const char * upload_data,
            size_t * upload_data_size,
                    void ** ptr) {
  static int dummy;
  const char * page = (const char *)cls;
  struct MHD_Response * response;
  int ret;

  printf("7100 %s %s with upload data size %d\n", method, url, upload_data_size);

  if (0 != strcmp(method, "GET"))
    return MHD_NO; /* unexpected method */
  if (&dummy != *ptr)
    {
      /* The first time only the headers are valid,
         do not respond in the first round... */
      *ptr = &dummy;
      return MHD_YES;
    }
  if (0 != *upload_data_size)
    return MHD_NO; /* upload data in a GET!? */
  *ptr = NULL; /* clear context pointer */
  response = MHD_create_response_from_data(strlen(page),
                       (void*) page,
                       MHD_NO,
                       MHD_NO);
MHD_add_response_header(response, "Content-Type", "text/x-apple-plist+xml");
  ret = MHD_queue_response(connection,
               MHD_HTTP_OK,
               response);
  MHD_destroy_response(response);
  return ret;
}