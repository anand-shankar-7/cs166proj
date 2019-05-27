#include <iostream>
#include "sbst.h"

using namespace std;



void printResult(SBST::Node* node, size_t expected) {
	if (node) {
		if (node->index == expected) cout << "PASS" << endl;
		else cout << "FAIL. Expected " << expected << ", got " << node->index << endl;
	} else {
		if (expected == 10000) cout << "PASS" << endl;
		else cout << "FAIL" << endl;
	}
}

void doSearch( SBST& tree, string prefix, size_t expected) {
	printResult(tree.search(prefix), expected);
}

int main() {
	SBST tree("CAATCACGGTCGGAC");

	tree.print();

	doSearch(tree, "CGGA", 10);
	doSearch(tree, "AC", 5); // could also be 12

	doSearch(tree, "CGT", 10000);

	doSearch(tree, "CAATCACGGTCGGAC", 0);
	doSearch(tree, "C", 0);
	doSearch(tree, "T", 3);

}
