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
#include "display.h"

#include <time.h>

#include <iostream>

using std::cout;

class FilterHandler: public PCapIPv4PacketHandler {
private:
  FilterList *f;
  PacketCounter *c;
  CounterDisplay *d;
  time_t last;

  void execute(const PCapIPv4Packet &p) {
    address_t src, dst;
    long len = p.ipheader->ip_len - IP_HL(p.ipheader);

    src = PlainAddress::convert(p.ipheader->ip_src);
    dst = PlainAddress::convert(p.ipheader->ip_dst);

    if(f->execute(src)) {
      c->increase(src, UPLOAD, len);
    } else {
      if(f->execute(dst))
        c->increase(dst, DOWNLOAD, len);
    }

    time_t t = time(NULL);
    if(t - last >= 1) {
      last = t;
      d->display();
    }
  }
  FilterHandler();
public:
  FilterHandler(FilterList &f, PacketCounter &c, CounterDisplay &d): last(0) {
    this->f = &f;
    this->c = &c;
    this->d = &d;
  }
};

int main(void) {
  AddressComparator *c[] = {
#define cidr(x) new CidrMask(x),
#define addr(x) new PlainAddress(x),

#include "filters.zpp"

#undef cidr
#undef addr
    0,
  };
  PCap *p = new PCap();

  FilterList *f = new FilterList();

  for(AddressComparator **cl=c;*cl;cl++) {
    f->add(**cl);
    delete *cl;
  }

  PacketCounter *cc = new PacketCounter();
  CounterDisplay d(*cc);

  FilterHandler *ff = new FilterHandler(*f, *cc, d);

  p->ipv4handler = ff;
  p->loop();

  delete ff;
  delete f;
  delete cc;
  delete p;

  return 0;
}
