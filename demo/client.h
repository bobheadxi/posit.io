
#ifndef CLIENT_H
#define CLIENT_H

/**
 * This is based on the reference implementation provided in netcode.io
 * @param serverAddress
 * @param protocolID
 * @param privateKey
 * @param quit
 * @return
 */
int demo_clientStart(const char *serverAddress, uint64_t protocolID, uint8_t *privateKey, volatile int *quit);

#endif // #ifndef CLIENT_H
