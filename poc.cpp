/*
    Temporary proof-of-concept demonstrator.
*/

#include "posit.h"
#include <iostream>

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
