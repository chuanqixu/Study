#include <iostream>
#include <string>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void printSideways(BinaryTreeNode *root, string indent = "") {
    if (!root) {
        return;
    }
    printSideways(root->right, indent + "    ");
    cout << indent << root->data << endl;
    printSideways(root->left, indent + "    ");
}

int main() {
    return 0;
}
