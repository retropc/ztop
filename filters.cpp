/*
 * Copyright (C) Chris Porter 2008.
 * All rights reserved.
 *
 * See LICENSE for licensing information.
 */

#include "filters.h"

FilterList::FilterList(): filters() {
  /* EMPTY */
}

FilterList::~FilterList() {
  std::vector<AddressComparator *>::iterator itt;

  for(itt=filters.begin();itt!=filters.end();itt++)
    delete *itt;
}

void FilterList::add(AddressComparator &filter) {
  AddressComparator *f = filter.clone();

  filters.push_back(f);
}

bool FilterList::execute(address_t address) {
  std::vector<AddressComparator *>::iterator itt;

  for(itt=filters.begin();itt!=filters.end();itt++)
    if((*itt)->match(address))
      return true;

  return false;
}
