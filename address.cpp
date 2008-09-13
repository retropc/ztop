/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sstream>

#include "address.h"

using std::string;

/**********************************************
 * AddressParseException
 **********************************************/

AddressParseException::AddressParseException(): std::runtime_error("AddressParseException") {
  /* EMPTY */
}

/**********************************************
 * AddressComparator
 **********************************************/

address_t AddressComparator::string_to_addr(string address) {
  struct in_addr pin;

  memset(&pin, 0, sizeof(pin));

  if(inet_aton(address.c_str(), &pin) != 1)
    throw AddressParseException();

  return PlainAddress::convert(pin);
}

bool AddressComparator::match(string address) {
  return match(this->string_to_addr(address));
}

AddressComparator::~AddressComparator() {
  /* EMPTY */
}

/**********************************************
 * PlainAddress
 **********************************************/

PlainAddress::PlainAddress(address_t address): address(address) {
  /* EMPTY */
}

PlainAddress::PlainAddress(string address) {
  this->address = this->string_to_addr(address);
}

bool PlainAddress::match(address_t address) {
  return address == this->address;
}

address_t PlainAddress::c_addr() {
  return address;
}

PlainAddress *PlainAddress::clone() const {
  return new PlainAddress(*this);
}

address_t PlainAddress::convert(const struct in_addr &address) {
  return ntohl(address.s_addr);
}

string PlainAddress::convert_str(const struct in_addr &address) {
  return inet_ntoa(address);
}

string PlainAddress::convert_str(const address_t &address) {
  struct in_addr pin;

  memset(&pin, 0, sizeof(pin));
  pin.s_addr = ntohl(address);

  return PlainAddress::convert_str(pin);
}

/**********************************************
 * CidrMask
 **********************************************/

mask_t CidrMask::string_to_mask(string mask) {
  mask_t i;
  std::stringstream iss(mask);

  if(!(iss >> i))
    throw AddressParseException();

  return i;
}

void CidrMask::init(address_t address, mask_t mask) {
  if(mask == 0) {
    this->mask = 0;
  } else if(mask > 32) {
    throw AddressParseException();
  } else {
    this->mask = 0xffffffff << (32-mask);
  }

  andedaddress = address & this->mask;
}

CidrMask::CidrMask(address_t address, mask_t mask) {
  init(address, mask);
}

CidrMask::CidrMask(address_t address, string mask) {
  init(address, string_to_mask(mask));
}

CidrMask::CidrMask(string address, mask_t mask) {
  init(string_to_addr(address), mask);
}

CidrMask::CidrMask(string address, string mask) {
  init(string_to_addr(address), string_to_mask(mask));
}

CidrMask::CidrMask(string address) {
  string::size_type slash = address.find("/");
  if(slash == string::npos)
    throw AddressParseException();

  string mask(address, slash + 1, address.length() - slash - 1), address_(address, 0, slash);

  init(string_to_addr(address_), string_to_mask(mask));
}

bool CidrMask::match(address_t addr) {
  return (addr & mask) == this->andedaddress;
}

CidrMask *CidrMask::clone() const {
  return new CidrMask(*this);
}
