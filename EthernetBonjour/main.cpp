#include <stdlib.h>
#include <stdio.h>
#include "blah.h"

#include "EthernetBonjour.h"

#include <time.h>
clock_t start;

int main() {
    printf("hello world!\n");
    start = clock();
    EthernetBonjourClass bonjour = EthernetBonjourClass();
    bonjour.begin("notarduino");
    printf("about to start loop\n");
    while (1) {
        bonjour.run();
        delay(100);
    }
    return EXIT_SUCCESS;
}
