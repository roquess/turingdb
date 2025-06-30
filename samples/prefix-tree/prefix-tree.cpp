#include <cctype>
#include <iostream>
#include <array>
#include <vector>

#include "utils.h"


// TODO:
// remove std namespace

using namespace std;

// Size of our alphabet: assumes some preprocessing,
// so only a-z and 1-9
constexpr size_t SIGMA = 26 + 10;

class StringApproximatorIndex {
public:
    struct PrefixTreeNode {
        array<PrefixTreeNode*, SIGMA> _children{};
        // Termination char denotes terminal node
        char _val{'\0'};
        bool _isComplete{false};

        PrefixTreeNode(char val)
            : _children{},
            _val{val},
            _isComplete(false)
        {
        }

        ~PrefixTreeNode() {
            for (auto child : _children) {
                delete child;
            }
        }

    };


    StringApproximatorIndex()
    {
        _root = new PrefixTreeNode{'\1'};
    }

    void insert(string_view str) { return _insert(_root, str); }

    void print() const { _printTree(_root); }

private:
    PrefixTreeNode* _root{nullptr};

    static size_t charToIndex(char c) {
        // Children array layout:
        // INDEX CHARACTER VALUE
        // 0     a
        // ...  ...
        // 25    z
        // 26    0
        // ...  ...
        // 36    9
        if (isalpha(c)) return c - 'a';
        else return 26 + c - '0';
    }

    void _insert(PrefixTreeNode* n, string_view str) {
        PrefixTreeNode* node = n;
        for (const char c : str) {
            size_t idx = charToIndex(c);
            if (node->_children[idx] == nullptr) {
                auto* newChild = new PrefixTreeNode(c);
                node->_children[idx] = newChild;
            }
            node = node->_children[idx];
        }
        node->_isComplete = true;
    }

    void _insert_rec(PrefixTreeNode* parent, string_view str) {
        if (str.empty()) {
            parent->_isComplete = true;
            return;
        }
        // Get first char
        char c = str.at(0);
        str.remove_prefix(1);
        size_t idx = charToIndex(c);

        // If next char already exists in the tree
        if (parent->_children[idx] != nullptr) {
            return _insert_rec(parent->_children[idx], str);
        }
        else {
            auto newChild = new PrefixTreeNode{c};
            parent->_children[idx] = newChild;
            return _insert_rec(newChild, str);
        }
    }

    enum FindResult {
        FOUND,
        FOUND_PREFIX,
        NOT_FOUND
    };

    FindResult _find(PrefixTreeNode* parent, string_view str) {
        if (str.empty()) {
            parent->_isComplete = true;
            return FOUND;
        }
        
    }

   void _printTree(PrefixTreeNode* node) const {
        _printTree(node, "", true);
    }

    void _printTree(PrefixTreeNode* node,
                    const std::string& prefix,
                    bool isLastChild) const {
        if (!node) return;

        if (node->_val != '\1') {
            std::cout << prefix
                      << (isLastChild ? "└── " : "├── ")
                      << node->_val
                      << (node->_isComplete ? "*" : "")
                      << '\n';
        }

        // Gather existing children so we know which one is the last
        vector<PrefixTreeNode*> kids;
        kids.reserve(SIGMA);
        for (std::size_t i = 0; i < SIGMA; ++i)
            if (node->_children[i]) kids.push_back(node->_children[i]);

        // Prefix extension: keep vertical bar if this isn’t last
        std::string nextPrefix = prefix;
        if (node->_val != '\1')          // don’t add for sentinel root
            nextPrefix += (isLastChild ? "    " : "│   ");

        // Recurse over children
        for (std::size_t k = 0; k < kids.size(); ++k) {
            _printTree(kids[k], nextPrefix, k + 1 == kids.size());
        }
    }
};


const string help = "";

int main() {
    cout << help << endl;

    StringApproximatorIndex tree{};

    string in;
    while (true) {
        cout << ">";
        getline(cin, in);
        auto tokens = split(in, " ");
        if (tokens[0] == "i") {
            if (tokens.size() != 2) cout << "i <param>" << endl;
            else tree.insert(tokens[1]);
        }
        else if (tokens[0] == "p") tree.print();
        else cout << "unkown cmd" << endl;
    }

}

