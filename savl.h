#ifndef _SAVL_h
#define _SAVL_h

#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

class SAVL {

  enum Direction {LEFT = 0, RIGHT, UNSET};

  public:

  SAVL(const std::string& text, bool addWords = false);
  ~SAVL();
  std::unordered_set<size_t> findAllOccurrences(const std::string& text);
  std::string getName();
  size_t findIndex(const std::string& prefix);


  void print();

  private:

  struct Node {
    size_t index;
    size_t m;
    Direction d;
    Node* left;
    Node* right;
    Node* parent;
    int balance;

    Node(size_t index, size_t m = 0, Direction d = UNSET, 
        Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr):
      index(index), m(m), d(d), left(left), right(right),
      parent(parent), balance(0) {}
  };


  struct Details {
    Node** location;
    Node* parent;
    size_t llcp;
    size_t rlcp;
    Direction childDir;
  };

  Node* search(const std::string& prefix);
  void postorder(Node* root, int indent);
  size_t longestCommonPrefix(const std::string& prefix, std::size_t index, std::size_t m_i);
  Details find(const std::string& prefix, bool searching);
  void freeTree(Node* curr);
  void rebalance(Node* node, int balance);
  Node* rotateLeft(Node* node);
  Node* rotateRight(Node* node);
  Node* rotateLeftRight(Node* node);
  Node* rotateRightLeft(Node* node);
  void findAllOccurrences(size_t textLen, std::unordered_set<size_t>& result,
      SAVL::Node* curr, Direction sideFromFound, bool matchAll);
  void insertIndex(std::size_t index);


  Node* root;
  std::string text;


  // Fun with C++: delete copy constructor and assignment operator
  SAVL(SAVL const &) = delete;
  void operator=(SAVL const &) = delete;
};

#endif
