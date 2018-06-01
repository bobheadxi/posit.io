/**
 * posit.io library implementation.
 * 
 * @file posit.cpp
 */

#include <cstring>
#include <iostream>

#include "posit.hpp"

extern "C"
{
#include "include/netcode.io/netcode.h"
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

ProtocolOptions::ProtocolOptions(uint64_t setProtocolID, int maxClients)
{
    this->protocolID = setProtocolID;
    this->maxClients = maxClients;
}

// ---------------------------------------------------------------------------------

Server::Server(
    char *address,
    uint8_t *privateKey,
    int keyBytes,
    double time,
    double deltaTime,
    posit::ProtocolOptions *opts)
{
    // Set up properties
    this->time = time;
    this->deltaTime = deltaTime;
    this->maxClients = opts->maxClients;

    // Set up netcode configuration
    struct netcode_server_config_t netcodeServerConfig;
    netcode_default_server_config(&netcodeServerConfig);
    netcodeServerConfig.protocol_id = opts->protocolID;
    memcpy(&netcodeServerConfig.private_key, privateKey, keyBytes);

    // Create server
    this->netcodeServer = netcode_server_create(
        address, &netcodeServerConfig, time);

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
        // if (this->isClientConnected(0))
        // {
        //     this->sendPacketToClient(0, packetData, NETCODE_MAX_PACKET_SIZE);
        // }

        // Receive packets from all connected clients
        for (int clientIndex = 0; clientIndex < this->maxClients; ++clientIndex)
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

                if (packetBytes > 0)
                {
                    // @TODO: do things with packets
                    // std::cout << "Received: " << packet << std::endl;
                }

                this->freePacket(packet);
            }
        }

        // Wait a moment before repeating
        sleep(this->deltaTime);
        this->time += this->deltaTime;
    }
}

void Server::start()
{
    netcode_server_start(this->netcodeServer, this->maxClients);
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
