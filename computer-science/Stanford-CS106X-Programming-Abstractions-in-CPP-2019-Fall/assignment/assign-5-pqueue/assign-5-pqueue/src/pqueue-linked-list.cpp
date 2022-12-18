#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    logSize = 0;
    sentinel = new Node("", nullptr, nullptr);
}

LinkedListPQueue::~LinkedListPQueue() {
    Node *trash = sentinel;
    while (sentinel) {
        sentinel = sentinel->next;
        delete trash;
        trash = sentinel;
    }
}

const string& LinkedListPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
//    return kEmptyString;
    if (logSize == 0) {
        throw "No item in the LinkedListPQueue";
    }
    return sentinel->next->str;
}

string LinkedListPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
//	return peek();

    string min_str = peek();

    auto trash = sentinel->next;
    sentinel->next = trash->next;
    if (trash->next) {
        sentinel->next->prev = sentinel;
    }
    delete trash;
    --logSize;

    return min_str;
}

void LinkedListPQueue::enqueue(const string &elem/* elem */) {
	// placeholder so method compiles..
	// replace with your own implementation

    auto curr = sentinel;
    while (curr->next && curr->next->str < elem) {
        curr = curr->next;
    }

    if (!curr->next) {
        curr->next = new Node(elem, curr, nullptr);
    } else {
        curr->next->prev = new Node(elem, curr, curr->next);
        curr->next = curr->next->prev;
    }
    ++logSize;
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue *one /* one */, LinkedListPQueue *two /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
//	return new LinkedListPQueue();

    // implement the merge step in merge sort
    auto merged_LinkedListPQueue_ptr = new LinkedListPQueue();

    auto curr_one = one->sentinel->next, curr_two = two->sentinel->next;

    while (curr_one && curr_two) {
        if (curr_one->str < curr_two->str) {
            merged_LinkedListPQueue_ptr->enqueue(curr_one->str);
            curr_one = curr_one->next;
        } else {
            merged_LinkedListPQueue_ptr->enqueue(curr_two->str);
            curr_two = curr_two->next;
        }
    }

    if (curr_one) {
        merged_LinkedListPQueue_ptr->enqueue(curr_one->str);
        curr_one = curr_one->next;
    } else {
        merged_LinkedListPQueue_ptr->enqueue(curr_two->str);
        curr_two = curr_two->next;
    }

    delete one;
    delete two;

    return merged_LinkedListPQueue_ptr;
}

