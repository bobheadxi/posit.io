/**
 * posit.io library interface.
 * 
 * @file posit.h
 */

#ifndef POSIT_HPP
#define POSIT_HPP

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

const int LOG_LEVEL_NONE = 0;
const int LOG_LEVEL_ERROR = 1;
const int LOG_LEVEL_INFO = 2;
const int LOG_LEVEL_DEBUG = 3;

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
 * Sets log verbosity.
 * @param level
 */
void logLevel(int level);

/**
 * Sets up posit.
 * @returns int 
 */
int init();

/**
 * Cleans up posit..
 * @returns int 
 */
void terminate();

// ---------------------------------------------------------------------------------

/**
 * Declares configuration for posit server. This will contain all configuration
 * pertaining to the game - movement schemas, entities, hook subscriptions, etc.
 */
struct ProtocolOptions
{
  ProtocolOptions(uint64_t protocolID, int maxClients);

  uint64_t protocolID;
  int maxClients;
};

// ---------------------------------------------------------------------------------

/**
 * Posit server class
 */
class Server
{
public:
  Server(
      char *address,
      uint8_t *privateKey,
      int keyBytes,
      double time,
      double delta_time,
      posit::ProtocolOptions *opts);
  ~Server();
  void listenAndServe(volatile int *quit);
  void destroy();

  int isClientConnected(int clientID);
  void sendPacketToClient(int clientID, uint8_t *packetData, int packetLength);

private:
  void start();
  void update();
  uint8_t *receivePacket(int clientID, uint64_t *packetData, int *packetLength);
  void freePacket(void *packet);

  netcode_server_t *netcodeServer;
  double time;
  double deltaTime;
  int maxClients;
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

#endif // #ifndef POSIT_HPP
