#include <iostream>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

int size(BinaryTreeNode *root) {
    if (!root) {
        return 0;
    }
    return 1 + size(root->left) + size(root->right);
}

int main() {
    return 0;
}
