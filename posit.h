/**
 * posit.io library interface.
 * 
 * @file posit.h
 */

#ifndef POSIT_H
#define POSIT_H

#include <cstdint>

// Netcode types
struct netcode_server_t;

/**
 * \addtogroup posit
 * @{
 */

/**
 * posit.io library functions.
 */
namespace posit
{

// ---------------------------------------------------------------------------------

/**
 * Return platform code as detected by netcode.io.
 * 1 = Windows, 2 = Mac, 3 = Unix
 * @returns int 
 */
int platform();

/**
 * Netcode max clients.
 * @returns int 
 */
int maxClients();

/**
 * Netcode max packet size.
 * @returns int 
 */
int maxPacketSize();

/**
 * Sets up posit.
 * @returns int 
 */
int initialize();

/**
 * Cleans up posit..
 * @returns int 
 */
void terminate();

// ---------------------------------------------------------------------------------

/**
 * Declares configuration for posit server.
 */
struct ServerOptions
{
  ServerOptions(uint64_t protocolID, uint8_t *privateKey, int keyBytes);

  uint64_t protocolID;
  uint8_t *privateKey;
  int privateKeyBytes;
};

// ---------------------------------------------------------------------------------

/**
 * Posit server class
 */
class Server
{
public:
  Server(char *address, double time, posit::ServerOptions opts);
  ~Server();
  void start(int clients);
  void update(double time);
  void destroy();

  int isClientConnected(int clientID);

  void sendPacketToClient(int clientID, uint8_t *packetData, int packetLength);
  uint8_t *receivePacket(int clientID, uint64_t *packetData, int *packetLength);
  void freePacket(void *packet);

private:
  netcode_server_t *netcodeServer;
};

// ---------------------------------------------------------------------------------

/**
 * Netcode platform-specific implementation of sleep
 * @param seconds 
 */
void sleep(double seconds);

} // End namespace posit

/**
 * @} End namespace posit documentation
 */

#endif // #ifndef POSIT_H
