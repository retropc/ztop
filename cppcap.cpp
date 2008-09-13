/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include <sstream>

#include "cppcap.h"

using std::string;

/**********************************************
 * PCapException
 **********************************************/

PCapException::PCapException(const PCap *p) {
  std::ostringstream s;
  s << "PCapException: " << p->errbuf;

  message = s.str();
}

PCapException::PCapException(const string &p) {
  message = p;
}

const char *PCapException::what() const throw() {
  return message.c_str();
}

PCapException::~PCapException() throw() {
}

/**********************************************
 * PCap
 **********************************************/

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
  handle = pcap_open_live(const_cast<char *>(device.c_str()), BUFSIZ, 0, 1000, errbuf);
  if(!handle)
    throw PCapException(this);
}

extern "C" void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
  PCap *p = reinterpret_cast<PCap *>(args);

  p->decode(header, packet);
}

void PCap::loop() {
  pcap_loop(handle, 0, got_packet, reinterpret_cast<u_char *>(this));
}

void PCap::decode(const struct pcap_pkthdr *header, const u_char *data) {
  PCapEthernetFrame p(header, data);

  switch(p.ethernetheader->ether_type) {
    case PROTOCOL_IP:
      {
        PCapIPv4Packet ipp(p);

        if(ipv4handler != NULL)
          ipv4handler->execute(ipp);

      }

      break;
    default:
      break;
  }
}

/**********************************************
 * PCapEthernetFrame
 **********************************************/

PCapEthernetFrame::PCapEthernetFrame(const PCapEthernetFrame &packet) {
  init(packet.caplength, packet.length, packet.packet);
}

PCapEthernetFrame::PCapEthernetFrame(const struct pcap_pkthdr *header, const u_char *packet) {
  init(header, packet);
}

void PCapEthernetFrame::init(const struct pcap_pkthdr *header, const u_char *packet) {
  init(header->caplen, header->len, packet);
}

void PCapEthernetFrame::init(bpf_u_int32 caplength, bpf_u_int32 length, const u_char *packet) {
  this->packet = 0;

  this->caplength = caplength;
  this->length = length;

  if(caplength < sizeof(struct sniff_ethernet))
    throw PCapException("Invalid ethernet frame length.");

  this->packet = new u_char[caplength];

  memcpy(const_cast<u_char *>(this->packet), packet, caplength);

  ethernetheader = reinterpret_cast<const struct sniff_ethernet *>(this->packet);

  l2headerlength = SIZE_ETHERNET;  
  l2payload = this->packet + l2headerlength;
}

PCapEthernetFrame &PCapEthernetFrame::operator=(const PCapEthernetFrame &p) {
  if(&p == this) /* self assignment */
    return *this;

  delete[] this->packet;

  init(p.caplength, p.length, p.packet);

  return *this;
}

PCapEthernetFrame::~PCapEthernetFrame() {
  delete[] this->packet;
}

/**********************************************
 * PCapIPv4Packet
 **********************************************/

PCapIPv4Packet::PCapIPv4Packet(const PCapEthernetFrame &packet): PCapEthernetFrame(packet) {
  init(packet);
}

void PCapIPv4Packet::init(const PCapEthernetFrame &packet) {
  if(caplength - l2headerlength < sizeof(struct sniff_ip))
    throw PCapException("Invalid ip packet length.");

  ipheader = reinterpret_cast<const struct sniff_ip *>(l2payload);

  l3headerlength = IP_HL(ipheader) * 4;

  if(l3headerlength < 20)
    throw PCapException("Invalid IP header length.");

  l3payload = this->packet + l2headerlength + l3headerlength;
}

/**********************************************
 * PCapIPv4PacketHandler
 **********************************************/

PCapIPv4PacketHandler::~PCapIPv4PacketHandler() {
  /* EMPTY */
}
