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
#include <utility>

#include "address.h"

typedef u_int64_t count_t;

enum direction { UPLOAD, DOWNLOAD };

typedef std::map<address_t, std::pair<count_t, count_t> > PacketCounterMap;
class PacketCounter {
private:
  PacketCounterMap counts;
public:
  PacketCounter();
  void increase(address_t address, direction d, count_t size);
  PacketCounterMap::const_iterator begin() const;
  PacketCounterMap::const_iterator end() const;
};

#endif
