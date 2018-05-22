/**
 * posit.io proof-of-concepts
 * 
 * @file poc.cpp
 */

#include "posit.h"

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#include <sodium.h>

// Indicates if an interrupt request has been sent
static volatile int quit = 0;

/**
 * Handles interrupts and signals the server to quit
 * @param signal 
 */
void interrupt_handler(int signal)
{
    (void)signal;
    quit = 1;
}

// Verification stuff for server connection
#define TEST_PROTOCOL_ID 0x1122334455667788
static uint8_t privateKey[32] = {0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
                                 0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
                                 0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                 0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1};

int main(int argc, char **argv)
{
    std::cout << "Testing libsodium link: " << sodium_init() << std::endl;
  
    // Read arg from command line or use a default
    char *serverAddress = (char *)"127.0.0.1:40000";
    if (argc > 1)
    {
        serverAddress = argv[1];
    }

    // Set up time-related constants
    double time = 0.0;
    double delta_time = 1.0 / 60.0;

    // Set up library
    if (posit::initialize() != 0)
    {
        std::cout << "error: initialization failed" << std::endl;
        return 1;
    }

    // Set up server
    struct posit::ServerOptions options = posit::ServerOptions(TEST_PROTOCOL_ID, privateKey, 32);
    struct posit::Server *server = new posit::Server(serverAddress, time, options);
    if (!server)
    {
        std::cout << "error: failed to create server" << std::endl;
        return 1;
    }

    // LET'S GO
    int maxClients = posit::maxClients();
    server->start(maxClients);
    signal(SIGINT, interrupt_handler);

    // Build test packet data
    int maxPacketSize = posit::maxPacketSize();
    uint8_t packetData[maxPacketSize];
    for (int i = 0; i < posit::maxPacketSize(); ++i)
    {
        packetData[i] = (uint8_t)i;
    }

    // Spin until you rudely interrupt as always
    while (!quit)
    {
        // Tick
        server->update(time);

        // Check if client is connected, and attempt to send packet
        if (server->isClientConnected(0))
        {
            server->sendPacketToClient(0, packetData, maxPacketSize);
        }

        // Verify server received packets
        for (int clientIndex = 0; clientIndex < maxClients; ++clientIndex)
        {
            while (1)
            {
                int packetBytes;
                uint64_t packetSequence;
                void *packet = server->receivePacket(clientIndex, &packetSequence, &packetBytes);
                if (!packet)
                {
                    break;
                }
                (void)packetSequence;

                // Checks
                assert(packetBytes == maxPacketSize);
                assert(memcmp(packet, packetData, maxPacketSize) == 0);

                server->freePacket(packet);
            }
        }

        // Wait a moment before repeating
        posit::sleep(delta_time);
        time += delta_time;
    }

    // Goodbye world
    std::cout << "status: shutting down" << std::endl;
    server->destroy();
    posit::terminate();
}
