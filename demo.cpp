#include <iostream>
#include <memory>
#include <thread>

#include <cassert>
#include <cstring>
#include <csignal>
#include <cinttypes>
#include <sodium.h>

#include "demo/server.hpp"
extern "C"
{
#include "demo/client.h"
}

// Indicates if an interrupt request has been sent
static volatile int quit = 0;

/**
 * Handles interrupts and signals server/client process to quit
 * @param signal 
 */
void interrupt_handler(int signal)
{
    (void)signal;
    quit = 1;
}

// Protocol ID defines the game
#define TEST_PROTOCOL_ID 0x1122334455667788

// Private key for verification
static uint8_t privateKey[32] = {0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
                                 0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
                                 0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                 0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1};

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Insufficient arguments (specify 'client' or 'server')" << std::endl;
        return 1;
    }

    // Read arg from command line or use a default
    char *serverAddress = (argc != 3) ? (char *)"[::1]:40000" : argv[2];

    // Stop process when SIGINT
    signal(SIGINT, interrupt_handler);

    // Start appropriate process
    if (std::strcmp(argv[1], "server") == 0)
    {
        return demo_positServerStart(serverAddress, TEST_PROTOCOL_ID, privateKey, &quit);
    }
    if (std::strcmp(argv[1], "client") == 0)
    {
        return demo_clientStart(serverAddress, TEST_PROTOCOL_ID, privateKey, &quit);
    }

    std::cout << "Unrecognized argument " << argv[1] << std::endl;
    return 1;
}
