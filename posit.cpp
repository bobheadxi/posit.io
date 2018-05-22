/**
 * posit.io library implementation.
 * 
 * @file posit.cpp
 */

#include "posit.h"
#include <cstring>

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

int initialize()
{
    return netcode_init();
}

void terminate()
{
    netcode_term();
}

// ---------------------------------------------------------------------------------

ServerOptions::ServerOptions(uint64_t setProtocolID, uint8_t *setPrivateKey, int setKeyBytes)
{
    this->protocolID = setProtocolID;
    this->privateKeyBytes = setKeyBytes;
    memcpy(this->privateKey, setPrivateKey, setKeyBytes);
}

// ---------------------------------------------------------------------------------

Server::Server(char *address, double time, posit::ServerOptions opts)
{
    // Set up configuration
    struct netcode_server_config_t *netConfig;
    netcode_default_server_config(netConfig);
    netConfig->protocol_id = opts.protocolID;
    memcpy(netConfig->private_key, opts.privateKey, opts.privateKeyBytes);

    // Create server
    this->netcodeServer = netcode_server_create(
        address, netConfig, time);
}

Server::~Server()
{
    this->destroy();
}

void Server::start(int clients)
{
    netcode_server_start(netcodeServer, clients);
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
