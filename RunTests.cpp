#include <iostream>
#include "sbst.h"
#include "savl.h"
#include "Timer.h"
#include <functional>
#include <algorithm>
#include <iterator>
#include <unistd.h>

using namespace std;

// Fn prototype
void doCorrectnessTests();

// Constants
const size_t kNotFound = -1;
const size_t kNumRepeats = 20;

// *************************************************************************************
// *************************** BEGIN section for timed tests ***************************
// *************************************************************************************


template <typename T>
double getAverage(const vector<T>& times) {
  return 1.0 * std::accumulate(times.begin(), times.end(), 0) / times.size();
}

template <typename Tree> 
void time_findAll(Tree& tree, const string& text) {
  cout << "Running find all test on " << tree.getName() << " using text = \"" << text << "\"" << endl;

  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats);

  for (size_t i = 0; i < kNumRepeats; i++) {
    Timer timmy;  
    timmy.start();
    auto result = tree.findAllOccurrences(text);
    timmy.stop();
    times.push_back(timmy.elapsed());
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;
}

void time_findAll_both(SBST& sbst, SAVL& savl, const string& text) {
  time_findAll(sbst, text);
  time_findAll(savl, text);
}

template <typename Tree>
void time_constructor(const string& text) {
  Tree nameExtractor("foo");
  cout << "Timing constructor on " << nameExtractor.getName() << " using text = \"" << text << "\"" << endl;
  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats);

  for (size_t i = 0; i < kNumRepeats; i++) {
    Timer timmy;  
    timmy.start();
    Tree tree(text);
    timmy.stop();
    times.push_back(timmy.elapsed());
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;

}

void time_constructor_both(const string& text) {
  time_constructor<SBST>(text);
  time_constructor<SAVL>(text);
}


template <typename Tree>
void time_search(Tree& tree, const string& prefix) {
  cout << "Timing search on " << tree.getName() << " using prefix = \"" << prefix << "\"" << endl;
  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats);
  
  for (size_t i = 0; i < kNumRepeats; i++) {
    Timer timmy;  
    timmy.start();
    tree.findIndex(prefix);
    timmy.stop();
    times.push_back(timmy.elapsed());
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;

}

void time_search_both(SBST& sbst, SAVL& savl, const string& prefix) {
  time_search(sbst, prefix);
  time_search(savl, prefix);
}

void printBeginTest(const string& testName) {
  cout << "--------------------------------------" << endl << endl;
  cout << "BEGIN tests for " << testName << endl << endl;
}

void printEndTest(const string& testName) {
  cout << "END tests for " << testName << endl << endl;
  cout << "--------------------------------------" << endl << endl;
}

void simpleDNATest() {
  const string testName = "Simple DNA Test";
  printBeginTest(testName);

  time_constructor_both("CAATCACGGTCGGAC");

  SAVL savl("CAATCACGGTCGGAC");
  SBST sbst("CAATCACGGTCGGAC");

  time_findAll_both(sbst, savl,  "A");
  time_findAll_both(sbst, savl, "GG");

  time_search_both(sbst, savl, "CA");

  printEndTest(testName);
}

void doTimedTests() {
  simpleDNATest();
}

int main() {
  doCorrectnessTests();
  cout << endl << endl;
  doTimedTests();
}

// ****************************************************
// ******** Correctness tests below this point ******** 
// ****************************************************

void printResult(size_t index, size_t expected) {
  if (index) {
    if (index == expected) cout << "PASS" << endl;
    else cout << "FAIL. Expected " << expected << ", got " << index << endl;
  }
}

void doSearch(SAVL& tree, string prefix, size_t expected) {
  printResult(tree.findIndex(prefix), expected);
}


void doFindAll(SAVL& tree, string prefix, size_t expected) {
  auto res = tree.findAllOccurrences(prefix);
  if (res.size() != expected) {
    cout << "BAD" << endl;
    cout << "got: " << res.size() << endl;
  } else {
    cout << "PASS" << endl;
  }
}

void timeOperation(const string& msg, const std::function<void()>& func) {
  Timer timmy;
  timmy.start();
  func();
  timmy.stop();
  cout << msg << " took: " << timmy.elapsed() << endl;
}


void doCorrectnessTests() {
    {
    SAVL tree("CAATCACGGTCGGAC");
    doSearch(tree, "CGGA", 10);
    doSearch(tree, "AC", 13); // could also be 5 and others

    doSearch(tree, "CGT", kNotFound);

    doSearch(tree, "CAATCACGGTCGGAC", 0);
    doSearch(tree, "C", 4); // could be 0 or others
    doSearch(tree, "T", 3);

    doFindAll(tree, "A", 4);
    doFindAll(tree, "GG", 2);
    doFindAll(tree, "CGG", 2);
    doFindAll(tree, "CGGA", 1);
    doFindAll(tree, "G", 4);

  }

  {

    SAVL tree("AAAAACAAA");
    doFindAll(tree, "A", 8);
    doFindAll(tree, "AA", 6);
    doFindAll(tree, "AAZ", 0);
  }


  {
    SAVL tree("abcdefghijklmnop");
    doSearch(tree, "abc", 0);
    doSearch(tree, "def", 3);
    doSearch(tree, "mnop", 12);
    doSearch(tree, "klmno", 10);
    doSearch(tree, "abcdefghijklmnop", 0);
    doSearch(tree, "z", kNotFound);
  }
}