#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {
    logSize = 0;
    sentinel = new HeapBit(nullptr, nullptr);
}

BinomialHeapPQueue::~BinomialHeapPQueue() {
    while (sentinel) {
        auto trash = sentinel;
        sentinel = sentinel->next;
        delete trash->head;
        delete trash;
    }
}

const string& BinomialHeapPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
//    return kEmptyString;

    if (logSize == 0) {
        throw "No item in the BinomialHeapPQueue!";
    }

    auto min_heapbit = sentinel->next;
    auto curr = sentinel->next->next;
    while (curr) {
        if (curr->head->elem < min_heapbit->head->elem) {
            min_heapbit = curr;
        }
        curr = curr->next;
    }
    return min_heapbit->head->elem;
}

void BinomialHeapPQueue::add_binomial_heap(HeapBit *curr, Node *carry) {
    if (!curr || !carry || (curr->head && curr->head->bit_idx() > carry->bit_idx())) {
        throw "Cannot add the binomial heap after curr!";
    }
    while (carry) {
        if (!curr->next || curr->next->head->bit_idx() > carry->bit_idx()) { // only one item at this bit: directly add it
            curr->next = new HeapBit(carry, curr->next);
            break;
        } else { // only two items: no item for this location, but one carry for the next bit
            carry = add_nodes(curr->next->head, carry);
            auto trash = curr->next;
            curr->next = curr->next->next;
            delete trash;
        }
    }
}

string BinomialHeapPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
//	return peek();

    string min_str = peek();
    --logSize;

    // end_ptr points to the previous element of the min string
    auto end_ptr = sentinel;
    while (end_ptr->next) {
        if (end_ptr->next->head->elem == min_str) {
            break;
        }
        end_ptr = end_ptr->next;
    }

    auto trash = end_ptr->next;
    // if the min string is at bit 0, directly remove it
    if (end_ptr->next->head->bit_idx() == 0) {
        end_ptr->next = end_ptr->next->next;
        delete trash;
        return min_str;
    }

    // for bits from 0 to max_bit, compute the carry
    // 1. from 0 to the bit before the non-zero bit of the original
    //  heap: because there is only the binomial heap to be added,
    //  add all them to the original heap
    // 2. at the non-zero bit of the original heap: compute the
    //  carry and remove the original binomial heap
    // 3. starting from the non-zero bit of the original heap:
    //  there is carry, because there is also a binomial heap to
    //  be added, compute the carry and keep the original heap
    //  the same
    size_t max_bit = end_ptr->next->head->bit_idx() - 1;
    Vector<Node *> *heads = &(end_ptr->next->head->children);
    end_ptr->next = end_ptr->next->next;
    delete trash;

    auto curr = sentinel;
    Node *carry = nullptr;

    for (size_t idx = 0; idx <= max_bit; ++idx) {
        if (carry) { // from the non-zero bit of the original heap to to end of the binomial heap to add
            carry = add_nodes(heads->get(idx), carry);
        } else {
            if (!curr->next || curr->next->head->bit_idx() > idx) { // from 0 to the bit before the non-zero bit of the original heap
                curr->next = new HeapBit(heads->get(idx), curr->next);
                curr = curr->next;
            } else { // at the non-zero bit of the original heap
                carry = add_nodes(curr->next->head, heads->get(idx));
                trash = curr->next;
                curr->next = curr->next->next;
                delete trash;
            }
        }
    }

    // if there is carry
    if (carry) {
        // get the location after which to add the carry
        curr = sentinel;
        while (curr->next && curr->next->head->bit_idx() < carry->bit_idx()) {
            curr = curr->next;
        }

        // add the carry
        add_binomial_heap(curr, carry);
    }

    return min_str;
}

void BinomialHeapPQueue::enqueue(const string &elem /* elem */) {
    // placeholder so method compiles..
	// replace with your own implementation

    add_binomial_heap(sentinel, new Node(elem));
    ++logSize;
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one /* one */, BinomialHeapPQueue *two /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
//	return new BinomialHeapPQueue();

    auto merged_BinomialHeapPQueue_ptr = new BinomialHeapPQueue;
    merged_BinomialHeapPQueue_ptr->logSize = one->logSize + two->logSize;

    // add two to one
    // similar to the merge step in merge sort
    HeapBit *curr = merged_BinomialHeapPQueue_ptr->sentinel,
            *curr_one = one->sentinel->next,
            *curr_two = two->sentinel->next;
    Node *carry = nullptr;

    while (curr_one && curr_two) {
        if (carry) {
            if (curr_one->head->bit_idx() > carry->bit_idx() && curr_two->head->bit_idx() > carry->bit_idx()) { // only one item at this bit: directly add it
                curr->next = new HeapBit(carry);
                carry = nullptr;
                curr = curr->next;
            } else if (curr_one->head->bit_idx() == carry->bit_idx() && curr_one->head->bit_idx() == curr_two->head->bit_idx()) { // three items at this bit: keep the carry and compute the next carry equal to one + two
                auto next_carry = add_nodes(curr_one->head, curr_two->head);
                curr->next = new HeapBit(carry);
                carry = next_carry;

                curr = curr->next;
                curr_one->head = nullptr;
                curr_two->head = nullptr;
                curr_one = curr_one->next;
                curr_two = curr_two->next;
            } else if (curr_one->head->bit_idx() == carry->bit_idx()) { // only two items: no item for this location, but one carry for the next bit
                carry = add_nodes(curr_one->head, carry);
                curr_one->head = nullptr;
                curr_one = curr_one->next;
            } else if (curr_two->head->bit_idx() == carry->bit_idx()){ // only two items: no item for this location, but one carry for the next bit
                carry = add_nodes(curr_two->head, carry);
                curr_two->head = nullptr;
                curr_two = curr_two->next;
            }
        } else {
            if (curr_one->head->bit_idx() == curr_two->head->bit_idx()) { // only two items: no item for this location, but one carry for the next bit
                carry = add_nodes(curr_one->head, curr_two->head);
                curr_one->head = nullptr;
                curr_two->head = nullptr;
                curr_one = curr_one->next;
                curr_two = curr_two->next;
            } else if (curr_one->head->bit_idx() < curr_two->head->bit_idx()) { // only one item at this bit: directly add it
                curr->next = new HeapBit(curr_one->head);
                curr_one->head = nullptr;
                curr_one = curr_one->next;
            } else { // only one item at this bit: directly add it
                curr->next = new HeapBit(curr_two->head);
                curr_two->head = nullptr;
                curr_two = curr_two->next;
            }
        }
    }

    // consider the remaining parts in two operands
    while(curr_one) {
        if (carry) {
            if (curr_one->head->bit_idx() > carry->bit_idx()) {
                curr->next = new HeapBit(carry);
                carry = nullptr;
                curr = curr->next;
            } else {
                carry = add_nodes(curr_one->head, carry);
                curr_one->head = nullptr;
                curr_one = curr_one->next;
            }
        } else {
            curr->next = new HeapBit(curr_one->head);
            curr_one->head = nullptr;
            curr_one = curr_one->next;
        }
    }

    while(curr_two) {
        if (carry) {
            if (curr_two->head->bit_idx() > carry->bit_idx()) {
                curr->next = new HeapBit(carry);
                carry = nullptr;
                curr = curr->next;
            } else {
                carry = add_nodes(curr_two->head, carry);
                curr_two->head = nullptr;
                curr_two = curr_two->next;
            }
        } else {
            curr->next = new HeapBit(curr_two->head);
            curr_two->head = nullptr;
            curr_two = curr_two->next;
        }
    }

    // consider the last carry
    if (carry) {
        curr->next = new HeapBit(carry);
    }

    delete one;
    delete two;

    return merged_BinomialHeapPQueue_ptr;
}
