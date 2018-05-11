/**
 * posit.io library implementation.
 * 
 * @file posit.cpp
 */

#include "posit.h"

extern "C"
{
#include "vendor/netcode.io/netcode.h"
}

namespace posit
{

int platform()
{
    return NETCODE_PLATFORM;
}

int initialize()
{
    return netcode_init();
}

void terminate()
{
    netcode_term();
}

struct ServerOptions
{
    netcode_server_config_t netcodeServerConfig;

    ServerOptions()
    {
        // Set up with netcode defaults
        netcode_default_server_config(&netcodeServerConfig);
    }

    ServerOptions(uint64_t protocolID)
    {
        ServerOptions();
        netcodeServerConfig.protocol_id = protocolID;
    }
};

struct ServerState
{

};

} // End namespace posit
