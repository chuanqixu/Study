#include <iostream>

using namespace std;

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

bool isBSTHelper(BinaryTreeNode *root, int &min, int &max) {
    if (!root) {
        min = INT_MAX;
        max = INT_MIN;
        return true;
    }
    int left_min, left_max, right_min, right_max;
    bool is_bst_left = isBSTHelper(root->left, left_min, left_max), 
        is_bst_right = isBSTHelper(root->right, right_min, right_max);

    if (is_bst_left && is_bst_right && root->data > left_max && root->data < right_min) {
        min = left_min == INT_MAX ? root->data : left_min;
        max = right_max == INT_MIN ? root->data : right_max;
        return true;
    } else {
        return false;
    }
}

bool isBST(BinaryTreeNode *root) {
    if (!root) {
        return true;
    }
    int min, max;
    return isBSTHelper(root, min, max);
}

int main() {
    return 0;
}
