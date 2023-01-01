#include <iostream>
#include <initializer_list>

using namespace std;

struct ListNode {
    int data;        // data stored in this node
    ListNode* next;  // a link to the next node in the list

    ListNode(int data = 0, ListNode* next = nullptr): data(data), next(next) {}   // constructor
    explicit ListNode(initializer_list<int> list) {
        if (list.size() == 0) {
            return;
        }
        data = *list.begin();
        next = nullptr;
        ListNode *curr = this;
        for (auto iter = list.begin() + 1; iter != list.end(); ++iter) {
            curr->next = new ListNode(*iter);
            curr = curr->next;
        }
    }
};

int countDuplicates(ListNode *front) {
    if (!front) {
        return 0;
    }

    int count = 0, curr_data = front->data;
    front = front->next;
    while (front) {
        if (front->data == curr_data) {
            ++count;
        } else {
            curr_data = front->data;
        }
        front = front->next;
    }
    return count;
}

int main() {
    ListNode *list = new ListNode {1, 1, 1, 3, 3, 6, 9, 15, 15, 23, 23, 23, 40, 40};
    cout << countDuplicates(list) << endl;

    return 0;
}
