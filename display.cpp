/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include <iostream>

#include "display.h"
#include "address.h"

using std::cout;

CounterDisplay::CounterDisplay(PacketCounter &p) {
  this->p = &p;
}

void CounterDisplay::display() {
  PacketCounterMap::const_iterator pi, end;

  cout << "-( counts )-------------------------------------\n";

  for(pi=p->begin(),end=p->end();pi!=end;pi++) {
    address_t addr = pi->first;

    /* p->second == pair<count_t, count_t> */
    count_t upload = pi->second.first, download = pi->second.second;

    cout << PlainAddress::convert_str(addr) << ": u(" << upload << "b) d(" << download << "b)\n";
  }
}
