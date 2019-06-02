/* C++ wrapper class around a suffix array, backed 
 * by the DivSufSort C implementation of a suffix
 * array. Exports a similar public interface as 
 * our SAVL and SBST classes to make testing easier.
 */

#ifndef _SA_h
#define _SA_h

#include <cstddef>
#include <string>
#include "libdivsufsort/build/include/divsufsort.h"

class SA {

  public:
    SA(const std::string& text, bool splitSpaces = false);
    ~SA();
    size_t findAllOccurrences(const std::string& pattern);
    size_t findIndex(const std::string& pattern);

    // Used by the testing harness.
    static std::string getName();

  private:
    // Private member variables.
    std::string text;
    int *suffix_array;

    /* Fun with C++: disable copy constructor and 
     * operator= to avoid memory issues and undefined
     * behavior.
     */
    SA(SA const &) = delete;
    void operator=(SA const &) = delete;
};

#endif
