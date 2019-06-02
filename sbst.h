/* Specifies the interface for a suffix binary search tree.
 * Uses the ideas presented in the paper "The suffix binary
 * search tree and suffix AVL tree" by Robert Irving and 
 * Lorna Love.
 */

#ifndef _sbst_h
#define _sbst_h

#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

class SBST {

enum Direction {LEFT = 0, RIGHT, UNSET};

public:
	SBST(const std::string& text, bool onlyWords = false);
	~SBST();
	std::unordered_set<size_t> findAllOccurrences(const std::string& text);
	size_t findIndex(const std::string& prefix);

  // Used by the testing harness.
  static std::string getName();

private:
  // Type representing a node in the suffix tree.
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

  // Utility struct used to return multiple values in find.
	struct Details {
		Node** location;
		size_t llcp;
		size_t rlcp;
	};

  // Private helper functions.
	Node* search(const std::string& prefix);
	void insertIndex(std::size_t index);
	size_t longestCommonPrefix(const std::string& prefix, std::size_t index, std::size_t m_i);
	Details find(const std::string& prefix, bool searching);
	void freeTree(Node* curr);
	void findAllOccurrences(size_t textLen, std::unordered_set<size_t>& result,
	                        SBST::Node* curr, Direction sideFromFound, bool matchAll);

  // Private member variables.
	Node* root;
	std::string text;

  /* Fun with C++: disable copy constructor and 
   * operator= to avoid memory issues and undefined
   * behavior.
   */
  SBST(SBST const &) = delete;
  void operator=(SBST const &) = delete;
};

#endif

