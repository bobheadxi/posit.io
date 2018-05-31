#include <iostream>
#include <memory>
#include <thread>

#include <cassert>
#include <cstring>
#include <csignal>
#include <cstdint>

#include "../posit.hpp"

int demo_positServerStart(char *serverAddress, uint64_t protocolID, uint8_t *privateKey, volatile int *quit)
{
    std::cout << "Preparing posit server" << std::endl;

    // Set up library. 1 is NETCODE_OK.
    if (posit::init() != 1)
    {
        std::cout << "error: initialization failed" << std::endl;
        return 1;
    }

    std::cout << "Your key: " << std::endl;
    for(int i = 0; i < 31; i++) {
        std::cout << +privateKey[i] << " ";
    }
    std::cout << std::endl;

    // Give me all the logs
    posit::logLevel(posit::LOG_LEVEL_DEBUG);

    // Protocol (game) configuration
    posit::ProtocolOptions options = posit::ProtocolOptions(
        protocolID, posit::maxClients());

    // Frame time (1/60th of a second) and start time
    double deltaTime = 1.0 / 60.0;
    double time = 0.0;

    // Set up server
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
    std::thread serve(&posit::Server::listenAndServe, server.get(), quit);

    // Wait for exit
    serve.join();

    // Goodbye world
    server->destroy();
    posit::terminate();
    return 0;
}
