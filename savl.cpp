#include "savl.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::max;
using std::min;
using std::flush;
using std::vector;


SAVL::SAVL(const string& text, bool allWords): root(nullptr), text(text) {
	root = new Node(0, 0, UNSET);

	if (allWords) {
		vector<size_t> indices = getIndices(text);
		for (size_t index: indices) {
			insertIndex(index);
		}
	} else {
		for (size_t i = 1; i < text.length(); i++) {
			insertIndex(i);
		}
	}

}

size_t SAVL::findIndex(const std::string& prefix) {
	Node* found = search(prefix);
	if (found) return found->index;
	return -1;
}

std::string SAVL::getName() {
  return "SAVL";
}

vector<size_t> SAVL::getIndices(const string& text) {
        vector<size_t> indices;
        for (size_t i = 0; i < text.length() - 1; i++) {
                if (text[i] == ' ' || text[i] == '\n') indices.push_back(i + 1);
        }
        return indices;
}

void SAVL::insertIndex(size_t index) {
	Details det = find(text.substr(index), false);
	size_t llcp = det.llcp;
	size_t rlcp = det.rlcp;

	Direction dir;
	if (llcp == rlcp && llcp == 0) dir = UNSET;
	else if (llcp > rlcp) dir = LEFT;
	else dir = RIGHT;

	*(det.location) = new Node(index, max(llcp, rlcp), dir, nullptr, nullptr, det.parent);

	if (det.childDir == LEFT) {
		rebalance(det.parent, 1);
	} else {
		rebalance(det.parent, -1);
	}
}

SAVL::Node* SAVL::rotateLeftRight(SAVL::Node* node) {
	node->left = rotateLeft(node->left);
	Node* right = rotateRight(node);

	if (right && right->balance == 0) {
		node->balance = 0;
		right->left->balance = 0;
	} else if (right->balance == -1) {
		node->balance = 0;
		right->left->balance = 1;
	} else {
		node->balance = -1;
		right->left->balance = 0;
	}
	right->balance = 0;

	return right;
}

SAVL::Node* SAVL::rotateRightLeft(SAVL::Node* node) {
	node->right = rotateRight(node->right);
	Node* left = rotateLeft(node);

	if (left && left->balance == 0) {
		node->balance = 0;
		left->right->balance = 0;
	} else if (left->balance == 1) {
		node->balance = 0;
		left->right->balance = -1;
	} else {
		node->balance = 1;
		left->right->balance = 0;
	}
	left->balance = 0;

	return left;
}

SAVL::Node* SAVL::rotateRight(SAVL::Node* node) {
	Node* const left = node->left;
	Node* const leftRight = node->left->right;
	Node* const parent = node->parent;

	left->parent = parent;
	node->parent = left;
	if (leftRight) leftRight->parent = node;
	left->right = node;
	node->left = leftRight;

	if (node == root) {
		root = left;
	} else if (parent->left == node) {
		parent->left = left;
	} else {
		parent->right = left;
	}

	// left->balance--;
	// node->balance = -1 * left->balance;

	if (node->d == LEFT && left->d == LEFT) {
		size_t nodeM = node->m;
		size_t leftM = left->m;
		node->m = max(nodeM, leftM);
		left->m = min(nodeM, leftM);
		if (nodeM < leftM) {
			node->d = RIGHT;
		}
	} else if (node->d == RIGHT && left->d == LEFT) {
		std::swap(node->m, left->m);
		left->d = RIGHT;
	} else if (node->d == UNSET && left->d == LEFT) {
		std::swap(node->m, left->m);
		left->d = UNSET;
		node->d = RIGHT;
	}

	return left;
}

SAVL::Node* SAVL::rotateLeft(SAVL::Node* node) {
	Node* const right = node->right;
	Node* const rightLeft = node->right->left;
	Node* const parent = node->parent;

	right->parent = parent;
	node->parent = right;
	if (rightLeft) rightLeft->parent = node;
	right->left = node;
	node->right = rightLeft;

	if (node == root) {
		root = right;
	} else if (parent->right == node) {
		parent->right = right;
	} else {
		parent->left = right;
	}

	// right->balance++;
	// node->balance = -1 * right->balance;

	if (node->d == UNSET && right->d == RIGHT) {
		std::swap(node->m, right->m);
		node->d = LEFT;
		right->d = UNSET;
	} else if (node->d == LEFT && right->d == RIGHT) {
		std::swap(node->m, right->m);
		right->d = LEFT;
	} else if (node->d == RIGHT && right->d == RIGHT) {
		if (node->m >= right->m) {
			node->d = RIGHT;
		} else {
			node->d = LEFT;
		}
		size_t nodeM = node->m;
		size_t rightM = right->m;
		node->m  = max(nodeM, rightM);
		right->m = min(nodeM, rightM);
	}

	return right;

}

void SAVL::rebalance(SAVL::Node* node, int balance) {
	while (node != nullptr) {
		balance = node->balance = node->balance + balance;

		if (balance == 0) {
			return;
		} else if (balance == 2) {
			if (node->left && node->left->balance == 1) {
				rotateRight(node);
				node->parent->balance = 0;
				node->balance = 0;
			} else {
				rotateLeftRight(node);
			}
			return;
		} else if (balance == -2) {
			if (node->right && node->right->balance == -1) {
				rotateLeft(node);
				node->parent->balance = 0;
				node->balance = 0;
			} else {
				rotateRightLeft(node);
			}
			return;
		}
		Node* parent = node->parent;
		if (parent) {
			balance = (parent->left == node) ? 1 : -1;
		}
		node = parent;
	}
}

void SAVL::freeTree(SAVL::Node* curr) {
	if (!curr) return;
	freeTree(curr->left);
	freeTree(curr->right);
	delete curr;
}

SAVL::~SAVL() {
	freeTree(root);
}

void SAVL::print() {
	postorder(root, 0);
	cout << flush << endl;
}

void SAVL::postorder(SAVL::Node* p, int indent) {
    if(p != nullptr) {
        if(p->right) {
        	if (p->right->parent != p) cout << "ERROR" << endl;
            postorder(p->right, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->index << ", " << p->balance << "\n ";
        if(p->left) {
        	if (p->left->parent != p) cout << "ERROR" << endl;
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder(p->left, indent+4);
        }
    }
}

SAVL::Details SAVL::find(const string& prefix, bool searching) {
	size_t llcp = 0, rlcp = 0;
	Node** i = &root;
	Direction childDir;
	Node* parent;
	while (*i) {
		parent = *i;
		if ((*i)->m > max(llcp, rlcp)) {
			if ((*i)->d == LEFT) {
				i = &((*i)->left);
				childDir = LEFT;
			} else {
				i = &((*i)->right);
				childDir = RIGHT;
			}

		} else if ((*i)->m < max(llcp, rlcp)) {
			if (llcp > rlcp) {
				if ((*i)->d == LEFT) {
					rlcp = (*i)->m;
				}
				i = &((*i)->right);
				childDir = RIGHT;
			} else /* rlcp > llcp */ {
				if ((*i)->d == RIGHT) {
					llcp = (*i)->m;
				}
				i = &((*i)->left);
				childDir = LEFT;
			}
		} else if ((*i)->m == llcp && llcp > rlcp && (*i)->d == RIGHT) {
			i = &((*i)->right);
			childDir = RIGHT;
		} else if ((*i)->m == rlcp && rlcp > llcp && (*i)->d == LEFT) {
			i = &((*i)->left);
			childDir = LEFT;
		} else {
			size_t t = longestCommonPrefix(prefix, (*i)->index, (*i)->m);
			if (t == prefix.length()) {
				if (searching) break;
				i = &((*i)->left);
				llcp = t;
				childDir = LEFT;
			} else if (t + (*i)->index == text.length() || 
				       prefix[t] > text[t + (*i)->index]) {
				i = &((*i)->right);
				rlcp = t;
				childDir = RIGHT;
			} else {
				i = &((*i)->left);
				llcp = t;
				childDir = LEFT;
			}
		}
	}
	return {i, parent, llcp, rlcp, childDir};
}

// Returns longest common prefix of text[index + m_i:] and prefix[m_i:]
size_t SAVL::longestCommonPrefix(const string& prefix, size_t index, size_t m_i) {
	size_t i;
	for (i = m_i; i < prefix.length() && i < text.length(); i++) {
		if (prefix[i] != text[index + i]) break;
	}
	return i;
}

SAVL::Node* SAVL::search(const string& prefix) {
	Details det = find(prefix, true);
	return *(det.location);
}

// sideFromFound: LEFT if curr is in left subtree of search(text)
void SAVL::findAllOccurrences(size_t textLen, 
	                          std::unordered_set<size_t>& result,
	                          SAVL::Node* curr,
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

std::unordered_set<size_t> SAVL::findAllOccurrences(const string& text) {
	std::unordered_set<size_t> result;
	Node* first = search(text);
	if (first) {
		result.insert(first->index);
		findAllOccurrences(text.size(), result, first->left, LEFT, false);
		findAllOccurrences(text.size(), result, first->right, RIGHT, false);
	}
	return result;
}
