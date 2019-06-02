#ifndef _SA_h
#define _SA_h

#include <cstddef>
#include <string>

#include "libdivsufsort/build/include/divsufsort.h"

class SA {

  public:

  SA(const std::string& text, bool ignoreThis = false);
  ~SA();
  size_t findAllOccurrences(const std::string& pattern);
  std::string getName();
  size_t findIndex(const std::string& pattern);


  private:
  std::string text;
  int *suffix_array;
  SA(SA const &) = delete;
  void operator=(SA const &) = delete;
};

#endif
