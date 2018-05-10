/**
 * Temporary proof-of-concept demonstrations.
 * 
 * @file poc.cpp
 */

#include "posit.h"
#include <iostream>

/**
 * Proof of concept runner.
 * Use this to test out POCs. Build using `make poc`.
 * @returns int 
 */
int main()
{
    std::cout
        << "Hello Tom!"
        << std::endl
        << "Detected platform: ";

    int platform = posit::platform();
    switch (platform)
    {
    case 1:
        std::cout << "Windows";
        break;
    case 2:
        std::cout << "Mac";
        break;
    case 3:
        std::cout << "Unix";
        break;
    default:
        std::cout << "Unexpected output: " << platform;
    }

    std::cout << std::endl;
    return 0;
}
