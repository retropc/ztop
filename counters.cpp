/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include "counters.h"

PacketCounter::PacketCounter(): counts() {
  /* EMPTY */
}

void PacketCounter::increase(address_t address, direction d, count_t size) {
  std::pair<count_t, count_t> &p = counts[address];

  if(d == UPLOAD) {
    p.first+=size;
  } else {
    p.second+=size;
  }
}

PacketCounterMap::const_iterator PacketCounter::begin() const {
  return counts.begin();
}

PacketCounterMap::const_iterator PacketCounter::end() const {
  return counts.end();
}
