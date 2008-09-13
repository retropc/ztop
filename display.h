/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "counters.h"

class CounterDisplay {
private:
  PacketCounter *p;
  CounterDisplay();
public:
  CounterDisplay(PacketCounter &p);
  void display();
};

#endif
