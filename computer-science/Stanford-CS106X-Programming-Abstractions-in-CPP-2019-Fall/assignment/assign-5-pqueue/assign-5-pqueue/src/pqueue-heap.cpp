#include "pqueue-heap.h"
#include <algorithm> // for swap()
#include <cmath> // for compute the capacify when merging
using namespace std;

HeapPQueue::HeapPQueue() {
    logSize = 0;
    collection_ptr = new string[4];
    capacity = 4;
}

HeapPQueue::~HeapPQueue() {
    delete[] collection_ptr;
}

const string& HeapPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
//    return kEmptyString;
    if (logSize == 0) {
        throw "No item in the HeapPQueue";
    }
    return collection_ptr[0];
}

void HeapPQueue::heapify(size_t idx) {
    size_t curr = idx, min_child_idx = 2 * curr + 1;

    do {
        // get the smallest chile index
        if (min_child_idx + 1 < logSize) { // has both children
            if (collection_ptr[min_child_idx] > collection_ptr[min_child_idx + 1]) {
                ++min_child_idx;
            }
        } else if (min_child_idx >= logSize) { // no child
            return;
        }

        // if element at curr is larger than any child, the process has finished
        if (collection_ptr[curr] <= collection_ptr[min_child_idx]) {
            return;
        }
        // otherwise, swap them
        swap(collection_ptr[curr], collection_ptr[min_child_idx]);

        // update to the next level
        curr = min_child_idx; // curr goes to the smaller child
        min_child_idx = 2 * min_child_idx + 1; // min_child_idx goes to its left child
    } while (min_child_idx < logSize);
}

string HeapPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
//	return peek();
    string min_str = peek();

    // remove the smallest element
    collection_ptr[0] = collection_ptr[logSize - 1];
    --logSize;

    heapify(0);

    return min_str;
}

void HeapPQueue::enqueue(const string& elem/* elem */) {
    // placeholder so method compiles..
	// replace with your own implementation

    // resize if there is no more space in the array
    if (capacity == logSize) {
        auto trash = collection_ptr;
        collection_ptr = new string[capacity * 2];
        capacity *= 2;
        for (size_t i = 0; i < logSize; ++i) {
            collection_ptr[i] = trash[i];
        }
        delete[] trash;
    }
    collection_ptr[logSize] = elem;
    ++logSize;

    // move up the newly added element to the right place
    size_t curr = logSize;
    while (curr > 1 && collection_ptr[curr / 2 - 1] > collection_ptr[curr - 1]) {
        swap(collection_ptr[curr / 2 - 1], collection_ptr[curr - 1]);
        curr /= 2;
    }
}

HeapPQueue *HeapPQueue::merge(HeapPQueue *one /* one */, HeapPQueue *two /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
//	return new HeapPQueue();

    auto merged_HeapPQueue_ptr = new HeapPQueue();
    delete merged_HeapPQueue_ptr->collection_ptr;

    merged_HeapPQueue_ptr->logSize = one->logSize + two->logSize;
    merged_HeapPQueue_ptr->capacity = pow(2, ceil(log2(merged_HeapPQueue_ptr->logSize)));
    merged_HeapPQueue_ptr->collection_ptr = new string [merged_HeapPQueue_ptr->capacity];

    // combine two arrays
    string *curr_ptr = merged_HeapPQueue_ptr->collection_ptr;
    for (size_t i = 0; i < one->logSize; ++i) {
        *curr_ptr = (one->collection_ptr)[i];
        ++curr_ptr;
    }
    for (size_t i = 0; i < two->logSize; ++i) {
        *curr_ptr = (two->collection_ptr)[i];
        ++curr_ptr;
    }

    // heapify each level of the heap
    // DO NOT USE size_t since we need level to be 0
    int curr_level = floor(log2(merged_HeapPQueue_ptr->logSize)) - 1;
    while (curr_level > -1) {
        for (int idx = pow(2, curr_level); idx < pow(2, curr_level + 1); ++idx) {
            merged_HeapPQueue_ptr->heapify(idx - 1);
        }
        --curr_level;
    }

    delete one;
    delete two;

    return merged_HeapPQueue_ptr;
}
