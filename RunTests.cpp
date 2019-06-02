#include <iostream>
#include "sbst.h"
#include "savl.h"
#include "Timer.h"
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <fstream>

using namespace std;

// Fn prototype
void doCorrectnessTests();

// Constants
const size_t kNotFound = -1;
const size_t kNumRepeats = 1;

// *************************************************************************************
// *************************** BEGIN section for timed tests ***************************
// *************************************************************************************

int randomInteger(int start, int end) {
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_int_distribution<> distr(start, end); // define the range
  return distr(eng);
}


template <typename T>
double getAverage(const vector<T>& times) {
  return 1.0 * std::accumulate(times.begin(), times.end(), 0) / times.size();
}

template <typename Tree>
void time_findAll(Tree& tree, const vector<string>& words, const string& vecName) {
  cout << "Running find all test on " << tree.getName() << " using list \"" << vecName << "\"" << endl;

  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats * words.size());

  for (const string& word: words) {
    for (size_t i = 0; i < kNumRepeats; i++) {
      Timer timmy;  
      timmy.start();
      (void) tree.findAllOccurrences(word);
      timmy.stop();
      times.push_back(timmy.elapsed());
    }
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;
}

void time_findAll_both(SBST& sbst, SAVL& savl, const vector<string>& words, const string& vecName) {
  time_findAll(sbst, words, vecName);
  time_findAll(savl, words, vecName);
}

template <typename Tree>
void time_constructor(const string& text, bool spacesSplit) {
  Tree nameExtractor("foo");
  cout << "Timing constructor on " << nameExtractor.getName() << endl;
  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats);

  Timer timmy;  
  timmy.start();
  Tree tree(text, spacesSplit);
  timmy.stop();

  cout << "    --> Time: " << timmy.elapsed() << endl << endl;

}

void time_constructor_both(const string& text, bool spacesSplit = false) {
  time_constructor<SBST>(text, spacesSplit);
  time_constructor<SAVL>(text, spacesSplit);
}


template <typename Tree>
void time_findIndex(Tree& tree, const vector<string>& words, const string& vecName) {
  cout << "Running find index test on " << tree.getName() << " using list \"" << vecName << "\"" << endl;
  std::vector<std::uint64_t> times;
  times.reserve(kNumRepeats * words.size());

  for (const string& word: words) {
    for (size_t i = 0; i < kNumRepeats; i++) {
      Timer timmy;  
      timmy.start();
      (void) tree.findIndex(word);
      timmy.stop();
      times.push_back(timmy.elapsed());
    }
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;

}

void time_findIndex_both(SBST& sbst, SAVL& savl, const vector<string>& words, const string& vecName) {
  time_findIndex(sbst, words, vecName);
  time_findIndex(savl, words, vecName);
}

void printBeginTest(const string& testName) {
  cout << "--------------------------------------" << endl << endl;
  cout << "BEGIN tests for " << testName << endl << endl;
}

void printEndTest(const string& testName) {
  cout << "END tests for " << testName << endl << endl;
  cout << "--------------------------------------" << endl << endl;
}

// *******************************************************
// ******* BEGIN test cases, each in 1 function **********
// *******************************************************

void simpleDNATest() {
  const string testName = "Simple DNA Test";
  printBeginTest(testName);

  time_constructor_both("CAATCACGGTCGGAC");

  SBST sbst("CAATCACGGTCGGAC");
  SAVL savl("CAATCACGGTCGGAC");

  time_findAll_both(sbst, savl, {"A"},  "one element A");
  time_findAll_both(sbst, savl, {"GG"}, "one element G");

  time_findIndex_both(sbst, savl, {"CA"}, "one element CA");

  printEndTest(testName);
}

void hugeAlphabetTest() {
  const string testName = "Huge Alphabet Tests";
  printBeginTest(testName);

  ifstream ifs("huge_alphabet.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
                   (std::istreambuf_iterator<char>()));

  time_constructor_both(content, true);

  SBST sbst(content, true);
  SAVL savl(content, true);

  printEndTest(testName);

}

void hugeDNATest() {
  const string testName = "Huge Tests";
  printBeginTest(testName);

  ifstream ifs("huge_alphabet.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
                   (std::istreambuf_iterator<char>()));
}

void doTimedTests() {
  simpleDNATest();
  hugeAlphabetTest();
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