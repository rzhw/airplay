#include <string>
#include <microhttpd.h>

#ifndef __AIRPLAY_H__
#define __AIRPLAY_H__

#define AIRPLAY_SERVICE_PHOTO_FLAG (1 << 1)
#define AIRPLAY_SERVICE_SCREEN_FLAG (1 << 7)

typedef struct _AirPlayInfo {
    const char *deviceid;
    int features;
    const char *model;
    const char *protovers;
    const char *srcvers;
} AirPlayInfo;

class AirPlayHTTPServer
{
public:
    AirPlayHTTPServer(const AirPlayInfo *);
    ~AirPlayHTTPServer();
    int begin();
    int post_reverse(struct MHD_Connection *);
    int get_server_info(struct MHD_Connection *);
    int get_slideshow_features(struct MHD_Connection *);
    int put_photo(struct MHD_Connection *, const void *, size_t);
    const AirPlayInfo *info;
private:
    struct MHD_Daemon *daemon;
    std::string session_id;
};

#endif