#include <iostream>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void print(BinaryTreeNode *root) {
    if (!root) {
        return;
    }
    print(root->left);
    cout << root->data << endl;
    print(root->right);
}

int main() {
    return 0;
}
