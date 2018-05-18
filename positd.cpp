/**
 * posit.io daemon implementation
 * 
 * @file positd.cpp
 */

#include "posit.h"

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

// Indicates if the an interrupt request has been sent
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

#define TEST_PROTOCOL_ID 0x1122334455667788

// The main daemon process
int main(int argc, char **argv)
{
    char *serverAddress = "127.0.0.1:40000";
    if (argc > 1)
    {
        serverAddress = argv[1];
    }

    double time = 0.0;
    double delta_time = 1.0 / 60.0;

    // Set up library
    if (posit::initialize() != 0)
    {
        std::cout << "error: initialization failed" << std::endl;
        return 1;
    }

    // Set up server
    struct posit::ServerOptions *options = new posit::ServerOptions();
    struct posit::Server *server = new posit::Server(serverAddress, options, time);
    if (!server)
    {
        std::cout << "error: failed to create server" << std::endl;
        return 1;
    }

    // LET'S GO
    int maxClients = posit::maxClients();
    posit::startServer(server, maxClients);
    signal(SIGINT, interrupt_handler);

    int maxPacketSize = posit::maxPacketSize();
    uint8_t packet_data[maxPacketSize];
    int i;
    for (i = 0; i < posit::maxPacketSize(); ++i)
    {
        packet_data[i] = (uint8_t)i;
    }

    while (!quit)
    {
        posit::startServer(server, time);

        if (posit::isClientConnected(server, 0))
        {
            posit::sendPacket(server, 0, packet_data, maxPacketSize);
        }

        int clientID;
        for (clientID = 0; clientID < maxClients; ++clientID)
        {
            while (1)
            {
                int packetBytes;
                uint64_t packetSequence;
                void *packet = posit::receivePacket(server, clientID, &packetSequence, &packetBytes);
                if (!packet)
                    break;
                (void)packetSequence;
                assert(packetBytes == maxPacketSize);
                assert(memcmp(packet, packet_data, maxPacketSize) == 0);

                posit::freePacket(server, packet);
            }
        }

        posit::sleep(delta_time);
        time += delta_time;
    }

    printf("status: shutting down\n");

    posit::destroyServer(server);
    posit::terminate();
}
