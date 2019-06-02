/* Implements the interface defined in sa.h, backed by
 * the DivSufSort suffix array.
 */

#include "sa.h"
using std::string;

SA::SA(const string& text, bool splitSpaces) : text(text) {
  suffix_array = (int *)malloc(text.length() * sizeof(int));
  divsufsort((unsigned char *)text.c_str(), suffix_array, text.length());
}

SA::~SA() {
  free(suffix_array);
}

size_t SA::findAllOccurrences(const string& pattern) {
  return sa_search((unsigned char *)text.c_str(), text.length(), (unsigned char *)pattern.c_str(), 
      pattern.size(), suffix_array, text.length(), nullptr);
}


size_t SA::findIndex(const std::string& pattern) {
  int32_t output_index;
  sa_search((unsigned char *)text.c_str(), text.length(), (unsigned char *)pattern.c_str(), 
      pattern.size(), suffix_array, text.length(), &output_index);
  return output_index;
}

std::string SA::getName() {
  return "SA";
}
