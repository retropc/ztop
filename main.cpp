/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include "filters.h"
#include "address.h"
#include "counters.h"
#include "protocol.h"
#include "cppcap.h"

int main(void) {
  FilterList f;
  CidrMask c("82.69.158.224/24");
  PCap *p = new PCap();

  p->loop();

  return 0;
}
