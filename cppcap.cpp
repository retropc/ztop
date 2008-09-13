/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include "cppcap.h"

#include <sstream>
#include <iostream>

PCapException::PCapException(const PCap *p) {
  std::ostringstream s;
  s << "PCapException: " << p->errbuf;

  message = s.str();
}

const char *PCapException::what() const throw() {
  return message.c_str();
}

PCapException::~PCapException() throw() {
  /* EMPTY */
}

PCap::PCap() {
  char *device = pcap_lookupdev(errbuf);

  if(!device)
    throw PCapException(this);

  init(device);
}

PCap::PCap(string device) {
  init(device);
}

PCap::~PCap() {
  if(handle)
    pcap_close(handle);
}

void PCap::init(string device) {
  handle = pcap_open_live(device.c_str(), BUFSIZ, 0, 1000, errbuf);
  if(!handle)
    throw PCapException(this);
}

extern "C" void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
  PCap *p = (PCap *)args;

  p->packet(header, packet);
}

void PCap::loop() {
  pcap_loop(handle, 0, got_packet, (u_char *)this);
}

void PCap::packet(const struct pcap_pkthdr *header, const u_char *data) {
  std::cout << "got zee packet!!\n";
}

