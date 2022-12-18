#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {
    logSize = 0;
    collection_ptr = new Vector<string>;
}

VectorPQueue::~VectorPQueue() {
    delete collection_ptr;
}

const string& VectorPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
//    return kEmptyString;
    if (logSize == 0) {
        throw "No item in the VectorPQueue!";
    }
    int min_idx = 0;
    for (int i = 1; i < collection_ptr->size(); ++i) {
        if (collection_ptr->get(i) < collection_ptr->get(min_idx)) {
            min_idx = i;
        }
    }
    return collection_ptr->get(min_idx);
}

string VectorPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
//    return peek();
    string min_str = peek();
    collection_ptr->removeValue(min_str);
    --logSize;
    return min_str;
}

void VectorPQueue::enqueue(const string& elem/* elem */) {
	// placeholder so method compiles..
	// replace with your own implementation
    collection_ptr->add(elem);
    ++logSize;
}

VectorPQueue *VectorPQueue::merge(VectorPQueue *one /* one */, VectorPQueue *two /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
//	return new VectorPQueue();

    auto merged_VectorPQueue_ptr = new VectorPQueue();
    *merged_VectorPQueue_ptr->collection_ptr = *one->collection_ptr + *two->collection_ptr;
    merged_VectorPQueue_ptr->logSize = one->logSize + two->logSize;
    delete one;
    delete two;
    return merged_VectorPQueue_ptr;
}
