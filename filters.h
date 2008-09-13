#ifndef __FILTERS_H
#define __FILTERS_H

#include <vector>

#include "address.h"

class FilterList {
private:
  std::vector<AddressComparator *> filters;
public:
  FilterList();
  ~FilterList();
  void add(AddressComparator &filter);
  bool execute(address_t address);
};

#endif
