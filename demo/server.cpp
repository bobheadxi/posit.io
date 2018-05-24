#include <iostream>
#include <memory>
#include <thread>

#include <cassert>
#include <cstring>
#include <csignal>
#include <cinttypes>

#include "../posit.h"

int demo_positServerStart(char *serverAddress, uint64_t protocolID, uint8_t *privateKey, volatile int *quit)
{
    std::cout << "Preparing posit server" << std::endl;

    // Set up library. 1 is NETCODE_OK.
    std::cout << "Initializing posit library..." << std::endl;
    if (posit::init() != 1)
    {
        std::cout << "error: initialization failed" << std::endl;
        return 1;
    }

    // Give me all the logs
    posit::logLevel(posit::LOG_LEVEL_DEBUG);

    // Protocol (game) configuration
    posit::ProtocolOptions options = posit::ProtocolOptions(
        protocolID, posit::maxClients());

    // Frame time (1/60th of a second) and start time
    double deltaTime = 1.0 / 60.0;
    double time = 0.0;

    std::cout << "Initializing server..." << std::endl; 
    std::unique_ptr<posit::Server> server;
    try
    {
        server = std::make_unique<posit::Server>(
                serverAddress,
                privateKey,
                32,
                time,
                deltaTime,
                &options);
    }
    catch (int e)
    {
        std::cout << "error: failed to create server" << std::endl;
        return e;
    }

    // LET'S GO
    std::cout << "Spinning up server..." << std::endl;
    std::thread serve(&posit::Server::listenAndServe, server.get(), quit);
    std::cout << "Posit listening on " << serverAddress << std::endl;

    // Wait for exit
    serve.join();

    // Goodbye world
    std::cout << "status: shutting down" << std::endl;
    server->destroy();
    posit::terminate();
    return 0;
}
