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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include <assert.h>
#include "airplay.hpp"
#include "plist.hpp"

AirPlayHTTPServer::AirPlayHTTPServer(const AirPlayInfo *info) {
    this->info = info;
    this->daemon = NULL;
}

AirPlayHTTPServer::~AirPlayHTTPServer() {
    if (this->daemon != NULL) {
        MHD_stop_daemon(this->daemon);
    }
}

static int handle_request(void *cls,
        struct MHD_Connection *connection,
        const char *url,
        const char *method,
        const char *version,
        const char *upload_data,
        size_t *upload_data_size,
        void **ptr) {

    static int dummy;
    AirPlayHTTPServer *that = (AirPlayHTTPServer *)cls;
    struct MHD_Response * response;
    int ret;

    printf("7000 %s %s with upload data size %d\n", method, url, upload_data_size);

    if (strcmp(method, "POST") == 0 && strcmp(url, "/reverse") == 0) {
        ret = that->post_reverse(connection);
    } else if (strcmp(method, "GET") == 0 && strcmp(url, "/server-info") == 0) {
        ret = that->get_server_info(connection);
    } else if (strcmp(method, "GET") == 0 && strcmp(url, "/slideshow-features") == 0) {
        ret = that->get_slideshow_features(connection);
    } else if (strcmp(method, "PUT") == 0 && strcmp(url, "/photo") == 0) {
        ret = that->put_photo(connection, (const void *)upload_data, *upload_data_size);
    } else {
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
        const char *page = "u wot mate";
        response = MHD_create_response_from_data(strlen(page),
                (void*) page,
                MHD_NO,
                MHD_NO);
        MHD_add_response_header(response, "Content-Type", "text/x-apple-plist+xml");
        ret = MHD_queue_response(connection,
                MHD_HTTP_OK,
                response);
        MHD_destroy_response(response);
    }
    return ret;
}

int AirPlayHTTPServer::begin() {
    this->daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
            7000,
            NULL,
            NULL,
            &handle_request,
            this,
            MHD_OPTION_END);
    if (this->daemon == NULL)
        return 1;
    return 0;
}

int AirPlayHTTPServer::post_reverse(struct MHD_Connection *connection) {
    const char *session_id = MHD_lookup_connection_value(
            connection, MHD_HEADER_KIND, "X-Apple-Session-ID");
    assert(session_id != NULL); // TODO handle properly
    this->session_id = std::string(session_id);

    printf("session id is %s\n", session_id);

    struct MHD_Response *response = MHD_create_response_from_data(
            0,
            NULL,
            MHD_NO,
            MHD_NO);
    MHD_add_response_header(response, "Upgrade", "PTTH/1.0");
    MHD_add_response_header(response, "Connection", "Upgrade");
    int ret = MHD_queue_response(connection,
            MHD_HTTP_SWITCHING_PROTOCOLS,
            response);
    MHD_destroy_response(response);
    return ret;
}

int AirPlayHTTPServer::get_server_info(struct MHD_Connection *connection) {
    PlistBuilder plist;
    plist.add_string("deviceid", this->info->deviceid);
    plist.add_integer("features", this->info->features);
    plist.add_string("model", this->info->model);
    plist.add_string("protovers", this->info->protovers);
    plist.add_string("srcvers", this->info->srcvers);

    struct MHD_Response *response = MHD_create_response_from_data(
            plist.str().length(),
            (void *)plist.str().c_str(),
            MHD_YES,
            MHD_YES);
    MHD_add_response_header(response, "Content-Type", "text/x-apple-plist+xml");
    int ret = MHD_queue_response(connection,
            MHD_HTTP_OK,
            response);
    MHD_destroy_response(response);
    return ret;
}

int AirPlayHTTPServer::get_slideshow_features(struct MHD_Connection *connection) {
    PlistBuilder themes;
    themes.add_string("key", "Wot");
    themes.add_string("name", "Wot");
    PlistBuilder plist;
    plist.add_array("themes", themes);

    struct MHD_Response *response = MHD_create_response_from_data(
            plist.str().length(),
            (void *)plist.str().c_str(),
            MHD_YES,
            MHD_YES);
    MHD_add_response_header(response, "Content-Type", "text/x-apple-plist+xml");
    int ret = MHD_queue_response(connection,
            MHD_HTTP_OK,
            response);
    MHD_destroy_response(response);
    return ret;
}

int AirPlayHTTPServer::put_photo(struct MHD_Connection *connection, const void *data, size_t size) {
    printf("Received photo, %d bytes\n", size);

    struct MHD_Response *response = MHD_create_response_from_data(
            0,
            NULL,
            MHD_NO,
            MHD_NO);
    int ret = MHD_queue_response(connection,
            MHD_HTTP_OK,
            response);
    MHD_destroy_response(response);
    return ret;
}