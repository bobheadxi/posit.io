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

int init()
{
    return netcode_init();
}

void terminate()
{
    netcode_term();
}

// ---------------------------------------------------------------------------------

ProtocolOptions::ProtocolOptions(uint64_t setProtocolID)
{
    this->protocolID = setProtocolID;
}

// ---------------------------------------------------------------------------------

Server::Server(char *address, uint8_t *privateKey, int keyBytes, double time, posit::ProtocolOptions *opts)
{
    // Set up configuration
    struct netcode_server_config_t netConfig;
    netcode_default_server_config(&netConfig);
    netConfig.protocol_id = opts->protocolID;
    memcpy(netConfig.private_key, privateKey, keyBytes);

    // Create server
    this->netcodeServer = netcode_server_create(
        address, &netConfig, time);
}

Server::~Server()
{
    // This should be manually called by the creator.
    // this->destroy();
}

void Server::start(int clients)
{
    netcode_server_start(this->netcodeServer, clients);
}

void Server::update(double time)
{
    netcode_server_update(this->netcodeServer, time);
}

void Server::destroy()
{
    netcode_server_destroy(this->netcodeServer);
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

void sendPacketToServer(posit::Server *server, int clientIndex, uint8_t packetData, int packetBytes)
{

}

void sleep(double seconds)
{
    netcode_sleep(seconds);
}

} // End namespace posit
