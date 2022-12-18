#include <iostream>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

int height(BinaryTreeNode *root) {
    if (!root) {
        return 0;
    }
    int left_height = height(root->left), right_height = height(root->right);
    return left_height > right_height ? left_height + 1 : right_height + 1;
}

int main() {
    return 0;
}
