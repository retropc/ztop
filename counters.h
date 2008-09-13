/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#ifndef __COUNTERS_H
#define __COUNTERS_H

#include <sys/types.h>

#include <map>

#include "address.h"

typedef u_int64_t count_t;

class PacketCounter {
private:
  std::map<address_t, count_t> counts;

public:
  PacketCounter();
  void increase(address_t address, count_t size);
};

#endif
