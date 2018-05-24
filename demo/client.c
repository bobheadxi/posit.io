
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#include "../include/netcode.io/netcode.h"

#define CONNECT_TOKEN_EXPIRY 30
#define CONNECT_TOKEN_TIMEOUT 5

int demo_clientStart(const char *serverAddress, uint64_t protocolID, uint8_t *privateKey, volatile int *quit)
{
    printf("Preparing client\n");

    if (netcode_init() != NETCODE_OK)
    {
        printf("error: failed to initialize netcode\n");
        return 1;
    }

    // Give me all the logs
    netcode_log_level(NETCODE_LOG_LEVEL_DEBUG);

    // Set up client
    double time = 0.0;
    double deltaTime = 1.0 / 60.0; // frame rate (60/s)
    struct netcode_client_config_t clientConfig;
    netcode_default_client_config(&clientConfig);
    struct netcode_client_t *client = netcode_client_create("::", &clientConfig, time);
    if (!client)
    {
        printf("error: failed to create client\n");
        return 1;
    }

    // Generate client ID
    uint64_t clientID = 0;
    netcode_random_bytes((uint8_t *)&clientID, 8);
    printf("your client id is %.16" PRIx64 "\n", clientID);

    // Generate connect token
    uint8_t connectToken[NETCODE_CONNECT_TOKEN_BYTES];
    int tokenCreateStatus = netcode_generate_connect_token(
        1,                                     // num_server_addresses
        (NETCODE_CONST char**)&serverAddress,  // public addresses
        (NETCODE_CONST char**)&serverAddress,  // internal addresses
        CONNECT_TOKEN_EXPIRY,                  // epiry seconds
        CONNECT_TOKEN_TIMEOUT,                 // timeout seconds
        clientID,                              // client ID
        protocolID,                            // protocol
        0,                                     // sequence
        (NETCODE_CONST uint8_t*)privateKey,    // encrypt key
        connectToken);                         // connect token
    if (tokenCreateStatus != NETCODE_OK)
    {
        printf("error: failed to generate connect token\n");
        return 1;
    }

    // Attempt connection
    printf("Attempting connection to %s...\n", serverAddress);
    netcode_client_connect(client, connectToken);

    // Set up test packet data
    uint8_t packetData[NETCODE_MAX_PACKET_SIZE];
    int i;
    for (i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i)
    {
        packetData[i] = (uint8_t)i;
    }

    // Spin up client
    while (!quit)
    {
        netcode_client_update(client, time);

        // Check if connected, and if connected, send packet
        if (netcode_client_state(client) == NETCODE_CLIENT_STATE_CONNECTED)
        {
            netcode_client_send_packet(client, packetData, NETCODE_MAX_PACKET_SIZE);
        }

        // Receive packet
        while (1)
        {
            int packetBytes;
            uint64_t packetSequence;
            void *packet = netcode_client_receive_packet(client, &packetBytes, &packetSequence);
            if (!packet)
            {
                break;
            }
            (void)packetSequence;

            // @todo do something with client
            assert(packetBytes == NETCODE_MAX_PACKET_SIZE);
            assert(memcmp(packet, packetData, NETCODE_MAX_PACKET_SIZE) == 0);

            // Release resources
            netcode_client_free_packet(client, (uint8_t *)packet);
        }

        // If client has been disconnected, exit
        if (netcode_client_state(client) <= NETCODE_CLIENT_STATE_DISCONNECTED)
        {
            printf("client disconnected\n");
            break;
        }

        // Tick tock
        netcode_sleep(deltaTime);
        time += deltaTime;
    }

    // Clean up
    printf("\nshutting down\n");
    netcode_client_destroy(client);
    netcode_term();
    return 0;
}
