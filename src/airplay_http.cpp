#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include "airplay.hpp"

#define PAGE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"\
"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\""\
" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"\
"<plist version=\"1.0\">"\
" <dict>"\
"  <key>themes</key>"\
"  <array>"\
"   <dict>"\
"    <key>key</key>"\
"    <string>Wot</string>"\
"    <key>name</key>"\
"    <string>Wot</string>"\
"   </dict>"\
"  </array>"\
" </dict>"\
"</plist>"

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

    if (strcmp(method, "GET") == 0 && strcmp(url, "/server-info") == 0) {
        ret = that->get_server_info(connection);
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
        const char *page = PAGE;
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

int AirPlayHTTPServer::get_server_info(struct MHD_Connection *connection) {
    char plist[500];
    snprintf(plist, sizeof(plist),
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"\n"\
" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"\
"<plist version=\"1.0\">\n"\
" <dict>\n"\
"  <key>deviceid</key>\n"\
"  <string>%s</string>\n"\
"  <key>features</key>\n"\
"  <integer>%d</integer>\n"\
"  <key>model</key>\n"\
"  <string>%s</string>\n"\
"  <key>protovers</key>\n"\
"  <string>%s</string>\n"\
"  <key>srcvers</key>\n"\
"  <string>%s</string>\n"\
" </dict>\n"\
"</plist>",
        this->info->deviceid,
        this->info->features,
        this->info->model,
        this->info->protovers,
        this->info->srcvers);

    struct MHD_Response *response = MHD_create_response_from_data(
            strlen(plist),
            (void *)plist,
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