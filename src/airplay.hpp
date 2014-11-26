#include <microhttpd.h>

#ifndef __AIRPLAY_H__
#define __AIRPLAY_H__

#define AIRPLAY_SERVICE_PHOTO_FLAG (1 << 1)
#define AIRPLAY_SERVICE_SCREEN_FLAG (1 << 7)

typedef struct _AirPlayInfo {
    char *deviceid;
    int features;
    char *model;
    char *protovers;
    char *srcvers;
} AirPlayInfo;

class AirPlayHTTPServer
{
public:
    AirPlayHTTPServer(const AirPlayInfo *);
    ~AirPlayHTTPServer();
    int begin();
    int get_server_info(struct MHD_Connection *);
    int put_photo();
    const AirPlayInfo *info;
private:
    struct MHD_Daemon *daemon;
};

#endif