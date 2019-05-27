#include <iostream>
#include "sbst.h"

using namespace std;

const size_t kNotFound = 10000;

void printResult(SBST::Node* node, size_t expected) {
	if (node) {
		if (node->index == expected) cout << "PASS" << endl;
		else cout << "FAIL. Expected " << expected << ", got " << node->index << endl;
	} else {
		if (expected == kNotFound) cout << "PASS" << endl;
		else cout << "FAIL" << endl;
	}
}

void doSearch( SBST& tree, string prefix, size_t expected) {
	printResult(tree.search(prefix), expected);
}

int main() {
	{
		SBST tree("CAATCACGGTCGGAC");

		tree.print();

		doSearch(tree, "CGGA", 10);
		doSearch(tree, "AC", 5); // could also be 12

		doSearch(tree, "CGT", kNotFound);

		doSearch(tree, "CAATCACGGTCGGAC", 0);
		doSearch(tree, "C", 0);
		doSearch(tree, "T", 3);
	}

	{
		SBST tree("abcdefghijklmnop");
		doSearch(tree, "abc", 0);
		doSearch(tree, "def", 3);
		doSearch(tree, "mnop", 12);
		doSearch(tree, "klmno", 10);
		doSearch(tree, "abcdefghijklmnop", 0);
		doSearch(tree, "z", kNotFound);
	}


}
