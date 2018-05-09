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
     * @see NETCODE_PLATFORM
     * @returns int 
     */
    int platform();

} // End namespace posit

/*! @} End namespace posit documentation */

#endif // #ifndef POSIT_H
