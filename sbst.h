#ifndef _sbst_h
#define _sbst_h

#include <cstddef>
#include <string>
#include <unordered_set>

class SBST {

enum Direction {LEFT = 0, RIGHT, UNSET};
public:
	struct Node {
		size_t index;
		size_t m;
		Direction d;
		Node* left;
		Node* right;

		Node(size_t index, size_t m = 0, Direction d = UNSET, 
			Node* left = nullptr, Node* right = nullptr):
			index(index), m(m), d(d), left(left), right(right) {}
	};


	SBST(const std::string& text);
	~SBST();
	Node* search(const std::string& prefix);
	std::unordered_set<size_t> findAllOccurrences(const std::string& text);

	void print();

private:


	struct Details {
		Node** location;
		size_t llcp;
		size_t rlcp;
	};

	void postorder(Node* root, int indent);
	size_t longestCommonPrefix(const std::string& prefix, std::size_t index, std::size_t m_i);
	Details find(const std::string& prefix, bool searching);
	void freeTree(Node* curr);
	void findAllOccurrences(size_t textLen, std::unordered_set<size_t>& result,
	                        SBST::Node* curr, Direction sideFromFound, bool matchAll);


	Node* root;
	std::string text;
};

#endif

