#include <iostream>

using namespace std;

struct ListNode {
    int data;        // data stored in this node
    ListNode* next;  // a link to the next node in the list

    ListNode(int data = 0, ListNode* next = nullptr): data(data), next(next) {}   // constructor
};

int min(ListNode *front) {
    if (!front) {
        throw "List is empty!";
    }
    int min_data = front->data;

    front = front->next;
    while (front) {
        if (front->data < min_data) {
            min_data = front->data;
        }
        front = front->next;
    }
    return min_data;
}

int main() {
    ListNode *list = new ListNode(12, new ListNode(1));
    cout << min(list) << endl;

    return 0;
}
