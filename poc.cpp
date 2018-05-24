#include <iostream>
#include <memory>
#include <thread>

#include <assert.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <sodium.h>

#include "posit.h"

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

// Protocol ID defines the game
#define TEST_PROTOCOL_ID 0x1122334455667788

// Private key for verification
static uint8_t privateKey[32] = {0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
                                 0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
                                 0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                 0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1};

int main(int argc, char **argv)
{
    std::cout << "Testing libsodium link: " << sodium_init() << std::endl;

    // Read arg from command line or use a default
    char *serverAddress = (char *)"127.0.0.1:40001";
    if (argc > 1)
    {
        serverAddress = argv[1];
    }

    // Set up library. 1 is NETCODE_OK.
    std::cout << "Initializing posit library..." << std::endl;
    if (posit::init() != 1)
    {
        std::cout << "error: initialization failed" << std::endl;
        return 1;
    }

    // Give me all the logs
    posit::logLevel(posit::LOG_LEVEL_DEBUG);

    // Set up server
    std::cout << "Setting up posit::Server..." << std::endl;
    // Protocol (game) configuration
    posit::ProtocolOptions options = posit::ProtocolOptions(
        TEST_PROTOCOL_ID, posit::maxClients());
    double time = 0.0;
    // Frame time (1/60th of a second)
    double delta_time = 1.0 / 60.0;
    std::unique_ptr<posit::Server> server;
    try
    {
        server = std::make_unique<posit::Server>(
                serverAddress,
                privateKey,
                32,
                time,
                delta_time,
                &options);
    }
    catch (int e)
    {
        std::cout << "error: failed to create server" << std::endl;
        return e;
    }

    // Stop server when SIGINT
    signal(SIGINT, interrupt_handler);

    // LET'S GO
    std::cout << "Spinning up server..." << std::endl;
    std::thread serve(&posit::Server::listenAndServe, server.get(), &quit);   
    std::cout << "Posit listening on " << serverAddress << std::endl;

    // Build test packet data
    // @TODO: use to send stuff
    int maxPacketSize = posit::maxPacketSize();
    uint8_t packetData[maxPacketSize];
    for (int i = 0; i < posit::maxPacketSize(); ++i)
    {
        packetData[i] = (uint8_t)i;
    }

    serve.join();

    // Goodbye world
    std::cout << "status: shutting down" << std::endl;
    server->destroy();
    posit::terminate();
}
