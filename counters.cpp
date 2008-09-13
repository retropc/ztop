/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include "counters.h"

PacketCounter::PacketCounter() : counts() {
  /* EMPTY */
}

void PacketCounter::increase(address_t address, count_t size) {
  counts[address]+=size;
}
