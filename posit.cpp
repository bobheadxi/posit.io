/*
  posit.io library implementation
*/

#include "posit.h"

extern "C" {
  #include "netcode.io/netcode.h"
}

namespace posit {
  int poc()
  {
    return NETCODE_PLATFORM;
  }
}
