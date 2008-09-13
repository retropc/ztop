/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#ifndef __CPPCAP_H
#define __CPPCAP_H

#include <pcap.h>

#include <string>
#include <exception>

using std::string;

class PCap {
  friend class PCapException;
private:
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  void init(string device);
public:
  PCap();
  PCap(string device);
  ~PCap();

  void loop();
  void packet(const struct pcap_pkthdr *header, const u_char *data);
};

class PCapException: public std::exception {
private:
  string message;
public:
  PCapException(const PCap *p);
  ~PCapException() throw();

  virtual const char *what() const throw();
};

#endif
