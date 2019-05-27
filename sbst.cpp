#include "sbst.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;


SBST::SBST(const string& text): root(nullptr), text(text) {
	// Node* fourteen = new Node(13, 2, LEFT);
	// Node* six = new Node(5, 1, LEFT, fourteen);
	// Node* fifteen = new Node(14, 1, LEFT);
	// Node* three = new Node(2, 1, RIGHT, six, fifteen);
	// Node* two = new Node(1, 0, UNSET, nullptr, three);
	// Node* thirteen = new Node(12, 1, LEFT);
	// Node* twelve = new Node(11, 2, LEFT, thirteen);
	// Node* nine = new Node(8, 1, RIGHT);
	// Node* eight = new Node(7, 0, UNSET, twelve, nine);
	// Node* eleven = new Node(10, 3, LEFT);
	// Node* seven = new Node(6, 1, RIGHT, eleven, eight);
	// Node* five = new Node(4, 2, RIGHT, nullptr, seven);
	// Node* ten = new Node(9, 2, RIGHT);
	// Node* four = new Node(3, 0, UNSET, five, ten);
	// root = new Node(0, 0, UNSET, two, four);
	root = new Node(0, 0, UNSET);

	for (size_t i = 1; i < text.length(); i++) {
		Details det = find(text.substr(i), false);
		size_t llcp = det.llcp;
		size_t rlcp = det.rlcp;

		Direction dir;
		if (llcp == rlcp && llcp == 0) dir = UNSET;
		else if (llcp > rlcp) dir = LEFT;
		else dir = RIGHT;

		cout << det.location << endl;
		*(det.location) = new Node(i, max(llcp, rlcp), dir);
	}

}

void SBST::freeTree(SBST::Node* curr) {
	if (!curr) return;
	freeTree(curr->left);
	freeTree(curr->right);
	delete curr;
}

SBST::~SBST() {
	freeTree(root);
}

void SBST::print() {
	postorder(root, 0);
	cout << flush << endl;
}

void SBST::postorder(SBST::Node* p, int indent) {
    if(p != nullptr) {
        if(p->right) {
            postorder(p->right, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->index << "\n ";
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder(p->left, indent+4);
        }
    }
}

SBST::Details SBST::find(const string& prefix, bool searching) {
	size_t llcp = 0, rlcp = 0;
	Node** i = &root;
	while (*i) {
		if ((*i)->m > max(llcp, rlcp)) {
			i = ((*i)->d == LEFT) ? &((*i)->left) : &((*i)->right);
		} else if ((*i)->m < max(llcp, rlcp)) {
			if (llcp > rlcp) {
				if ((*i)->d == LEFT) {
					rlcp = (*i)->m;
				}
				i = &((*i)->right);
			} else /* rlcp > llcp */ {
				if ((*i)->d == RIGHT) {
					llcp = (*i)->m;
				}
				i = &((*i)->left);
			}
		} else if ((*i)->m == llcp && llcp > rlcp && (*i)->d == RIGHT) {
			i = &((*i)->right);
		} else if ((*i)->m == rlcp && rlcp > llcp && (*i)->d == LEFT) {
			i = &((*i)->left);
		} else {
			size_t t = longestCommonPrefix(prefix, (*i)->index, (*i)->m);
			if (t == prefix.length()) {
				if (searching) break;
				i = &((*i)->left);
			} else if (t + (*i)->index == text.length() || 
				       prefix[t] > text[t + (*i)->index]) {
				i = &((*i)->right);
				rlcp = t;
			} else {
				i = &((*i)->left);
				llcp = t;
			}
		}
	}
	return {i, llcp, rlcp};
}

// Returns longest common prefix of text[index + m_i:] and prefix[m_i:]
size_t SBST::longestCommonPrefix(const string& prefix, size_t index, size_t m_i) {
	size_t i;
	for (i = m_i; i < prefix.length() && i < text.length(); i++) {
		if (prefix[i] != text[index + i]) break;
	}
	return i;
}

SBST::Node* SBST::search(const string& prefix) {
	Details det = find(prefix, true);
	return *(det.location);
}