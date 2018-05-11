/**
 * posit.io daemon implementation
 * 
 * @file positd.cpp
 */

#include "posit.h"

#include <stdio.h>
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

// Instantiate a private key
#define TEST_PROTOCOL_ID 0x1122334455667788

// The main daemon process
int main(int argc, char **argv)
{
    char *server_address = "127.0.0.1:40000";
    if (argc > 1)
    {
        server_address = argv[1];
    }

    if (posit::initialize() != 0)
    {
        printf("error: initialization failed\n");
        return 1;
    }

    struct posit::Server *server;

    while (!quit)
    {
        // Do things
    }

    printf("status: shutting down\n");

    posit::terminate();
}
