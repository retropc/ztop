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

#include "address.h"
#include "packetstruct.h"

class PCapIPv4PacketHandler;

class PCap {
  friend class PCapException;
private:
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  void init(std::string device);
public:
  PCapIPv4PacketHandler *ipv4handler;

  PCap();
  PCap(std::string device);
  ~PCap();

  void loop();
  void decode(const struct pcap_pkthdr *header, const u_char *data);
};

class PCapException: public std::exception {
private:
  std::string message;
public:
  PCapException(const PCap *p);
  PCapException(const std::string &p);
  ~PCapException() throw();

  virtual const char *what() const throw();
};

class PCapEthernetFrame {
  friend class PCap;
private:
  PCapEthernetFrame();

  void init(const struct pcap_pkthdr *header, const u_char *packet);
  void init(bpf_u_int32 caplength, bpf_u_int32 length, const u_char *packet);
protected:
  struct timeval ts;
  bpf_u_int32 caplength;
  bpf_u_int32 length;

  const u_char *packet;

  const u_char *l2payload;
  bpf_u_int32 l2headerlength;

public:
  const struct sniff_ethernet *ethernetheader;

  PCapEthernetFrame(const PCapEthernetFrame &p);
  PCapEthernetFrame(const struct pcap_pkthdr *header, const u_char *packet);
  PCapEthernetFrame &operator=(const PCapEthernetFrame &p);
  ~PCapEthernetFrame();
};

class PCapPacketHandler {
};

class PCapIPv4Packet: public PCapEthernetFrame {
  friend class PCap;
private:
  const u_char *l3payload;
  bpf_u_int32 l3headerlength;

  PCapIPv4Packet();
  void init(const PCapEthernetFrame &packet);
public:
  const struct sniff_ip *ipheader;

  PCapIPv4Packet(const PCapEthernetFrame &packet);
};

class PCapIPv4PacketHandler: public PCapPacketHandler {
public:
  virtual void execute(const PCapIPv4Packet &packet) = 0;
  virtual ~PCapIPv4PacketHandler();
};

#endif
