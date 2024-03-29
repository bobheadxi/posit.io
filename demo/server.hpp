
#pragma once

#include <cinttypes>

/**
 * Spin up a demo Posit instance
 * @param serverAddress
 * @param protocolID
 * @param privateKey
 * @param quit
 * @return
 */
int demo_positServerStart(char *serverAddress, uint64_t protocolID, uint8_t *privateKey, volatile int *quit);
