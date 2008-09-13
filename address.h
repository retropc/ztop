/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#ifndef __ADDRESS_H
#define __ADDRESS_H

#include <sys/types.h>

#include <stdexcept>
#include <string>

typedef u_int32_t address_t;
typedef unsigned short mask_t;

class AddressParseException: public std::runtime_error {
public:
  AddressParseException();
};

class AddressComparator {
protected:
  address_t string_to_addr(std::string addr);
public:
  virtual bool match(address_t addr) = 0;
  virtual AddressComparator *clone() const = 0;
  bool match(std::string addr);
};

class PlainAddress: public AddressComparator {
private:
  address_t address;
  PlainAddress();
public:
  PlainAddress(address_t address);
  PlainAddress(std::string address);

  address_t c_addr();

  using AddressComparator::match;
  bool match(address_t address);

  PlainAddress *clone() const;

  static address_t convert(const struct in_addr &address);
  static std::string convert_str(const struct in_addr &address);
  static std::string convert_str(const address_t &address);
};

class CidrMask: public AddressComparator {
private:
  address_t andedaddress;
  address_t mask; 
  mask_t string_to_mask(std::string mask);
  CidrMask();
  void init(address_t address, mask_t mask);
public:
  CidrMask(address_t address, mask_t mask);
  CidrMask(address_t address, std::string mask);
  CidrMask(std::string address, mask_t mask);
  CidrMask(std::string address, std::string mask);
  CidrMask(std::string address);

  using AddressComparator::match;
  bool match(address_t address);

  CidrMask *clone() const;
};

#endif
