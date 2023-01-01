// CodeStepByStep checker is confusing for this problem

#include <iostream>

using namespace std;

struct ListNode {
    int data;        // data stored in this node
    ListNode* next;  // a link to the next node in the list

    ListNode(int data = 0, ListNode* next = nullptr): data(data), next(next) {}   // constructor
};

int main() {
    ListNode *before = new ListNode(1, new ListNode(2));

    ListNode *curr = before;
    curr = before;
    cout << "Before: " << endl;
    while (curr) {
        cout << curr->data << endl;
        curr = curr->next;
    }
    cout << endl;
    
    curr = before;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = new ListNode(3);

    ListNode *after = before;

    curr = after;
    cout << "After: " << endl;
    while (curr) {
        cout << curr->data << endl;
        curr = curr->next;
    }

    return 0;
}
