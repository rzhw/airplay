#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "EthernetBonjour.h"

#include "blah.h"
#include <time.h>
clock_t start;

#define PAGE "<html><head><title>libmicrohttpd demo</title>"\
             "</head><body>libmicrohttpd demo</body></html>"
#include <microhttpd.h>
#include <string.h>

#define AIRPLAY_SERVICE_PHOTO_FLAG (1 << 1)
#define AIRPLAY_SERVICE_SCREEN_FLAG (1 << 7)

static int ahc_echo(void * cls,
            struct MHD_Connection * connection,
            const char * url,
            const char * method,
                    const char * version,
            const char * upload_data,
            size_t * upload_data_size,
                    void ** ptr);

int main() {
    printf("hello world!\n");

    char txt_buffer[100];
    char mac[6];
    net_get_mac_address(mac);
    snprintf(txt_buffer, sizeof(txt_buffer),
        "deviceid=%02X:%02X:%02X:%02X:%02X:%02X\n"
        "features=0x%x\n"
        "model=uwotm8",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
        AIRPLAY_SERVICE_PHOTO_FLAG & AIRPLAY_SERVICE_SCREEN_FLAG);
    printf("txt is:\n%s\n", txt_buffer);

    start = clock();
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    int err = bonjour.begin("wot");
    bonjour.addServiceRecord("_airplay._tcp", 7000, MDNSServiceTCP, NULL);
    assert(!err);

  struct MHD_Daemon * d;
  d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
               1337,
               NULL,
               NULL,
               &ahc_echo,
               (void *)PAGE,
               MHD_OPTION_END);
  if (d == NULL)
    return 1;

    printf("about to start loop\n");
    while (1) {
        //printf("millis is %d\n", millis());
        bonjour.run();
        delay(100);
    }

  MHD_stop_daemon(d);

    return EXIT_SUCCESS;
}

static int ahc_echo(void * cls,
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
  ret = MHD_queue_response(connection,
               MHD_HTTP_OK,
               response);
  MHD_destroy_response(response);
  return ret;
}