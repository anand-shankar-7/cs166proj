#include <iostream>
#include "sbst.h"
#include "savl.h"

using namespace std;

const size_t kNotFound = 10000;

void printResult(SAVL::Node* node, size_t expected) {
	if (node) {
		if (node->index == expected) cout << "PASS" << endl;
		else cout << "FAIL. Expected " << expected << ", got " << node->index << endl;
	} else {
		if (expected == kNotFound) cout << "PASS" << endl;
		else cout << "FAIL" << endl;
	}
}

void doSearch(SAVL& tree, string prefix, size_t expected) {
	printResult(tree.search(prefix), expected);
}

void doFindAll(SAVL& tree, string prefix, size_t expected) {
  auto res = tree.findAllOccurrences(prefix);
  if (res.size() != expected) {
    cout << "FUCK" << endl;
    cout << "got: " << res.size() << endl;
  } else {
    cout << "WOOT" << endl;
  }
}


int main() {

  {
    SAVL tree("CAATCACGGTCGGAC");

    tree.print();

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
