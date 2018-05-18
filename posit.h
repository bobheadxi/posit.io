/**
 * posit.io library interface.
 * 
 * @file posit.h
 */

#ifndef POSIT_H
#define POSIT_H

/**
 * \addtogroup posit
 * @{
 */

/**
 *  posit.io library functions.
 */
namespace posit
{

    /**
     * Return platform code as detected by netcode.io.
     * 1 = Windows, 2 = Mac, 3 = Unix
     * @returns int 
     */
    int platform();

    /**
     * Netcode max clients
     * @returns int 
     */
    int maxClients();

    /**
     * Netcode max packet size
     * @returns int 
     */
    int maxPacketSize();


    /**
     * Creates posit server.
     * @returns int 
     */
    int initialize();

    /**
     * Destroys posit server.
     * @returns int 
     */
    void terminate();

    /**
     * Declares configuration for posit server.
     */
    struct ServerOptions
    {
        ServerOptions();
        ServerOptions(uint64_t protocolID);
    };

    /**
     * Holds Server properties
     */
    struct ServerState;

    /**
     * An active posit server
     */
    class Server
    {
        public:
            Server(char *address, posit::ServerOptions *opts, double time);
        
        private:
            posit::ServerOptions *options;
            posit::ServerState *state;
    };

    void startServer(posit::Server *server, int clients);
    void updateServer(posit::Server *server, double time);
    void destroyServer(posit::Server *server);
    
    int isClientConnected(posit::Server *server, int clientID);
    void sendPacket(posit::Server *server, int clientID, uint8_t packetData, int packetBytes);

    uint8_t *receivePacket(posit::Server *server, int clientID, uint64_t *packetData, int *packetBytes);
    void freePacket(posit::Server *server, void *packet);

    void sleep(double deltaTime);
} // End namespace posit

/**
 * @} End namespace posit documentation
 */

#endif // #ifndef POSIT_H
