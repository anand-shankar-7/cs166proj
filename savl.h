#ifndef _SAVL_h
#define _SAVL_h

#include <cstddef>
#include <string>
#include <unordered_set>

class SAVL {

enum Direction {LEFT = 0, RIGHT, UNSET};

public:
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


        SAVL(const std::string& text);
        ~SAVL();
        Node* search(const std::string& prefix);
        std::unordered_set<size_t> findAllOccurrences(const std::string& text);

        void print();

private:


        struct Details {
                Node** location;
                Node* parent;
                size_t llcp;
                size_t rlcp;
                Direction childDir;
        };

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


        Node* root;
        std::string text;
};

#endif