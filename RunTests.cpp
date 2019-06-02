#include <iostream>
#include "sbst.h"
#include "savl.h"
#include "sa.h"
#include "Timer.h"
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;

// Function prototype
void doCorrectnessTests();

// Constants
const size_t kNotFound = -1;

// *************************************************************************************
// *************************** BEGIN section for timed tests ***************************
// *************************************************************************************

// BEGIN utility/testing functions for timed tests

// Taken from StackOverflow
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
  cout << "Running find all test on " << Tree::getName() << " using list \"" << vecName << "\"" << endl;

  std::vector<std::uint64_t> times;
  times.reserve(words.size());

  for (const string& word: words) {
    Timer timmy;  
    timmy.start();
    (void) tree.findAllOccurrences(word);
    timmy.stop();
    times.push_back(timmy.elapsed());
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;
}

void time_findAll_all(SBST& sbst, SAVL& savl, SA& sa, const vector<string>& words, const string& vecName) {
  time_findAll(sbst, words, vecName);
  time_findAll(savl, words, vecName);
  time_findAll(sa, words, vecName);
}

template <typename Tree>
void time_constructor(const string& text, bool spacesSplit) {
  cout << "Timing constructor on " << Tree::getName() << endl;

  Timer timmy;  
  timmy.start();
  Tree* tree = new Tree(text, spacesSplit); // pointer forces construction and controls destruction
  timmy.stop();

  delete tree;

  cout << "    --> Time: " << timmy.elapsed() << endl << endl;

}

void time_constructor_all(const string& text, bool spacesSplit = false) {
  time_constructor<SBST>(text, spacesSplit);
  time_constructor<SAVL>(text, spacesSplit);
  time_constructor<SA>(text, false);
}

template <typename Tree>
void time_findIndex(Tree& tree, const vector<string>& words, const string& vecName) {
  cout << "Running find index test on " << Tree::getName() << " using list \"" << vecName << "\"" << endl;
  std::vector<std::uint64_t> times;
  times.reserve(words.size());

  for (const string& word: words) {
    Timer timmy;  
    timmy.start();
    (void) tree.findIndex(word);
    timmy.stop();
    times.push_back(timmy.elapsed());
  }

  cout << "    --> Time: " << getAverage(times) << endl << endl;
}

// deliberately doesn't do findIndex on suffix array because
// SA findIndex runtime is the same as that of findAllOccurrences
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

vector<string> getFileContentsOf(string filename) {
  vector<string> result;
  ifstream input(filename);
  for (string line; getline(input, line); ) {
    result.push_back(line);
  }
  return result;
}

// Used for huge DNA test.
vector<string> getGoodSequencesFrom(const string& content) {
  const size_t numWords = 200;
  vector<string> result;
  result.reserve(numWords);

  for (size_t i = 0; i < numWords; i++) {
    int randomStart = randomInteger(0, content.length() - 3);
    int randomLength = randomInteger(10, 100);
    // substr will truncate if length is too large. 
    string random = content.substr(randomStart, randomLength);
    result.push_back(random);
  }
  return result;
}

// Used for huge DNA test.
string generateRandomDNAString() {
  static string chars = "ACTG";
  size_t len = randomInteger(10, 100);
  ostringstream out;
  for (size_t i = 0; i < len; i++) {
    out << chars[rand() % chars.length()];
  }
  return out.str();
}

// Used for huge DNA test. All returned
// strings are NOT present in the tree 
// that's passed in.
vector<string> getRandomDNAStrings(SAVL& savl) {
  const size_t numWords = 200;
  const size_t kNotFound = -1;
  vector<string> result;
  result.reserve(numWords);

  while (result.size() < numWords) {
    string random = generateRandomDNAString();
    if (savl.findIndex(random) == kNotFound) {
      result.push_back(random);
    }
  }

  return result;
}

// *******************************************************
// ******* BEGIN test cases, each in 1 function **********
// *******************************************************

void simpleDNATest() {
  const string testName = "Simple DNA Test";
  printBeginTest(testName);

  time_constructor_all("CAATCACGGTCGGAC");

  SBST sbst("CAATCACGGTCGGAC");
  SAVL savl("CAATCACGGTCGGAC");
  SA   sa  ("CAATCACGGTCGGAC");

  time_findAll_all(sbst, savl, sa, {"A"},  "one element A");
  time_findAll_all(sbst, savl, sa, {"GG"}, "one element G");

  time_findIndex_both(sbst, savl, {"CA"}, "one element CA");

  printEndTest(testName);
}

void hugeDictionaryTest() {
  const string testName = "Huge Dictionary Tests";
  printBeginTest(testName);

  ifstream ifs("dictionary.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  time_constructor_all(content, true);

  SBST sbst(content, true);
  SAVL savl(content, true);
  SA   sa  (content, false);

  vector<string> fakeWords = getFileContentsOf("fake_words.txt");
  time_findAll_all(sbst, savl, sa, fakeWords, "Fake Words");
  vector<string> popularWords = getFileContentsOf("popular_words.txt");
  time_findAll_all(sbst, savl, sa, popularWords, "1000 Most Popular Words");

  printEndTest(testName);
}

void prejudiceTest() {
  const string testName = "Prejudice Tests";
  printBeginTest(testName);

  ifstream ifs("prideprejudice.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  time_constructor_all(content, true);

  SBST sbst(content, true);
  SAVL savl(content, true);
  SA   sa  (content);

  vector<string> fakeWords = getFileContentsOf("fake_words.txt");
  time_findAll_all(sbst, savl, sa, fakeWords, "Fake Words");
  vector<string> popularWords = getFileContentsOf("popular_words.txt");
  time_findAll_all(sbst, savl, sa, popularWords, "1000 Most Popular Words");

  printEndTest(testName);
}

void hugeDNATest() {
  const string testName = "Huge DNA Tests";
  printBeginTest(testName);

  ifstream ifs("dna.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  time_constructor_all(content, false);

  SBST sbst(content);
  SAVL savl(content);
  SA   sa  (content);

  vector<string> sequencesPresent = getGoodSequencesFrom(content);
  time_findAll_all(sbst, savl, sa, sequencesPresent, "Sequences that are present");
  vector<string> sequencesNotPresent = getRandomDNAStrings(savl);
  time_findAll_all(sbst, savl, sa, sequencesNotPresent, "Sequences that are NOT present");

  printEndTest(testName);
}

void smallDictionaryTest() {
  const string testName = "Small Dictionary Tests";
  printBeginTest(testName);

  ifstream ifs("popular_words.txt");
  string content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  time_constructor_all(content, false);

  printEndTest(testName);
}


void doTimedTests() {
  smallDictionaryTest();
  simpleDNATest();
  prejudiceTest();
  hugeDNATest();
  hugeDictionaryTest();
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
