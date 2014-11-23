#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "EthernetBonjour.h"

#include "blah.h"
#include <time.h>
clock_t start;

#define AIRPLAY_SERVICE_SCREEN_FLAG (1 << 7)

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
        AIRPLAY_SERVICE_SCREEN_FLAG);
    printf("txt is:\n%s\n", txt_buffer);

    start = clock();
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    int err = bonjour.begin("wot");
    bonjour.addServiceRecord("_airplay._tcp", 7000, MDNSServiceTCP, NULL);
    assert(!err);

    printf("about to start loop\n");
    while (1) {
        //printf("millis is %d\n", millis());
        bonjour.run();
        delay(100);
    }
    return EXIT_SUCCESS;
}
