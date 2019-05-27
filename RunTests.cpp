#include <iostream>
#include "sbst.h"

using namespace std;

int main() {
	SBST tree("CAATCACGGTCGGAC");

	tree.print();

	cout << tree.search("CGGA")->index << endl; // expect 10
	cout << tree.search("AC")->index << endl; // expect 5 or 12

	if (tree.search("CGT") == nullptr) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << tree.search("CAATCACGGTCGGAC")->index << endl; // expect 0
	cout << tree.search("C")->index << endl; // expect 0
	cout << tree.search("T")->index << endl; // expect 3

}
