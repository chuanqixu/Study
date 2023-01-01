#include <iostream>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

bool contains(BinaryTreeNode *root, int value) {
    if (!root) {
        return false;
    }
    if (root->data == value) {
        return true;
    }

    return contains(root->left, value) || contains(root->right, value);
}

int main() {
    return 0;
}
