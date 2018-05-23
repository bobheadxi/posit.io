/**
 * posit.io library implementation.
 * 
 * @file posit.cpp
 */

#include "posit.h"
#include <cstring>
#include <iostream>

extern "C"
{
#include "vendor/netcode.io/netcode.h"
}

namespace posit
{

// ---------------------------------------------------------------------------------

int platform()
{
    return NETCODE_PLATFORM;
}

int maxClients()
{
    return NETCODE_MAX_CLIENTS;
}

int maxPacketSize()
{
    return NETCODE_MAX_PACKET_SIZE;
}

void logLevel(int level)
{
    netcode_log_level(level);
}

int init()
{
    return netcode_init();
}

void terminate()
{
    netcode_term();
}

// ---------------------------------------------------------------------------------

ProtocolOptions::ProtocolOptions(uint64_t setProtocolID, int clients)
{
    this->protocolID = setProtocolID;
    this->clients = clients;
}

// ---------------------------------------------------------------------------------

Server::Server(
    char *address,
    uint8_t *privateKey,
    int keyBytes,
    double time,
    double delta_time,
    posit::ProtocolOptions *opts
) {
    // Set up properties
    this->time = time;
    this->delta_time = delta_time;
    this->clients = opts->clients;

    // Set up netcode configuration
    struct netcode_server_config_t netConfig;
    netcode_default_server_config(&netConfig);
    netConfig.protocol_id = opts->protocolID;
    memcpy(netConfig.private_key, privateKey, keyBytes);

    // Create server
    this->netcodeServer = netcode_server_create(
        address, &netConfig, time);

    // Check if successful
    if (!this->netcodeServer)
    {
        throw 1;
    }
}

Server::~Server()
{
    // This should be manually called by the creator.
    // this->destroy();
}

void Server::destroy()
{
    netcode_server_destroy(this->netcodeServer);
}

void Server::listenAndServe(volatile int *quit)
{
    this->start();
    while (!*quit)
    {
        // Tick
        this->update();

        // @TODO: Check if client is connected, and attempt to send packet something
        // in a configurable manner
        std::cout << "No client connected - skipping client packet send\n";

        // Receive packets from all connected clients
        for (int clientIndex = 0; clientIndex < this->clients; ++clientIndex)
        {
            while (1)
            {
                int packetBytes;
                uint64_t packetSequence;
                void *packet = this->receivePacket(clientIndex, &packetSequence, &packetBytes);
                if (!packet)
                {
                    break;
                }
                (void)packetSequence;

                // @TODO: do things with packets
                if (packetBytes > 0) {
                    std::cout << "Received: " << packet << std::endl;
                }

                this->freePacket(packet);
            }
        }

        // Wait a moment before repeating
        sleep(this->delta_time);
        this->time += this->delta_time;
    }
}

void Server::start()
{
    std::cout << (this->netcodeServer) << std::endl;
    netcode_server_start(this->netcodeServer, this->clients);
}

void Server::update()
{
    netcode_server_update(this->netcodeServer, this->time);
}


int Server::isClientConnected(int clientIndex)
{
    return netcode_server_client_connected(
        this->netcodeServer,
        clientIndex);
}

void Server::sendPacketToClient(int clientIndex, uint8_t *packetData, int packetLength)
{
    netcode_server_send_packet(this->netcodeServer, clientIndex, packetData, packetLength);
}

uint8_t *Server::receivePacket(int clientIndex, uint64_t *packetData, int *packetLength)
{
    return netcode_server_receive_packet(
        this->netcodeServer,
        clientIndex,
        packetLength,
        packetData);
}

void Server::freePacket(void *packet)
{
    netcode_server_free_packet(this->netcodeServer, packet);
}

// ---------------------------------------------------------------------------------

void sleep(double seconds)
{
    netcode_sleep(seconds);
}

} // End namespace posit
