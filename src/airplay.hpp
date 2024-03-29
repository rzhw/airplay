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