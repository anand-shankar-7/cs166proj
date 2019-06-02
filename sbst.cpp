#include "sbst.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;


SBST::SBST(const string& text, bool onlyWords): root(nullptr), text(text) {
	root = new Node(0, 0, UNSET);

	if (onlyWords) {
		for (size_t i = 0; i < text.length() - 1; i++) {
			if (text[i] == ' ' || text[i] == '\n') insertIndex(i + 1);
		}
	} else {
		for (size_t i = 1; i < text.length(); i++) {
			insertIndex(i);
		}
	}
  
}

size_t SBST::findIndex(const std::string& prefix) {
	Node* found = search(prefix);
	if (found) return found->index;
	return -1;
}

string SBST::getName() { 
  return "SBST";
}

void SBST::insertIndex(size_t index) {
	Details det = find(text.substr(index), false);
	size_t llcp = det.llcp;
	size_t rlcp = det.rlcp;

	Direction dir;
	if (llcp == rlcp && llcp == 0) dir = UNSET;
	else if (llcp > rlcp) dir = LEFT;
	else dir = RIGHT;

	*(det.location) = new Node(index, max(llcp, rlcp), dir);
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
				llcp = t;
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

// sideFromFound: LEFT if curr is in left subtree of search(text)
void SBST::findAllOccurrences(size_t textLen, 
	                          std::unordered_set<size_t>& result,
	                          SBST::Node* curr,
	                          Direction sideFromFound,
	                          bool matchAll) {
	if (!curr) return;

	if (matchAll || (curr->m >= textLen && curr->d == sideFromFound)) {  // I am an occurrence.
		findAllOccurrences(textLen, result, curr->left, sideFromFound, matchAll || sideFromFound == RIGHT);
		result.insert(curr->index);
		findAllOccurrences(textLen, result, curr->right, sideFromFound, matchAll || sideFromFound == LEFT);
	} else if (sideFromFound == LEFT) {
		findAllOccurrences(textLen, result, curr->right, sideFromFound, false);
	} else {
		findAllOccurrences(textLen, result, curr->left, sideFromFound, false);
	}
}

std::unordered_set<size_t> SBST::findAllOccurrences(const string& text) {
	std::unordered_set<size_t> result;
	Node* first = search(text);
	if (first) {
		result.insert(first->index);
		findAllOccurrences(text.size(), result, first->left, LEFT, false);
		findAllOccurrences(text.size(), result, first->right, RIGHT, false);
	}
	return result;
}
