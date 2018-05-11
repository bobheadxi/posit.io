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

} // End namespace posit

/**
 * @} End namespace posit documentation
 */

#endif // #ifndef POSIT_H
