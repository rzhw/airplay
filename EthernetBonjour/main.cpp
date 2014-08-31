#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "EthernetBonjour.h"

#include "blah.h"
#include <time.h>
clock_t start;

int main() {
    printf("hello world!\n");
    start = clock();
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    int err = bonjour.begin("notarduino");
    bonjour.addServiceRecord("My Not Arduino Web Server._http", 1337, MDNSServiceTCP);
    assert(!err);
    printf("about to start loop\n");
    while (1) {
        //printf("millis is %d\n", millis());
        bonjour.run();
        delay(100);
    }
    return EXIT_SUCCESS;
}
