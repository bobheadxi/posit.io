/*
  posit.io library implementation
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
}
