/**
 * posit.io library implementation.
 * 
 * @file posit.cpp
 */

#include "posit.h"

extern "C" {
  #include "vendor/netcode.io/netcode.h"
}

namespace posit {
  int platform()
  {
    return NETCODE_PLATFORM;
  }
} // End namespace posit
